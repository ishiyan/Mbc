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
MBC_LIBTYPE=./../../mbc/$(LIB_LIBTYPE)
DEPS_LIBTYPE=./../../dependencies_prebuilt/gcc_$(LIBTYPE)
MBC_INC=./../../mbc
INC=-I./../shared -I$(MBC_INC)/include -I$(MBC_INC)/include/mbc -I$(DEPS_LIBTYPE)/include_hdf5 -I$(DEPS_LIBTYPE)/include_zlib -I$(DEPS_LIBTYPE)/include_szip
LDFLAGS=-m64 -L$(MBC_INC)/$(LIB_LIBTYPE) -L$(DEPS_LIBTYPE)/lib -fvisibility=hidden $(LDFLAGS_LIBTYPE)
CXXDEFS=
STANDARDLIBS=-ldl

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
	BINNAME=mbch5exp
	CUSTOMLIBS=-lmbc -lhdf5 -lzlib -lszip
        MBCLIB=$(MBC_LIBTYPE)/libmbc.a
else
ifeq ($(BUILD),debug)
	CXXFLAGS += $(CXXFLAGS_DEBUG) 
	BINNAME=mbch5exp_d
	CUSTOMLIBS=-lmbcd -lhdf5d -lzlibd -lszipd
        MBCLIB=$(MBC_LIBTYPE)/libmbcd.a
else
$(error The BUILD macro is undefined. Should be one of {release, debug})
endif
endif

# =====================================================================================================
OBJDIR=$(OBJ_LIBTYPE)/$(BUILD)
BINDIR=$(BIN_LIBTYPE)
BIN=$(BINDIR)/$(BINNAME)

# -----------------------------------------------------------------------------------------------------
.PHONY: all

all: create_dirs $(BIN)

clean:
	rm -f ./$(BIN)
	rm -f ./$(BIN).make.log
	rm -f ./$(BIN).make.err
	rm -rf ./$(OBJDIR)

create_dirs:
	mkdir -p ./$(BINDIR)
	mkdir -p ./$(OBJDIR)

OBJLIST=$(OBJDIR)/mbch5exp.o

$(BIN): $(OBJLIST) $(MBCLIB)
	$(LD) $(LDFLAGS) -o $(BIN) $(OBJLIST) $(CUSTOMLIBS) $(STANDARDLIBS)
	strip $(BIN)

# -----------------------------------------------------------------------------------------------------
$(OBJDIR)/mbch5exp.o: mbch5exp.cpp ../shared/option_parser.h ../../mbc/include/mbc/h5.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/mbch5exp.o -c mbch5exp.cpp

# -----------------------------------------------------------------------------------------------------
