# externally defined macro: RUNTIME={MD, MDd}

OBJDIR=obj_msvc
BINDIR=bin_msvc
BINNAME=mbcdll_tests
DLLBIN=..\mbcdll\bin_msvc\mbc
DLLPDB=..\mbcdll\bin_msvc\mbc
MBC_INC=..\mbc
MBCDLL_INC=..\mbcdll
INC=/I "$(MBCDLL_INC)\include" /I "$(MBC_INC)\include"
LINKFLAGS=/SUBSYSTEM:CONSOLE /LARGEADDRESSAWARE /MACHINE:X64 /NOLOGO /MANIFEST:EMBED /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /INCREMENTAL:NO /DYNAMICBASE:NO /RELEASE /ERRORREPORT:PROMPT /TLBID:1 /LIBPATH:"$(MBCDLL_INC)\bin_msvc"
STANDARDLIBS="kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib"
CUSTOMLIBS=
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
!IF "$(RUNTIME)" == "MD"
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

LINKFLAGS=$(LINKFLAGS) /LTCG /OPT:REF /OPT:ICF=4

OBJDIR=$(OBJDIR)\md
BINNAME=$(BINNAME)_md
CFLAGS=$(CFLAGS) /MD
CUSTOMLIBS=$(CUSTOMLIBS) "mbc.lib"
# =====================================================================================================
!ELSE IF "$(RUNTIME)" == "MDd"
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

LINKFLAGS=$(LINKFLAGS) /OPT:NOREF /OPT:NOICF

OBJDIR=$(OBJDIR)\mdd
BINNAME=$(BINNAME)_mdd
CFLAGS=$(CFLAGS) /MDd
CUSTOMLIBS=$(CUSTOMLIBS) "mbcd.lib"
DLLBIN=$(DLLBIN)d
DLLPDB=$(DLLPDB)d
# =====================================================================================================
!ELSE
!ERROR The RUNTIME macro is undefined. Should be one of {MD, MDd}.
!ENDIF
# =====================================================================================================

DLLBIN=$(DLLBIN).dll
DLLPDB=$(DLLPDB).pdb

BINNAME=$(BINDIR)\$(BINNAME)
TESTEXE=$(BINNAME).exe

LINKFLAGS=$(LINKFLAGS) /OUT:"$(TESTEXE)"

# -----------------------------------------------------------------------------------------------------
all: create_dirs $(TESTEXE)

clean:
	if exist $(TESTEXE) del /f /s /q $(TESTEXE)
	if exist $(BINNAME).exp del /f /s /q $(BINNAME).exp
	if exist $(BINNAME).lib del /f /s /q $(BINNAME).lib
	if exist $(BINNAME).pdb del /f /s /q $(BINNAME).pdb
	if exist $(BINNAME).nmake.log del /f /s /q $(BINNAME).nmake.log
	if exist $(BINNAME).nmake.log err /f /s /q $(BINNAME).nmake.err
	if exist $(BINNAME).testresults.log del /f /s /q $(BINNAME).testresults.log
	if exist $(BINNAME).testresults.log err /f /s /q $(BINNAME).testresults.err
	if exist $(OBJDIR) rmdir /s /q $(OBJDIR)

create_dirs:
	if not exist $(BINDIR) mkdir $(BINDIR)
	if not exist $(OBJDIR) mkdir $(OBJDIR)

OBJLIST=$(OBJDIR)\mbcdll_tests.obj \
$(OBJDIR)\h5capiTestInfo.obj $(OBJDIR)\h5capiTests.Common.obj \
$(OBJDIR)\h5capiTests.File.obj $(OBJDIR)\h5capiTests.Instrument.obj $(OBJDIR)\h5capiTests.Data.obj \
$(OBJDIR)\h5capiTests.OhlcvData.obj $(OBJDIR)\h5capiTests.OhlcvPriceOnlyData.obj \
$(OBJDIR)\h5capiTests.QuoteData.obj $(OBJDIR)\h5capiTests.QuotePriceOnlyData.obj \
$(OBJDIR)\h5capiTests.TradeData.obj $(OBJDIR)\h5capiTests.TradePriceOnlyData.obj \
$(OBJDIR)\h5capiTests.ScalarData.obj

$(TESTEXE): $(OBJLIST)
	link $(LINKFLAGS) $(OBJLIST) $(CUSTOMLIBS) $(STANDARDLIBS)
	if exist "vc140.pdb" del /f /s /q "vc140.pdb"
	if exist "$(BINNAME).lib" del /f /s /q "$(BINNAME).lib"
	if exist "$(BINNAME).exp" del /f /s /q "$(BINNAME).exp"
	if exist "$(DLLBIN)" copy /y "$(DLLBIN)" "$(BINDIR)"
	if exist "$(DLLPDB)" copy /y "$(DLLPDB)" "$(BINDIR)"
	$(TESTEXE) -m >$(BINNAME).testresults.log

