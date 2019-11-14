# externally defined macros: BUILD={debug, release} LIBTYPE={glibc, musl}
ifeq ($(LIBTYPE),glibc)
	CXXFLAGS_LIBTYPE=-flto
	LDFLAGS_LIBTYPE=-flto
else
ifeq ($(LIBTYPE),musl)
	CXXFLAGS_LIBTYPE=
	LDFLAGS_LIBTYPE=
else
$(error The LIBTYPE macro is undefined. Should be one of {glibc, musl})
endif
endif

CXX=g++
LD=g++
BIN_LIBTYPE=bin_gcc_$(LIBTYPE)
OBJ_LIBTYPE=obj_gcc_$(LIBTYPE)
MBCSO_LIBTYPE=./../mbcdll/$(BIN_LIBTYPE)
MBC_INC=./../mbc
MBCDLL_INC=./../mbcdll
INC=-I$(MBCDLL_INC)/include -I$(MBC_INC)/include -I$(MBC_INC)/include/mbc
LDFLAGS=-m64 -L$(MBCDLL_INC)/$(BIN_LIBTYPE) -fvisibility=hidden $(LDFLAGS_LIBTYPE)
CXXDEFS=
STANDARDLIBS=-ldl -lpthread

# -m64        The -m64 option sets int to 32 bits and long and pointer types to 64 bits, and generates code for the x86-64 architecture.
# -flto       Runs the standard link-time optimizer. To use the link-time optimizer, -flto and optimization options should be specified at compile time and during the final link.
# -Wall       Enables all the warnings about constructions that some users consider questionable.
# -std=c++17  Compile as C++17.
CXXFLAGS=-m64 -Wall -pedantic -std=c++17 -fvisibility=hidden $(CXXFLAGS_LIBTYPE)

# https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html

# -O3                 Turns on all optimizations specified by -O2 and also turns on the following optimization flags:
#                     -fgcse-after-reload -finline-functions -fipa-cp-clone -floop-interchange -floop-unroll-and-jam -fpeel-loops -fpredictive-commoning
#                     -fsplit-paths -ftree-loop-distribute-patterns -ftree-loop-distribution -ftree-loop-vectorize -ftree-partial-pre -ftree-slp-vectorize
#                     -funswitch-loops -fvect-cost-model -fversion-loops-for-strides.
# -Ofast              Enables all -O3 optimizations and also enables optimizations that are not valid for all standard-compliant programs.
#                     It turns on -ffast-math and the Fortran-specific -fstack-arrays, unless -fmax-stack-var-size is specified, and -fno-protect-parens.
# -frename-registers  Attempt to avoid false dependencies in scheduled code by making use of registers left over after register allocation.
#                     This optimization most benefits processors with lots of registers. Depending on the debug information format adopted by the target,
#                     however, it can make debugging impossible, since variables no longer stay in a “home register”. Enabled by default with -funroll-loops.
# -funroll-loops      Unroll loops whose number of iterations can be determined at compile time or upon entry to the loop. Implies -frerun-cse-after-loop,
#                     -fweb and -frename-registers. It also turns on complete loop peeling (i.e. complete removal of loops with a small constant number of iterations).
#                     This option makes code larger, and may or may not make it run faster. Enabled by -fprofile-use and -fauto-profile.
# -Ofast sets -ffast-math which makes std::isnan() not working. 
CXXFLAGS_RELEASE=-O3 -frename-registers -funroll-loops

# https://gcc.gnu.org/onlinedocs/gcc/Debugging-Options.html

# -O0  Reduce compilation time and make debugging produce the expected results.
# -g3  includes extra information, such as all the macro definitions present in the program. Some debuggers support macro expansion when you use -g3.
CXXFLAGS_DEBUG=-O0 -g3

# =====================================================================================================
ifeq ($(BUILD),release)
	CXXFLAGS += $(CXXFLAGS_RELEASE)
	BINNAME=mbcdll_tests
	CUSTOMLIBS=-lmbc
        MBCSO=$(MBCSO_LIBTYPE)/libmbc.so
else
ifeq ($(BUILD),debug)
	CXXFLAGS += $(CXXFLAGS_DEBUG) 
	BINNAME=mbcdll_tests_d
	CUSTOMLIBS=-lmbcd
        MBCSO=$(MBCSO_LIBTYPE)/libmbcd.so
else
$(error The BUILD macro is undefined. Should be one of {release, debug})
endif
endif

# =====================================================================================================
OBJDIR=$(OBJ_LIBTYPE)/$(BUILD)
BINDIR=$(BIN_LIBTYPE)
TESTEXE=$(BINDIR)/$(BINNAME)

# -----------------------------------------------------------------------------------------------------
.PHONY: all

all: create_dirs $(TESTEXE)

clean:
	rm -f ./$(TESTEXE)
	rm -f ./$(TESTEXE).testresults.log
	rm -f ./$(TESTEXE).testresults.err
	rm -f ./$(TESTEXE).make.log
	rm -f ./$(TESTEXE).make.err
	rm -rf ./$(OBJDIR)

create_dirs:
	mkdir -p ./$(BINDIR)
	mkdir -p ./$(OBJDIR)

