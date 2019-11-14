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
LIB_LIBTYPE=lib_gcc_$(LIBTYPE)
OBJ_LIBTYPE=obj_gcc_$(LIBTYPE)
MBC_LIBTYPE=./../mbc/$(LIB_LIBTYPE)
DEPS_LIBTYPE=./../dependencies_prebuilt/gcc_$(LIBTYPE)
MBC_INC=./../mbc
INC=-I$(MBC_INC)/include -I$(MBC_INC)/include/mbc -I$(DEPS_LIBTYPE)/include_hdf5 -I$(DEPS_LIBTYPE)/include_zlib -I$(DEPS_LIBTYPE)/include_szip
LDFLAGS=-m64 -L$(MBC_INC)/$(LIB_LIBTYPE) -L$(DEPS_LIBTYPE)/lib -fvisibility=hidden $(LDFLAGS_LIBTYPE)
CXXDEFS=
STANDARDLIBS=-ldl -lpthread

# -m64        The -m64 option sets int to 32 bits and long and pointer types to 64 bits, and generates code for the x86-64 architecture.
# -flto       Runs the standard link-time optimizer. To use the link-time optimizer, -flto and optimization options should be specified at compile time and during the final link.
# -Wall       Enables all the warnings about constructions that some users consider questionable.
# -std=c++17  Compile as C++17.
CXXFLAGS=-m64 -Wall -pedantic -std=c++17 -pthread -fvisibility=hidden $(CXXFLAGS_LIBTYPE)

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
	BINNAME=mbc_tests
	CUSTOMLIBS=-lmbc -lhdf5 -lzlib -lszip
        MBCLIB=$(MBC_LIBTYPE)/libmbc.a
else
ifeq ($(BUILD),debug)
	CXXFLAGS += $(CXXFLAGS_DEBUG) 
	BINNAME=mbc_tests_d
	CUSTOMLIBS=-lmbcd -lhdf5d -lzlibd -lszipd
        MBCLIB=$(MBC_LIBTYPE)/libmbcd.a
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

OBJLIST=$(OBJDIR)/mbc_tests.o $(OBJDIR)/crtDebug.o $(OBJDIR)/testharnessTests.o $(OBJDIR)/dotNetTicksTests.o \
$(OBJDIR)/entitiesTests.o $(OBJDIR)/exchangeTests.o \
$(OBJDIR)/mersenneTwisterSfmtTests607.o $(OBJDIR)/mersenneTwisterSfmtTests1279.o $(OBJDIR)/mersenneTwisterSfmtTests2281.o \
$(OBJDIR)/mersenneTwisterSfmtTests4253.o $(OBJDIR)/mersenneTwisterSfmtTests11213.o $(OBJDIR)/mersenneTwisterSfmtTests44497.o \
$(OBJDIR)/mersenneTwisterSfmtTests86243.o $(OBJDIR)/mersenneTwisterSfmtTests132049.o $(OBJDIR)/mersenneTwisterSfmtTests216091.o \
$(OBJDIR)/h5Tests.Common.o $(OBJDIR)/h5Tests.File.o $(OBJDIR)/h5Tests.Instrument.o $(OBJDIR)/h5Tests.Data.o \
$(OBJDIR)/h5Tests.OhlcvData.o $(OBJDIR)/h5Tests.OhlcvData.Exploratory.o $(OBJDIR)/h5Tests.OhlcvPriceOnlyData.o $(OBJDIR)/h5Tests.OhlcvPriceOnlyData.Exploratory.o \
$(OBJDIR)/h5Tests.QuoteData.o $(OBJDIR)/h5Tests.QuoteData.Exploratory.o $(OBJDIR)/h5Tests.QuotePriceOnlyData.o $(OBJDIR)/h5Tests.QuotePriceOnlyData.Exploratory.o \
$(OBJDIR)/h5Tests.TradeData.o $(OBJDIR)/h5Tests.TradeData.Exploratory.o $(OBJDIR)/h5Tests.TradePriceOnlyData.o $(OBJDIR)/h5Tests.TradePriceOnlyData.Exploratory.o \
$(OBJDIR)/h5Tests.ScalarData.o $(OBJDIR)/h5Tests.ScalarData.Exploratory.o

