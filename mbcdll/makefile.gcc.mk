# externally defined macros: BUILD={debug_pic, release_pic} LIBTYPE={glibc, musl}
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
MBC_LIBTYPE=./../mbc/lib_gcc_$(LIBTYPE)
DEPS_LIBTYPE=./../dependencies_prebuilt/gcc_$(LIBTYPE)
MBC_INC=./../mbc
INC=-I$(MBC_INC)/include -I$(MBC_INC)/include/mbc -I$(DEPS_LIBTYPE)/include_hdf5 -I$(DEPS_LIBTYPE)/include_zlib -I$(DEPS_LIBTYPE)/include_szip
LDFLAGS=-m64 -L$(MBC_INC)/$(LIB_LIBTYPE) -L$(DEPS_LIBTYPE)/lib -shared -fvisibility=hidden $(LDFLAGS_LIBTYPE)
CXXDEFS=-D DLL_EXPORTS
STANDARDLIBS=

# -m64        The -m64 option sets int to 32 bits and long and pointer types to 64 bits, and generates code for the x86-64 architecture.
# -flto       Runs the standard link-time optimizer. To use the link-time optimizer, -flto and optimization options should be specified at compile time and during the final link.
# -Wall       Enables all the warnings about constructions that some users consider questionable.
# -std=c++17  Compile as C++17.
CXXFLAGS=-m64 -flto -Wall -pedantic -std=c++17 -pthread -fPIC -fvisibility=hidden $(CXXFLAGS_LIBTYPE)

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
ifeq ($(BUILD),release_pic)
	CXXFLAGS += $(CXXFLAGS_RELEASE)
	BINNAME=libmbc
	CUSTOMLIBS=-lmbcpic -lhdf5pic -lzlibpic -lszippic
        MBCLIB=$(MBC_LIBTYPE)/libmbcpic.a
	MBCSO=mbc.so
else
ifeq ($(BUILD),debug_pic)
	CXXFLAGS += $(CXXFLAGS_DEBUG) 
	BINNAME=libmbcd
	CUSTOMLIBS=-lmbcpicd -lhdf5picd -lzlibpicd -lszippicd
        MBCLIB=$(MBC_LIBTYPE)/libmbcpicd.a
	MBCSO=mbcd.so
else
$(error The BUILD macro is undefined. Should be one of {release_pic, debug_pic})
endif
endif

# =====================================================================================================
OBJDIR=$(OBJ_LIBTYPE)/$(BUILD)
BINDIR=$(BIN_LIBTYPE)
SONAME=$(BINNAME).so
SOVERSION=1.0.1
SOVERSIONEDNAME=$(SONAME).$(SOVERSION)
SO=$(BINDIR)/$(SONAME)
SOVERSIONED=$(BINDIR)/$(SOVERSIONEDNAME)

# -----------------------------------------------------------------------------------------------------
.PHONY: all

all: create_dirs $(SOVERSIONED)

clean:
	rm -f ./$(SOVERSIONED)
	rm -f ./$(SO)
	rm -f ./$(BINDIR)/$(MBCSO)
	rm -f ./$(SOVERSIONED).ldd
	rm -f ./$(SOVERSIONED).nm.dynamic
	rm -rf ./$(OBJDIR)

create_dirs:
	mkdir -p ./$(BINDIR)
	mkdir -p ./$(OBJDIR)

OBJLIST=$(OBJDIR)/crtDebug.o $(OBJDIR)/h5capi.o

$(SOVERSIONED): $(OBJLIST) $(MBCLIB)
	$(LD) $(LDFLAGS) -Wl,-soname=$(SOVERSIONEDNAME) -o $(SOVERSIONED) $(OBJLIST) $(CUSTOMLIBS) $(STANDARDLIBS)
	ldd $(SOVERSIONED) >$(SOVERSIONED).ldd
	nm --dynamic $(SOVERSIONED) >$(SOVERSIONED).nm.dynamic
	cp -f $(SOVERSIONED) $(SOVERSIONEDNAME)
	ln -sf ./$(SOVERSIONEDNAME) $(SONAME)
	mv -f $(SONAME) $(SO)
	rm -f $(SOVERSIONEDNAME)
	cp -f $(SOVERSIONED) $(BINDIR)/$(MBCSO)

# -----------------------------------------------------------------------------------------------------

$(OBJDIR)/crtDebug.o: src/crtDebug.cpp include/crtDebug.h include/visibility.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/crtDebug.o -c src/crtDebug.cpp

$(OBJDIR)/h5capi.o: src/h5capi.cpp include/h5capi.h include/visibility.h ../mbc/include/mbc/h5.h
	$(CXX) $(CXXDEFS) $(INC) $(CXXFLAGS) -o $(OBJDIR)/h5capi.o -c src/h5capi.cpp

# -----------------------------------------------------------------------------------------------------
