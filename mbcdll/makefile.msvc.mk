# externally defined macro: RUNTIME={MD, MDd}

OBJDIR=obj_msvc
BINDIR=bin_msvc
DLLNAME=mbc
DEPS=..\dependencies_prebuilt\msvc
MBC_INC=..\mbc
INC=/I "$(MBC_INC)\include" /I "$(DEPS)\include_hdf5" /I "$(DEPS)\include_zlib" /I "$(DEPS)\include_szip"
LINKFLAGS=/SUBSYSTEM:WINDOWS /LARGEADDRESSAWARE /MACHINE:X64 /DLL /NOLOGO /MANIFEST:EMBED /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /INCREMENTAL:NO /DYNAMICBASE /NXCOMPAT /RELEASE /ERRORREPORT:PROMPT /TLBID:1 /LIBPATH:"$(MBC_INC)\lib_msvc" /LIBPATH:"$(DEPS)\lib"
STANDARDLIBS="kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib"
CUSTOMLIBS=
CDEFS=/D "WIN32" /D "_WIN64" /D "WIN64" /D "X64" /D "_USRDLL" /D "_WINDLL" /D "DLL_EXPORTS" /D "_CRT_SECURE_NO_WARNINGS" /D "_UNICODE" /D "UNICODE"
RCDEFS=/l"0x0409" /nologo /D "_UNICODE" /D "UNICODE"

# /W{3, 4, all}        Displays all warnings displayed by /W4 and all other warnings that /W4 does not include—for example, warnings that are off by default.
# /WX                  Treats all compiler warnings as errors.
# /Zc:wchar_t          wchar_t is a native type, not a typedef (on by default).
# /Zc:inline           Remove unreferenced function or data if it is COMDAT or has internal linkage only (off by default).
# /Zc:forScope         Enforce Standard C++ for scoping rules (on by default).
# /std:c++latest       Controls the version-specific ISO C++ programming language standard features enabled during compilation of your code.
# /Gd                  Specifies the __cdecl calling convention for all functions except C++ member functions and functions that are marked __stdcall, __fastcall, or __vectorcall.
# /FC                  Causes the compiler to display the full path of source code files passed to the compiler in diagnostics.
# /EHsc                Catches C++ exceptions only and tells the compiler to assume that functions declared as extern "C" never throw a C++ exception.
# /nologo              Suppresses the display of the copyright banner when the compiler starts up and display of informational messages during compiling.
# /diagnostics:column  Also includes the column where the issue was found.
# /fp:{fast, precise, strict}
# /errorReport:{none, prompt, send}
CFLAGS=/W4 /WX- /Zc:inline /Zc:wchar_t /Zc:forScope /std:c++17 /Gd /FC /EHsc /nologo /diagnostics:column /fp:precise /errorReport:prompt

# =====================================================================================================
!IF "$(RUNTIME)" == "MD"
# release definitions
CDEFS=$(CDEFS) /D "NDEBUG"
RCDEFS=$(RCDEFS) /D "NDEBUG"

# /JMC-          Specifies compiler support for native Just My Code debugging in the Visual Studio debugger.
# /GS-           Detects some buffer overruns that overwrite a function's return address, exception handler address, or certain types of parameters.
# /sdl           Enables a superset of the baseline security checks provided by /GS and overrides /GS-.
# /Qpar          Enables the Auto-Parallelizer feature of the compiler to automatically parallelize loops in your code.
# /Qpar-report:1 Outputs an informational message for loops that are parallelized.
# /Qpar-report:2 Outputs an informational message for loops that are parallelized and also for loops that are not parallelized, together with a reason code.
# /Qvec-report:2 Enables the reporting feature of the compiler Auto-Vectorizer and specifies the level of informational messages for output during compilation.
# /GL            Enables whole program optimization.
# /Gy            Enables Function-Level Linking. Allows the compiler to package individual functions in the form of packaged functions (COMDATs).
# /GF            Eliminate Duplicate Strings. Enables the compiler to create a single copy of identical strings in the program image and in memory during execution.
# /Og            Is deprecated. These optimizations are now generally enabled by default.
# /Oi            Generate Intrinsic Functions. Replaces some function calls with intrinsic or otherwise special forms of the function that help your application run faster.
# /Ot            Favor Fast Code, maximizes the speed of EXEs and DLLs by instructing the compiler to favor speed over size.
# /Oy            Frame-Pointer Omission.Suppresses creation of frame pointers on the call stack. Not available in x64 compiler.
# /Ob3           This option specifies more aggressive inlining than /Ob2. Allows the compiler to expand any function not explicitly marked for no inlining.
# /O2 = /Og /Oi /Ot /Oy /Ob2 /GF /Gy
# /Ox = /Og /Oi /Ot /Oy /Ob2
CFLAGS=$(CFLAGS) /JMC- /GS- /Qpar /Qpar-report:1 /GL /Gy /GF /Oi /Ot /Ob3