$(TESTEXE): $(OBJLIST) $(MBCLIB)
	$(LD) $(LDFLAGS) -o $(TESTEXE) $(OBJLIST) $(CUSTOMLIBS) $(STANDARDLIBS)
	strip $(TESTEXE)
	$(TESTEXE) -m >$(BINDIR)/$(BINNAME).testresults.log 2>$(BINDIR)/$(BINNAME).testresults.err

# -----------------------------------------------------------------------------------------------------
$(OBJDIR)/mbc_tests.o: mbc_tests.cpp ../mbc/include/mbc/testharness_shell.h ../mbc/include/mbc/testharness.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/mbc_tests.o -c mbc_tests.cpp

$(OBJDIR)/crtDebug.o: crtDebug.cpp ../mbc/include/mbc/testharness_shell.h ../mbc/include/mbc/testharness.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/crtDebug.o -c crtDebug.cpp

$(OBJDIR)/testharnessTests.o: mbc/testharnessTests.cpp ../mbc/include/mbc/testharness.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/testharnessTests.o -c mbc/testharnessTests.cpp

$(OBJDIR)/dotNetTicksTests.o: mbc/time/dotNetTicksTests.cpp ../mbc/include/mbc/testharness.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/dotNetTicksTests.o -c mbc/time/dotNetTicksTests.cpp

$(OBJDIR)/entitiesTests.o: mbc/trading/data/entitiesTests.cpp ../mbc/include/mbc/testharness.h ../mbc/include/mbc/trading/data/entities.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/entitiesTests.o -c mbc/trading/data/entitiesTests.cpp

$(OBJDIR)/exchangeTests.o: mbc/trading/markets/exchangeTests.cpp ../mbc/include/mbc/testharness.h ../mbc/include/mbc/trading/markets/exchange.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/exchangeTests.o -c mbc/trading/markets/exchangeTests.cpp


$(OBJDIR)/mersenneTwisterSfmtTests607.o: mbc/numerics/random/mersenneTwisterSfmtTests607.cpp ../mbc/include/mbc/testharness.h mbc/numerics/random/mersenneTwisterSfmtTests.h ../mbc/include/mbc/numerics/random/mersenneTwisterSfmt.h ../mbc/include/mbc/numerics/random/randomGenerator.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/mersenneTwisterSfmtTests607.o -c mbc/numerics/random/mersenneTwisterSfmtTests607.cpp

$(OBJDIR)/mersenneTwisterSfmtTests1279.o: mbc/numerics/random/mersenneTwisterSfmtTests1279.cpp ../mbc/include/mbc/testharness.h mbc/numerics/random/mersenneTwisterSfmtTests.h ../mbc/include/mbc/numerics/random/mersenneTwisterSfmt.h ../mbc/include/mbc/numerics/random/randomGenerator.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/mersenneTwisterSfmtTests1279.o -c mbc/numerics/random/mersenneTwisterSfmtTests1279.cpp

$(OBJDIR)/mersenneTwisterSfmtTests2281.o: mbc/numerics/random/mersenneTwisterSfmtTests2281.cpp ../mbc/include/mbc/testharness.h mbc/numerics/random/mersenneTwisterSfmtTests.h ../mbc/include/mbc/numerics/random/mersenneTwisterSfmt.h ../mbc/include/mbc/numerics/random/randomGenerator.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/mersenneTwisterSfmtTests2281.o -c mbc/numerics/random/mersenneTwisterSfmtTests2281.cpp