# -----------------------------------------------------------------------------------------------------
$(OBJDIR)\mbcdll_tests.obj: mbcdll_tests.cpp ..\mbcdll\include\crtDebug.h ..\mbc\include\mbc\testharness_shell.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\mbcdll_tests.obj" /c mbcdll_tests.cpp

$(OBJDIR)\h5capiTestInfo.obj: mbcdll\h5capi\h5capiTestInfo.cpp mbcdll\h5capi\h5capiTestInfo.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\h5capiTestInfo.obj" /c mbcdll\h5capi\h5capiTestInfo.cpp

$(OBJDIR)\h5capiTests.Common.obj: mbcdll\h5capi\h5capiTests.Common.cpp mbcdll\h5capi\h5capiTestInfo.h ..\mbcdll\include\h5capi.h ..\mbc\include\mbc\testharness.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\h5capiTests.Common.obj" /c mbcdll\h5capi\h5capiTests.Common.cpp

$(OBJDIR)\h5capiTests.File.obj: mbcdll\h5capi\h5capiTests.File.cpp mbcdll\h5capi\h5capiTestInfo.h ..\mbcdll\include\h5capi.h ..\mbc\include\mbc\testharness.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\h5capiTests.File.obj" /c mbcdll\h5capi\h5capiTests.File.cpp

$(OBJDIR)\h5capiTests.Instrument.obj: mbcdll\h5capi\h5capiTests.Instrument.cpp mbcdll\h5capi\h5capiTestInfo.h ..\mbcdll\include\h5capi.h ..\mbc\include\mbc\testharness.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\h5capiTests.Instrument.obj" /c mbcdll\h5capi\h5capiTests.Instrument.cpp

$(OBJDIR)\h5capiTests.Data.obj: mbcdll\h5capi\h5capiTests.Data.cpp ..\mbcdll\include\h5capi.h ..\mbc\include\mbc\testharness.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\h5capiTests.Data.obj" /c mbcdll\h5capi\h5capiTests.Data.cpp

$(OBJDIR)\h5capiTests.OhlcvData.obj: mbcdll\h5capi\h5capiTests.OhlcvData.cpp ..\mbcdll\include\h5capi.h ..\mbc\include\mbc\testharness.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\h5capiTests.OhlcvData.obj" /c mbcdll\h5capi\h5capiTests.OhlcvData.cpp

$(OBJDIR)\h5capiTests.OhlcvPriceOnlyData.obj: mbcdll\h5capi\h5capiTests.OhlcvPriceOnlyData.cpp ..\mbcdll\include\h5capi.h ..\mbc\include\mbc\testharness.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\h5capiTests.OhlcvPriceOnlyData.obj" /c mbcdll\h5capi\h5capiTests.OhlcvPriceOnlyData.cpp

$(OBJDIR)\h5capiTests.QuoteData.obj: mbcdll\h5capi\h5capiTests.QuoteData.cpp ..\mbcdll\include\h5capi.h ..\mbc\include\mbc\testharness.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\h5capiTests.QuoteData.obj" /c mbcdll\h5capi\h5capiTests.QuoteData.cpp

$(OBJDIR)\h5capiTests.QuotePriceOnlyData.obj: mbcdll\h5capi\h5capiTests.QuotePriceOnlyData.cpp ..\mbcdll\include\h5capi.h ..\mbc\include\mbc\testharness.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\h5capiTests.QuotePriceOnlyData.obj" /c mbcdll\h5capi\h5capiTests.QuotePriceOnlyData.cpp

$(OBJDIR)\h5capiTests.TradeData.obj: mbcdll\h5capi\h5capiTests.TradeData.cpp ..\mbcdll\include\h5capi.h ..\mbc\include\mbc\testharness.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\h5capiTests.TradeData.obj" /c mbcdll\h5capi\h5capiTests.TradeData.cpp

$(OBJDIR)\h5capiTests.TradePriceOnlyData.obj: mbcdll\h5capi\h5capiTests.TradePriceOnlyData.cpp ..\mbcdll\include\h5capi.h ..\mbc\include\mbc\testharness.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\h5capiTests.TradePriceOnlyData.obj" /c mbcdll\h5capi\h5capiTests.TradePriceOnlyData.cpp

$(OBJDIR)\h5capiTests.ScalarData.obj: mbcdll\h5capi\h5capiTests.ScalarData.cpp ..\mbcdll\include\h5capi.h ..\mbc\include\mbc\testharness.h
	cl $(CDEFS) $(INC) $(CFLAGS) /Fo"$(OBJDIR)\h5capiTests.ScalarData.obj" /c mbcdll\h5capi\h5capiTests.ScalarData.cpp

# -----------------------------------------------------------------------------------------------------