LINKFLAGS=$(LINKFLAGS) /LTCG /OPT:REF /OPT:ICF=4

OBJDIR=$(OBJDIR)\md
CFLAGS=$(CFLAGS) /MD
CUSTOMLIBS=$(CUSTOMLIBS) "libmbcmd.lib" "libhdf5md.lib" "libzlibmd.lib" "libszipmd.lib"
# =====================================================================================================
!ELSE IF "$(RUNTIME)" == "MDd"
# debug definitions
DLLNAME=$(DLLNAME)d
CDEFS=$(CDEFS) /D "_DEBUG"
RCDEFS=$(RCDEFS) /D "_DEBUG"

# /JMC    Specifies compiler support for native Just My Code debugging in the Visual Studio debugger.
# /GS     Detects some buffer overruns that overwrite a function's return address, exception handler address, or certain types of parameters. 
# /sdl    Enables a superset of the baseline security checks provided by /GS and overrides /GS-.
# /Zi     Generates complete debugging information.
# /Od     Turns off all optimizations in the program and speeds compilation.
# /RTC1   Equivalent of /RTCsu. 's' enables stack frame run-time error checking, 'u' reports when a variable is used without having been initialized.
# /RTCc   Reports when a value is assigned to a smaller data type and results in a data loss. For example, if a value of type short 0x101 is assigned to a variable of type char.
CFLAGS=$(CFLAGS) /JMC /GS /sdl /Zi /Od /RTC1

LINKFLAGS=$(LINKFLAGS) /OPT:NOREF /OPT:NOICF

OBJDIR=$(OBJDIR)\mdd
CFLAGS=$(CFLAGS) /MDd
CUSTOMLIBS=$(CUSTOMLIBS) "libmbcmdd.lib" "libhdf5mdd.lib" "libzlibmdd.lib" "libszipmdd.lib"
# =====================================================================================================
!ELSE
!ERROR The RUNTIME macro is undefined. Should be one of {MD, MDd}.
!ENDIF
# =====================================================================================================

DLLNAME=$(BINDIR)\$(DLLNAME)
DLL=$(DLLNAME).dll
RES=$(OBJDIR)\mbcdll.res

LINKFLAGS=$(LINKFLAGS) /OUT:"$(DLL)"

# -----------------------------------------------------------------------------------------------------
all: create_dirs $(DLL)

clean:
	if exist $(DLL) del /f /s /q $(DLL)
	if exist $(DLLNAME).exp del /f /s /q $(DLLNAME).exp
	if exist $(DLLNAME).lib del /f /s /q $(DLLNAME).lib
	if exist $(DLLNAME).pdb del /f /s /q $(DLLNAME).pdb
	if exist $(DLLNAME).nmake.log del /f /s /q $(DLLNAME).nmake.log
	if exist $(DLLNAME).nmake.err del /f /s /q $(DLLNAME).nmake.err
	if exist $(OBJDIR) rmdir /s /q $(OBJDIR)

create_dirs:
	if not exist $(BINDIR) mkdir $(BINDIR)
	if not exist $(OBJDIR) mkdir $(OBJDIR)

OBJLIST=$(OBJDIR)/dllmain.obj $(OBJDIR)/crtDebug.obj $(OBJDIR)/h5capi.obj

$(DLL): $(OBJLIST) $(RES)
	link $(LINKFLAGS) $(RES) $(OBJLIST) $(CUSTOMLIBS) $(STANDARDLIBS)
	if exist "vc140.pdb" del /f /s /q "vc140.pdb"

# -----------------------------------------------------------------------------------------------------
$(OBJDIR)\dllmain.obj: src\dllmain.cpp include\crtDebug.h include\visibility.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\dllmain.obj" /c src\dllmain.cpp

$(OBJDIR)\crtDebug.obj: src\crtDebug.cpp include\crtDebug.h include\visibility.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\crtDebug.obj" /c src\crtDebug.cpp

$(OBJDIR)\h5capi.obj: src\h5capi.cpp include\h5capi.h include\visibility.h ..\mbc\include\mbc\h5.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\h5capi.obj" /c src\h5capi.cpp

$(RES): mbcdll.rc
	rc $(RCDEFS) /Fo"$(RES)" mbcdll.rc

# -----------------------------------------------------------------------------------------------------