$(OBJDIR)/mersenneTwisterSfmtTests4253.o: mbc/numerics/random/mersenneTwisterSfmtTests4253.cpp ../mbc/include/mbc/testharness.h mbc/numerics/random/mersenneTwisterSfmtTests.h ../mbc/include/mbc/numerics/random/mersenneTwisterSfmt.h ../mbc/include/mbc/numerics/random/randomGenerator.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/mersenneTwisterSfmtTests4253.o -c mbc/numerics/random/mersenneTwisterSfmtTests4253.cpp

$(OBJDIR)/mersenneTwisterSfmtTests11213.o: mbc/numerics/random/mersenneTwisterSfmtTests11213.cpp ../mbc/include/mbc/testharness.h mbc/numerics/random/mersenneTwisterSfmtTests.h ../mbc/include/mbc/numerics/random/mersenneTwisterSfmt.h ../mbc/include/mbc/numerics/random/randomGenerator.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/mersenneTwisterSfmtTests11213.o -c mbc/numerics/random/mersenneTwisterSfmtTests11213.cpp

$(OBJDIR)/mersenneTwisterSfmtTests19937.o: mbc/numerics/random/mersenneTwisterSfmtTests19937.cpp ../mbc/include/mbc/testharness.h mbc/numerics/random/mersenneTwisterSfmtTests.h ../mbc/include/mbc/numerics/random/mersenneTwisterSfmt.h ../mbc/include/mbc/numerics/random/randomGenerator.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/mersenneTwisterSfmtTests19937.o -c mbc/numerics/random/mersenneTwisterSfmtTests19937.cpp

$(OBJDIR)/mersenneTwisterSfmtTests44497.o: mbc/numerics/random/mersenneTwisterSfmtTests44497.cpp ../mbc/include/mbc/testharness.h mbc/numerics/random/mersenneTwisterSfmtTests.h ../mbc/include/mbc/numerics/random/mersenneTwisterSfmt.h ../mbc/include/mbc/numerics/random/randomGenerator.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/mersenneTwisterSfmtTests44497.o -c mbc/numerics/random/mersenneTwisterSfmtTests44497.cpp

$(OBJDIR)/mersenneTwisterSfmtTests86243.o: mbc/numerics/random/mersenneTwisterSfmtTests86243.cpp ../mbc/include/mbc/testharness.h mbc/numerics/random/mersenneTwisterSfmtTests.h ../mbc/include/mbc/numerics/random/mersenneTwisterSfmt.h ../mbc/include/mbc/numerics/random/randomGenerator.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/mersenneTwisterSfmtTests86243.o -c mbc/numerics/random/mersenneTwisterSfmtTests86243.cpp

$(OBJDIR)/mersenneTwisterSfmtTests132049.o: mbc/numerics/random/mersenneTwisterSfmtTests132049.cpp ../mbc/include/mbc/testharness.h mbc/numerics/random/mersenneTwisterSfmtTests.h ../mbc/include/mbc/numerics/random/mersenneTwisterSfmt.h ../mbc/include/mbc/numerics/random/randomGenerator.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/mersenneTwisterSfmtTests132049.o -c mbc/numerics/random/mersenneTwisterSfmtTests132049.cpp

$(OBJDIR)/mersenneTwisterSfmtTests216091.o: mbc/numerics/random/mersenneTwisterSfmtTests216091.cpp ../mbc/include/mbc/testharness.h mbc/numerics/random/mersenneTwisterSfmtTests.h ../mbc/include/mbc/numerics/random/mersenneTwisterSfmt.h ../mbc/include/mbc/numerics/random/randomGenerator.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/mersenneTwisterSfmtTests216091.o -c mbc/numerics/random/mersenneTwisterSfmtTests216091.cpp


$(OBJDIR)/h5Tests.Common.o: mbc/h5/h5Tests.Common.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.Common.o -c mbc/h5/h5Tests.Common.cpp

$(OBJDIR)/h5Tests.File.o: mbc/h5/h5Tests.File.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.File.o -c mbc/h5/h5Tests.File.cpp

$(OBJDIR)/h5Tests.Instrument.o: mbc/h5/h5Tests.Instrument.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.Instrument.o -c mbc/h5/h5Tests.Instrument.cpp

