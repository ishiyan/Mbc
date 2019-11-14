# externally defined macro: RUNTIME={MT, MTd, MD, MDd}

OBJDIR=obj_msvc
LIBDIR=lib_msvc
LIBNAME_BASE=libmbc

DEPS=..\dependencies_prebuilt\msvc
INC=/I "include\mbc" /I "$(DEPS)\include_hdf5" /I "$(DEPS)\include_zlib" /I "$(DEPS)\include_szip"
LIBFLAGS=/MACHINE:X64 /NOLOGO /LIBPATH:"$(DEPS)\lib"
CDEFS=/D "WIN32" /D "_WIN64" /D "WIN64" /D "X64" /D "_LIB" /D "_CRT_SECURE_NO_WARNINGS" /D "_UNICODE" /D "UNICODE"

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
!IF "$(RUNTIME)" == "MT" || "$(RUNTIME)" == "MD"
# release definitions
CDEFS=$(CDEFS) /D "NDEBUG"

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

LIBFLAGS=$(LIBFLAGS) /LTCG

# ----------------------
!IF "$(RUNTIME)" == "MT"
OBJDIR=$(OBJDIR)\mt
LIBNAME_BASE=$(LIBNAME_BASE)mt
CFLAGS=$(CFLAGS) /MT
# ----------------------
!ELSE
OBJDIR=$(OBJDIR)\md
LIBNAME_BASE=$(LIBNAME_BASE)md
CFLAGS=$(CFLAGS) /MD
!ENDIF
# ----------------------
# =====================================================================================================
!ELSE IF "$(RUNTIME)" == "MTd" || "$(RUNTIME)" == "MDd"
# debug definitions
CDEFS=$(CDEFS) /D "_DEBUG"

# /JMC    Specifies compiler support for native Just My Code debugging in the Visual Studio debugger.
# /GS     Detects some buffer overruns that overwrite a function's return address, exception handler address, or certain types of parameters. 
# /sdl    Enables a superset of the baseline security checks provided by /GS and overrides /GS-.
# /Zi     Generates complete debugging information.
# /Od     Turns off all optimizations in the program and speeds compilation.
# /RTC1   Equivalent of /RTCsu. 's' enables stack frame run-time error checking, 'u' reports when a variable is used without having been initialized.
# /RTCc   Reports when a value is assigned to a smaller data type and results in a data loss. For example, if a value of type short 0x101 is assigned to a variable of type char.
CFLAGS=$(CFLAGS) /JMC /GS /sdl /Zi /Od /RTC1

# ----------------------
!IF "$(RUNTIME)" == "MTd"
OBJDIR=$(OBJDIR)\mtd
LIBNAME_BASE=$(LIBNAME_BASE)mtd
CFLAGS=$(CFLAGS) /MTd
# ----------------------
!ELSE
OBJDIR=$(OBJDIR)\mdd
LIBNAME_BASE=$(LIBNAME_BASE)mdd
CFLAGS=$(CFLAGS) /MDd
!ENDIF
# ----------------------
# =====================================================================================================
!ELSE
!ERROR The RUNTIME macro is undefined. Should be one of {MT, MTd, MD, MDd}.
!ENDIF
# =====================================================================================================

LIBNAME_BASE=$(LIBDIR)\$(LIBNAME_BASE)
LIBNAME_BASE_NMAKE=$(LIBNAME_BASE).nmake
LIBNAME=$(LIBNAME_BASE).lib
LIBFLAGS=$(LIBFLAGS) /OUT:"$(LIBNAME)"

# -----------------------------------------------------------------------------------------------------
all: create_dirs $(LIBNAME)

clean:
	if exist $(LIBNAME) del /f /s /q $(LIBNAME)
	if exist $(LIBNAME_BASE_NMAKE).log del /f /s /q $(LIBNAME_BASE_NMAKE).log
	if exist $(LIBNAME_BASE_NMAKE).err del /f /s /q $(LIBNAME_BASE_NMAKE).err
	if exist $(OBJDIR) rmdir /s /q $(OBJDIR)

create_dirs:
	if not exist $(LIBDIR) mkdir $(LIBDIR)
	if not exist $(OBJDIR) mkdir $(OBJDIR)

OBJLIST=$(OBJDIR)\h5.obj $(OBJDIR)\exchange.obj

$(LIBNAME): $(OBJLIST)
	lib $(LIBFLAGS) $(OBJLIST)
	if exist vc140.pdb del /f /s /q vc140.pdb

# -----------------------------------------------------------------------------------------------------
$(OBJDIR)\h5.obj: src\h5.cpp include\mbc\h5.h include\mbc\time\dotNetTicks.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\h5.obj" /c src\h5.cpp

$(OBJDIR)\exchange.obj: src\trading\markets\exchange.cpp include\mbc\trading\markets\exchange.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\exchange.obj" /c src\trading\markets\exchange.cpp

# -----------------------------------------------------------------------------------------------------
src\h5.cpp: include\mbc\h5.h include\mbc\time\dotNetTicks.h

src\trading\markets\exchange.cpp: include\mbc\trading\markets\exchange.h