OBJLIST=$(OBJDIR)/mbcdll_tests.o \
$(OBJDIR)/h5capiTestInfo.o $(OBJDIR)/h5capiTests.Common.o \
$(OBJDIR)/h5capiTests.File.o $(OBJDIR)/h5capiTests.Instrument.o $(OBJDIR)/h5capiTests.Data.o \
$(OBJDIR)/h5capiTests.OhlcvData.o $(OBJDIR)/h5capiTests.OhlcvPriceOnlyData.o \
$(OBJDIR)/h5capiTests.QuoteData.o $(OBJDIR)/h5capiTests.QuotePriceOnlyData.o \
$(OBJDIR)/h5capiTests.TradeData.o $(OBJDIR)/h5capiTests.TradePriceOnlyData.o \
$(OBJDIR)/h5capiTests.ScalarData.o

$(TESTEXE): $(OBJLIST)
	$(LD) $(LDFLAGS) -o $(TESTEXE) $(OBJLIST) $(CUSTOMLIBS) $(STANDARDLIBS)
	strip $(TESTEXE)
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(MBCSO_LIBTYPE) $(TESTEXE) -m >$(BINDIR)/$(BINNAME).testresults.log 2>$(BINDIR)/$(BINNAME).testresults.err

# -----------------------------------------------------------------------------------------------------
$(OBJDIR)/mbcdll_tests.o: mbcdll_tests.cpp ../mbcdll/include/crtDebug.h ../mbc/include/mbc/testharness_shell.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/mbcdll_tests.o -c mbcdll_tests.cpp

$(OBJDIR)/h5capiTestInfo.o: mbcdll/h5capi/h5capiTestInfo.cpp mbcdll/h5capi/h5capiTestInfo.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5capiTestInfo.o -c mbcdll/h5capi/h5capiTestInfo.cpp

$(OBJDIR)/h5capiTests.Common.o: mbcdll/h5capi/h5capiTests.Common.cpp mbcdll/h5capi/h5capiTestInfo.h ../mbcdll/include/h5capi.h ../mbc/include/mbc/testharness.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5capiTests.Common.o -c mbcdll/h5capi/h5capiTests.Common.cpp

$(OBJDIR)/h5capiTests.File.o: mbcdll/h5capi/h5capiTests.File.cpp mbcdll/h5capi/h5capiTestInfo.h ../mbcdll/include/h5capi.h ../mbc/include/mbc/testharness.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5capiTests.File.o -c mbcdll/h5capi/h5capiTests.File.cpp

$(OBJDIR)/h5capiTests.Instrument.o: mbcdll/h5capi/h5capiTests.Instrument.cpp mbcdll/h5capi/h5capiTestInfo.h ../mbcdll/include/h5capi.h ../mbc/include/mbc/testharness.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5capiTests.Instrument.o -c mbcdll/h5capi/h5capiTests.Instrument.cpp

$(OBJDIR)/h5capiTests.Data.o: mbcdll/h5capi/h5capiTests.Data.cpp ../mbcdll/include/h5capi.h ../mbc/include/mbc/testharness.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5capiTests.Data.o -c mbcdll/h5capi/h5capiTests.Data.cpp

$(OBJDIR)/h5capiTests.OhlcvData.o: mbcdll/h5capi/h5capiTests.OhlcvData.cpp ../mbcdll/include/h5capi.h ../mbc/include/mbc/testharness.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5capiTests.OhlcvData.o -c mbcdll/h5capi/h5capiTests.OhlcvData.cpp

$(OBJDIR)/h5capiTests.OhlcvPriceOnlyData.o: mbcdll/h5capi/h5capiTests.OhlcvPriceOnlyData.cpp ../mbcdll/include/h5capi.h ../mbc/include/mbc/testharness.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5capiTests.OhlcvPriceOnlyData.o -c mbcdll/h5capi/h5capiTests.OhlcvPriceOnlyData.cpp

$(OBJDIR)/h5capiTests.QuoteData.o: mbcdll/h5capi/h5capiTests.QuoteData.cpp ../mbcdll/include/h5capi.h ../mbc/include/mbc/testharness.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5capiTests.QuoteData.o -c mbcdll/h5capi/h5capiTests.QuoteData.cpp

$(OBJDIR)/h5capiTests.QuotePriceOnlyData.o: mbcdll/h5capi/h5capiTests.QuotePriceOnlyData.cpp ../mbcdll/include/h5capi.h ../mbc/include/mbc/testharness.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5capiTests.QuotePriceOnlyData.o -c mbcdll/h5capi/h5capiTests.QuotePriceOnlyData.cpp

$(OBJDIR)/h5capiTests.TradeData.o: mbcdll/h5capi/h5capiTests.TradeData.cpp ../mbcdll/include/h5capi.h ../mbc/include/mbc/testharness.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5capiTests.TradeData.o -c mbcdll/h5capi/h5capiTests.TradeData.cpp

$(OBJDIR)/h5capiTests.TradePriceOnlyData.o: mbcdll/h5capi/h5capiTests.TradePriceOnlyData.cpp ../mbcdll/include/h5capi.h ../mbc/include/mbc/testharness.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5capiTests.TradePriceOnlyData.o -c mbcdll/h5capi/h5capiTests.TradePriceOnlyData.cpp

$(OBJDIR)/h5capiTests.ScalarData.o: mbcdll/h5capi/h5capiTests.ScalarData.cpp ../mbcdll/include/h5capi.h ../mbc/include/mbc/testharness.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5capiTests.ScalarData.o -c mbcdll/h5capi/h5capiTests.ScalarData.cpp

# -----------------------------------------------------------------------------------------------------