$(OBJDIR)/h5Tests.Data.o: mbc/h5/h5Tests.Data.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.Data.o -c mbc/h5/h5Tests.Data.cpp

$(OBJDIR)/h5Tests.OhlcvData.o: mbc/h5/h5Tests.OhlcvData.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.OhlcvData.o -c mbc/h5/h5Tests.OhlcvData.cpp

$(OBJDIR)/h5Tests.OhlcvData.Exploratory.o: mbc/h5/h5Tests.OhlcvData.Exploratory.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.OhlcvData.Exploratory.o -c mbc/h5/h5Tests.OhlcvData.Exploratory.cpp

$(OBJDIR)/h5Tests.OhlcvPriceOnlyData.o: mbc/h5/h5Tests.OhlcvPriceOnlyData.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.OhlcvPriceOnlyData.o -c mbc/h5/h5Tests.OhlcvPriceOnlyData.cpp

$(OBJDIR)/h5Tests.OhlcvPriceOnlyData.Exploratory.o: mbc/h5/h5Tests.OhlcvPriceOnlyData.Exploratory.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.OhlcvPriceOnlyData.Exploratory.o -c mbc/h5/h5Tests.OhlcvPriceOnlyData.Exploratory.cpp

$(OBJDIR)/h5Tests.QuoteData.o: mbc/h5/h5Tests.QuoteData.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.QuoteData.o -c mbc/h5/h5Tests.QuoteData.cpp

$(OBJDIR)/h5Tests.QuoteData.Exploratory.o: mbc/h5/h5Tests.QuoteData.Exploratory.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.QuoteData.Exploratory.o -c mbc/h5/h5Tests.QuoteData.Exploratory.cpp

$(OBJDIR)/h5Tests.QuotePriceOnlyData.o: mbc/h5/h5Tests.QuotePriceOnlyData.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.QuotePriceOnlyData.o -c mbc/h5/h5Tests.QuotePriceOnlyData.cpp

$(OBJDIR)/h5Tests.QuotePriceOnlyData.Exploratory.o: mbc/h5/h5Tests.QuotePriceOnlyData.Exploratory.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.QuotePriceOnlyData.Exploratory.o -c mbc/h5/h5Tests.QuotePriceOnlyData.Exploratory.cpp

$(OBJDIR)/h5Tests.TradeData.o: mbc/h5/h5Tests.TradeData.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.TradeData.o -c mbc/h5/h5Tests.TradeData.cpp

$(OBJDIR)/h5Tests.TradeData.Exploratory.o: mbc/h5/h5Tests.TradeData.Exploratory.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.TradeData.Exploratory.o -c mbc/h5/h5Tests.TradeData.Exploratory.cpp

$(OBJDIR)/h5Tests.TradePriceOnlyData.o: mbc/h5/h5Tests.TradePriceOnlyData.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.TradePriceOnlyData.o -c mbc/h5/h5Tests.TradePriceOnlyData.cpp

$(OBJDIR)/h5Tests.TradePriceOnlyData.Exploratory.o: mbc/h5/h5Tests.TradePriceOnlyData.Exploratory.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.TradePriceOnlyData.Exploratory.o -c mbc/h5/h5Tests.TradePriceOnlyData.Exploratory.cpp

$(OBJDIR)/h5Tests.ScalarData.o: mbc/h5/h5Tests.ScalarData.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.ScalarData.o -c mbc/h5/h5Tests.ScalarData.cpp

$(OBJDIR)/h5Tests.ScalarData.Exploratory.o: mbc/h5/h5Tests.ScalarData.Exploratory.cpp mbc/h5/h5Tests.Common.h ../mbc/include/mbc/testharness.h ../mbc/include/mbc/h5.h ../mbc/include/mbc/time/dotNetTicks.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5Tests.ScalarData.Exploratory.o -c mbc/h5/h5Tests.ScalarData.Exploratory.cpp

# -----------------------------------------------------------------------------------------------------
