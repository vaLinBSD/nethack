$ ! vms/vmsbuild.com -- compile and link NetHack 3.1 patchlevel 0	[pr]
$ !
$ ! usage:
$ !   $ set default [.src]	!or [-.-.src] if starting from [.sys.vms]
$ !   $ @[-.sys.vms]vmsbuild  [compiler-option]  [link-option]  [cc-switches]
$ ! options:
$ !	compiler-option :  either "VAXC" or "GNUC" or ""	!default VAXC
$ !	link-option	:  either "SHARE[able]" or "LIB[rary]"	!default SHARE
$ !	cc-switches	:  optional qualifiers for CC (such as "/noOpt/Debug")
$ ! notes:
$ !	If the symbol "CC" is defined, compiler-option is not used.
$ !	The link-option refers to VAXCRTL (C Run-Time Library) handling;
$ !	  to specify it while letting compiler-option default, use "" as
$ !	  the compiler-option.
$ !	To re-link without compiling, use "LINK" as special 'compiler-option';
$ !	  to re-link with GNUC library, 'CC' must begin with "G" (or "g").
$ !	Default wizard definition moved to include/vmsconf.h.
$
$	vaxc_ = "CC/NOLIST/OPTIMIZE=NOINLINE"	    !vaxc v3.x (2.x fixed below)
$	gnuc_ = "GCC"
$	gnulib = "gnu_cc:[000000]gcclib/Library"    !(not used w/ vaxc)
$ ! common CC options (/obj=file doesn't work for GCC 1.36, use rename instead)
$	c_c_  = "/INCLUDE=[-.INCLUDE]"
$	veryold_vms = f$extract(1,1,f$getsyi("VERSION")).eqs."4" -
		.and. f$extract(3,3,f$getsyi("VERSION")).lts."6"
$	if veryold_vms then  c_c_ = c_c_ + "/DEFINE=(""VERYOLD_VMS"")"
$ ! miscellaneous setup
$	ivqual = %x00038240	!DCL-W-IVQUAL (used to check for ancient vaxc)
$	abort := exit %x1000002A
$	cur_dir  = f$environment("DEFAULT")
$	vmsbuild = f$environment("PROCEDURE")
$ ! validate first parameter
$	p1 := 'p1'
$	o_VAXC = 0	!(c_opt substring positions)
$	o_GNUC = 5
$	o_LINK = 10
$	o_SPCL = 15
$	c_opt = f$locate("|"+p1, "|VAXC|GNUC|LINK|SPECIAL|") !5
$     if (c_opt/5)*5 .eq. c_opt then  goto p1_ok
$	copy sys$input: sys$error:	!p1 usage
%first arg is compiler option; it must be one of
       "VAXC" -- use VAX C to compile everything
   or  "GNUC" -- use GNU C to compile everything
   or  "LINK" -- skip compilation, just relink nethack.exe
   or  "SPEC[IAL]" -- just compile and link lev_comp.exe
   or    ""   -- default operation (VAXC unless 'CC' is defined)

Note: if a DCL symbol for CC is defined, "VAXC" and "GNUC" are no-ops.
      If the symbol value begins with "G" (or "g"), then the GNU C
      library will be included in all link operations.  Do not rebuild
      lev_comp with "SPECIAL" unless you have a CC symbol setup with
      the proper options.
$	abort
$p1_ok:
$ ! validate second parameter
$	p2 := 'p2'
$	l_opt = f$locate("|"+p2, "|SHAREABLE|LIBRARY__|") !10
$     if (l_opt/10)*10 .eq. l_opt then	goto p2_ok
$	copy sys$input: sys$error:	!p2 usage
%second arg is VAXCRTL handling; it must be one of
       "SHAREABLE" -- link with SYS$SHARE:VAXCRTL.EXE/SHAREABLE
   or   "LIBRARY"  -- link with SYS$LIBRARY:VAXCRTL.OLB/LIBRARY
   or      ""      -- default operation (use shareable image)

Note: for MicroVMS 4.x, "SHAREABLE" (which is the default) is required.
$	abort
$p2_ok:
$ ! start from a known location -- [.sys.vms], then move to [-.-.src]
$	set default 'f$parse(vmsbuild,,,"DIRECTORY")'
$	set default [-.-.src]	!move to source directory
$ ! compiler setup; if a symbol for "CC" is already defined it will be used
$     if f$type(cc).eqs."STRING" then  goto got_cc
$	cc = vaxc_			!assume "VAXC" requested or defaulted
$	if c_opt.eq.o_GNUC then  goto chk_gcc !explicitly invoked w/ "GNUC" option
$	if c_opt.ne.o_VAXC then  goto got_cc !"SPEC" or "LINK", skip compiler check
$	! we want to prevent function inlining with vaxc v3.x (/opt=noinline)
$	!   but we can't use noInline with v2.x, so need to determine version
$	  set noOn
$	  msgenv = f$environment("MESSAGE")
$	  set message/noFacil/noSever/noIdent/noText
$	  cc/noObject _NLA0:/Include=[]     !strip 'noinline' if error
$	  sts = $status
$	if sts then  goto reset_msg	!3.0 or later will check out OK
$	! must be dealing with vaxc 2.x; ancient version (2.2 or earlier)
$	!   can't handle /include='dir', needs c$include instead
$	  cc = cc - "=NOINLINE" - ",NOINLINE" - "NOINLINE,"
$	  if sts.ne.IVQUAL then  goto reset_msg
$	    define/noLog c$include [-.INCLUDE]
$	    c_c_ = "/DEFINE=(""ANCIENT_VAXC"")"
$	    if veryold_vms then  c_c_ = c_c_ - ")" + ",""VERYOLD_VMS"")"
$reset_msg:
$	  set message 'msgenv'
$	  set On
$	  goto got_cc
$ !
$chk_gcc:
$	cc = gnuc_
$ ! old versions of gcc-vms don't have <varargs.h> or <stdarg.h> available
$	  c_c_ = "/DEFINE=(""USE_OLDARGS"")"
$	  if veryold_vms then  c_c_ = c_c_ - ")" + ",""VERYOLD_VMS"")"
$	  if veryold_vms then  goto chk_gas	!avoid varargs & stdarg
$	  if f$search("gnu_cc_include:[000000]varargs.h").nes."" then -
		c_c_ = "/DEFINE=(""USE_VARARGS"")"
$	  if f$search("gnu_cc_include:[000000]stdarg.h").nes."" then -
		c_c_ = "/DEFINE=(""USE_STDARG"")"
$chk_gas:
$ ! test whether this version of gas handles the 'const' construct correctly
$ gas_chk_tmp = "sys$scratch:gcc-gas-chk.tmp"
$ if f$search(gas_chk_tmp).nes."" then  delete/noconfirm/nolog 'gas_chk_tmp';*
$ gas_ok = 0	!assume bad
$ on warning then goto skip_gas
$ define/user/nolog sys$error 'gas_chk_tmp'
$ mcr gnu_cc:[000000]gcc-as sys$input: -o _NLA0:
$DECK
.const
.comm dummy,0
.const
.comm dummy,0
$EOD
$ gas_ok = 1	!assume good
$ if f$search(gas_chk_tmp).eqs."" then  goto skip_gas
$ ! if the error file is empty, gas can deal properly with const
$  gas_ok = f$file_attrib(gas_chk_tmp,"EOF") .eq. 0
$  delete/noconfirm/nolog 'gas_chk_tmp';*
$skip_gas:
$ on warning then continue
$	  if .not.gas_ok then  c_c_ = c_c_ - ")" + ",""const="")"
$	  c_c_ = "/INCLUDE=[-.INCLUDE]" + c_c_
$ !
$got_cc:
$	cc = cc + c_c_			!append common qualifiers
$	if p3.nes."" then  cc = cc + p3 !append optional user preferences
$	g := 'f$extract(0,1,cc)'
$	if g.nes."G" .and. c_opt.ne.o_GNUC then  gnulib = ""
$	if g.eqs."G"  .or. c_opt.eq.o_GNUC then  gnulib = "," + gnulib
$ ! linker setup; if a symbol for "LINK" is defined, we'll use it
$	if f$type(link).nes."STRING" then  link = "LINK/NOMAP"
$	if p4.nes."" then  link = link + p4 !append optional user preferences
$	crtl = "sys$library:vaxcrtl.olb/Library"	!object library
$     if l_opt.ne.0 then  goto crtl_ok
$	crtl = "sys$disk:[-.src]crtl.opt/Options"	!shareable image
$     if f$search("crtl.opt").nes."" then  goto crtl_ok !assume its right
$	create sys$disk:[-.src]crtl.opt
sys$share:vaxcrtl.exe/Shareable
$crtl_ok:
$ ! final setup
$	nethacklib = "[-.src]nethack.olb"
$	milestone = "write sys$output f$fao("" !5%T "",0),"
$     if c_opt.eq.o_LINK then  goto link  !"LINK" requested, skip compilation
$	rename	 := rename/New_Vers
$	touch	 := set file/Truncate
$	makedefs := $sys$disk:[-.util]makedefs
$	show symbol cc
$	goto begin	!skip subroutines
$!
$compile_file:	!input via 'c_file'
$	no_lib = ( f$extract(0,1,c_file) .eqs. "#" )
$	if no_lib then	c_file = f$extract(1,255,c_file)
$	c_name = f$edit(f$parse(c_file,,,"NAME"),"LOWERCASE")
$	f_opts = ""	!options for this file
$	if f$type('c_name'_options).nes."" then  f_opts = 'c_name'_options
$	milestone " (",c_name,")"
$	if f$search("''c_name'.obj").nes."" then  delete 'c_name'.obj;*
$	cc 'f_opts' 'c_file'.c
$	if .not.no_lib then  nh_obj_list == nh_obj_list + ",''c_name'.obj;0"
$     return
$!
$compile_list:	!input via 'c_list'
$	nh_obj_list == ""
$	j = -1
$ c_loop:
$	j = j + 1
$	c_file = f$element(j,",",c_list)  !get next file
$	if c_file.nes."," then	gosub compile_file
$	if c_file.nes."," then	goto c_loop
$	nh_obj_list == f$extract(1,999,nh_obj_list)
$	if nh_obj_list.nes."" then  libr/Obj 'nethacklib' 'nh_obj_list'/Replace
$	if nh_obj_list.nes."" then  delete 'nh_obj_list'
$	delete/symbol/global nh_obj_list
$     return
$!
$begin:
$!
$! miscellaneous special source file setup
$!
$ if f$search("monst.c").eqs."" then  copy/Concat monst.c1+.c2 monst.c
$ if f$search("random.c").eqs."" then  copy [-.sys.share]random.c []*.*
$ if f$search("[-.util]lev_yacc.c").eqs."" then  @[-.sys.vms]spec_lev.com
$!
$! create object library
$!
$ libr/Obj 'nethacklib'/Create=(Block=3000,Hist=0)
$ if f$search("''nethacklib';-1").nes."" then  purge 'nethacklib'
$!
$! compile and link makedefs, then nethack, finally lev_comp & dgn_comp.
$!
$ milestone "<compiling...>"
$ c_list = "[-.sys.vms]vmsmisc,[]alloc,monst,objects"
$     if c_opt.eq.o_SPCL then  c_list = c_list + ",decl,drawing"
$ gosub compile_list
$     if c_opt.eq.o_SPCL then  goto special !"SPECIAL" requested, skip main build
$ set default [-.util]
$ c_list = "#makedefs"
$ gosub compile_list
$ link makedefs.obj,'nethacklib'/Lib,'crtl''gnulib',sys$input:/Opt
identification="makedefs 3.1.0"
$ milestone "makedefs"
$! create some build-time files
$ makedefs -p	!pm.h
$ makedefs -o	!onames.h
$ makedefs -v	!date.h
$ milestone " (*.h)"
$ makedefs -m	!../src/monstr.c
$ makedefs -z	!../src/vis_tab.c, ../include/vis_tab.h
$ milestone " (*.c)"
$ set default [-.src]
$! compile most of the source files:
$	gnutermcap_options = "/Define=(""bcopy(s,d,n)=memcpy(d,s,n)"",""exit=vms_exit"")"
$	if c_opt.eq.o_GNUC then  gnutermcap_options = "/Define=(""exit=vms_exit"")"  !gcc
$ c_list = "decl,version,[-.sys.vms]vmsmain,[-.sys.vms]vmsunix" -
	+ ",[-.sys.vms]vmstty,[-.sys.vms]vmsmail,[-.sys.vms]vmsfiles" -
	+ ",[]random"	!copied from [-.sys.share]
$ gosub compile_list
$ c_list = "[-.win.tty]getline,[-.win.tty]termcap" -
	+ ",[-.win.tty]topl,[-.win.tty]wintty" -
	+ ",[-.sys.vms]gnutparam,[-.sys.vms]gnutermcap"
$ gosub compile_list
$ c_list = "allmain,apply,artifact,attrib,ball,bones,botl,cmd,dbridge,detect" -
	+ ",display,do,do_name,do_wear,dog,dogmove,dokick,dothrow,drawing" -
	+ ",dungeon,eat,end,engrave,exper,explode,extralev,files,fountain"
$ gosub compile_list
$ c_list = "hack,hacklib,invent,lock,mail,makemon,mcastu,mhitm,mhitu" -
	+ ",minion,mklev,mkmap,mkmaze,mkobj,mkroom,mon,mondata,monmove,monstr" -
	+ ",mplayer,mthrowu,muse,music,o_init,objnam,options,pager,pickup"
$ gosub compile_list
$ c_list = "pline,polyself,potion,pray,priest,quest,questpgr,read" -
	+ ",rect,restore,rip,rnd,rumors,save,shk,shknam,sit,sounds" -
	+ ",sp_lev,spell,steal,timeout,topten,track,trap,u_init"
$ gosub compile_list
$ c_list = "uhitm,vault,vision,vis_tab,weapon,were,wield,windows" -
	+ ",wizard,worm,worn,write,zap"
$ gosub compile_list
$!
$link:
$ milestone "<linking...>"
$ link/Exe=nethack 'nethacklib'/Lib/Incl=(vmsmain),'crtl''gnulib',-
	sys$input:/Opt
identification="NetHack 3.1.0"
$ milestone "NetHack"
$     if c_opt.eq.o_LINK then  goto done	!"LINK" only
$special:
$!
$! build special level and dungeon compilers
$!
$ set default [-.util]
$ c_list = "#panic,#lev_main,#lev_yacc,#dgn_main,#dgn_yacc"
$     if c_opt.eq.o_SPCL then  c_list = "[-.sys.vms]vmsfiles," + c_list
$ gosub compile_list
$ c_list = "#lev_lex,#dgn_lex"
$ copy [-.sys.vms]lev_lex.h stdio.*/Prot=(s:rwd,o:rwd)
$ gosub compile_list
$ rename stdio.h lev_lex.*
$ link/exe=lev_comp lev_main,lev_yacc,lev_lex,-
	panic.obj,'nethacklib'/Lib,'crtl''gnulib',sys$input:/Opt
identification="lev_comp 3.1.0"
$ milestone "lev_comp"
$ link/exe=dgn_comp dgn_main,dgn_yacc,dgn_lex,-
	panic.obj,'nethacklib'/Lib,'crtl''gnulib',sys$input:/Opt
identification="dgn_comp 3.1.0"
$ milestone "dgn_comp"
$!
$ c_list = "#recover"
$ gosub compile_list
$ link/exe=[] recover.obj,'nethacklib'/Lib,'crtl''gnulib',sys$input:/Opt
identification="recover 3.1.0"
$ milestone "recover"
$!
$done:
$	set default 'cur_dir'
$ exit
