# externally defined macros: LIBTYPE={glibc, musl}
ifeq ($(LIBTYPE),glibc)
	# noop
else
ifeq ($(LIBTYPE),musl)
	# noop
else
$(error The LIBTYPE macro is undefined. Should be one of {glibc, musl})
endif
endif

MKFILE=makefile.gcc.mk LIBTYPE=$(LIBTYPE)

OBJ_DIR=obj_gcc_$(LIBTYPE)
BIN_DIR=bin_gcc_$(LIBTYPE)
LIB_DIR=lib_gcc_$(LIBTYPE)

MBC_DIR=mbc
MBC_TESTS_DIR=mbc_tests
MBCDLL_DIR=mbcdll
MBCDLL_TESTS_DIR=mbcdll_tests
MBCS_DIR=mbcs
MBCS_TESTS_DIR=mbcs_tests

MBCH5_LS_DIR=mbc_h5tools/mbch5ls
MBCH5_EXP_DIR=mbc_h5tools/mbch5exp

MBCSH5_LS_DIR=mbcs_h5tools/mbcsh5ls

.PHONY: all

all: build

rebuild: clean build

build:  mbc_libs mbc_h5tools mbcs_h5tools

mbc_libs: mbc_static_lib mbc_static_lib_tests mbc_dynamic_lib mbc_dynamic_lib_tests mbcs_lib mbcs_lib_tests

mbc_h5tools: mbc_h5ls mbc_h5exp

mbcs_h5tools: mbcs_h5ls

clean: clean_obj clean_bin

clean_obj:
	rm -rf ./$(MBC_DIR)/$(OBJ_DIR)
	rm -rf ./$(MBC_TESTS_DIR)/$(OBJ_DIR)
	rm -rf ./$(MBCDLL_DIR)/$(OBJ_DIR)
	rm -rf ./$(MBCDLL_TESTS_DIR)/$(OBJ_DIR)
	rm -rf ./$(MBCH5_LS_DIR)/$(OBJ_DIR)
	rm -rf ./$(MBCH5_EXP_DIR)/$(OBJ_DIR)

clean_bin:
	rm -rf ./$(MBC_DIR)/$(LIB_DIR)
	rm -rf ./$(MBC_TESTS_DIR)/$(BIN_DIR)
	rm -rf ./$(MBCDLL_DIR)/$(BIN_DIR)
	rm -rf ./$(MBCDLL_TESTS_DIR)/$(BIN_DIR)
	rm -rf ./$(MBCH5_LS_DIR)/$(BIN_DIR)
	rm -rf ./$(MBCH5_EXP_DIR)/$(BIN_DIR)

mbc_static_lib:
	mkdir -p $(MBC_DIR)/$(LIB_DIR)
	make -C ./$(MBC_DIR) -f $(MKFILE) BUILD=release     >>./$(MBC_DIR)/$(LIB_DIR)/libmbc.make.log     2>>./$(MBC_DIR)/$(LIB_DIR)/libmbc.make.err
	make -C ./$(MBC_DIR) -f $(MKFILE) BUILD=release_pic >>./$(MBC_DIR)/$(LIB_DIR)/libmbcpic.make.log  2>>./$(MBC_DIR)/$(LIB_DIR)/libmbcpic.make.err
	make -C ./$(MBC_DIR) -f $(MKFILE) BUILD=debug       >>./$(MBC_DIR)/$(LIB_DIR)/libmbcd.make.log    2>>./$(MBC_DIR)/$(LIB_DIR)/libmbcd.make.err
	make -C ./$(MBC_DIR) -f $(MKFILE) BUILD=debug_pic   >>./$(MBC_DIR)/$(LIB_DIR)/libmbcpicd.make.log 2>>./$(MBC_DIR)/$(LIB_DIR)/libmbcpicd.make.err

mbc_static_lib_tests: mbc_static_lib
	mkdir -p $(MBC_TESTS_DIR)/$(BIN_DIR)
	make -C ./$(MBC_TESTS_DIR) -f $(MKFILE) BUILD=release >>./$(MBC_TESTS_DIR)/$(BIN_DIR)/mbc_tests.make.log   2>>./$(MBC_TESTS_DIR)/$(BIN_DIR)/mbc_tests.make.err
	make -C ./$(MBC_TESTS_DIR) -f $(MKFILE) BUILD=debug   >>./$(MBC_TESTS_DIR)/$(BIN_DIR)/mbc_tests_d.make.log 2>>./$(MBC_TESTS_DIR)/$(BIN_DIR)/mbc_tests_d.make.err
	# to run tests manually:
	# ./$(MBC_TESTS_DIR)/$(BIN_DIR)/mbc_tests
	# ./$(MBC_TESTS_DIR)/$(BIN_DIR)/mbc_tests_d

mbc_dynamic_lib: mbc_static_lib
	mkdir -p $(MBCDLL_DIR)/$(BIN_DIR)
	make -C ./$(MBCDLL_DIR) -f $(MKFILE) BUILD=release_pic >>./$(MBCDLL_DIR)/$(BIN_DIR)/mbcdll.make.log   2>>./$(MBCDLL_DIR)/$(BIN_DIR)/mbcdll.make.err
	make -C ./$(MBCDLL_DIR) -f $(MKFILE) BUILD=debug_pic   >>./$(MBCDLL_DIR)/$(BIN_DIR)/mbcdll_d.make.log 2>>./$(MBCDLL_DIR)/$(BIN_DIR)/mbcdll_d.make.err

mbc_dynamic_lib_tests: mbc_dynamic_lib
	mkdir -p $(MBCDLL_TESTS_DIR)/$(BIN_DIR)
	make -C ./$(MBCDLL_TESTS_DIR) -f $(MKFILE) BUILD=release >>./$(MBCDLL_TESTS_DIR)/$(BIN_DIR)/mbcdll_tests.make.log   2>>./$(MBCDLL_TESTS_DIR)/$(BIN_DIR)/mbcdll_tests.make.err
	make -C ./$(MBCDLL_TESTS_DIR) -f $(MKFILE) BUILD=debug   >>./$(MBCDLL_TESTS_DIR)/$(BIN_DIR)/mbcdll_tests_d.make.log 2>>./$(MBCDLL_TESTS_DIR)/$(BIN_DIR)/mbcdll_tests_d.make.err
	# to run tests manually:
	# LD_LIBRARY_PATH=D_LIBRARY_PATH:./$(MBCDLL_DIR)/$(BIN_DIR) ./$(MBCDLL_TESTS_DIR)/$(BIN_DIR)/mbcdll_tests
	# LD_LIBRARY_PATH=D_LIBRARY_PATH:./$(MBCDLL_DIR)/$(BIN_DIR) ./$(MBCDLL_TESTS_DIR)/$(BIN_DIR)/mbcdll_tests_d
	# or:
	# cd ./$(MBCDLL_TESTS_DIR)/$(BIN_DIR)
	# LD_LIBRARY_PATH=D_LIBRARY_PATH:../../$(MBCDLL_DIR)/$(BIN_DIR) ./mbcdll_tests
	# LD_LIBRARY_PATH=D_LIBRARY_PATH:../../$(MBCDLL_DIR)/$(BIN_DIR) ./mbcdll_tests_d

mbcs_lib: mbc_dynamic_lib
	mkdir -p $(MBCS_DIR)/$(BIN_DIR)
	make -C ./$(MBCS_DIR) -f $(MKFILE) BUILD=release >>./$(MBCS_DIR)/$(BIN_DIR)/mbcs.make.log   2>>./$(MBCS_DIR)/$(BIN_DIR)/mbcs.make.err
	make -C ./$(MBCS_DIR) -f $(MKFILE) BUILD=debug   >>./$(MBCS_DIR)/$(BIN_DIR)/mbcs_d.make.log 2>>./$(MBCS_DIR)/$(BIN_DIR)/mbcs_d.make.err

mbcs_lib_tests: mbcs_lib
	mkdir -p $(MBCS_TESTS_DIR)/$(BIN_DIR)
	make -C ./$(MBCS_TESTS_DIR) -f $(MKFILE) BUILD=release >>./$(MBCS_TESTS_DIR)/$(BIN_DIR)/mbcs_tests.make.log   2>>./$(MBCS_TESTS_DIR)/$(BIN_DIR)/mbcs_tests.make.err
	make -C ./$(MBCS_TESTS_DIR) -f $(MKFILE) BUILD=debug   >>./$(MBCS_TESTS_DIR)/$(BIN_DIR)/mbcs_tests_d.make.log 2>>./$(MBCS_TESTS_DIR)/$(BIN_DIR)/mbcs_tests_d.make.err
	# to run tests manually:
	# dotnet vstest ./$(MBCS_TESTS_DIR)/$(BIN_DIR)/Release_md/MbcsTests.dll
	# dotnet vstest ./$(MBCS_TESTS_DIR)/$(BIN_DIR)/Debug_md/MbcsTests.dll

mbc_h5ls: mbc_static_lib
	mkdir -p $(MBCH5_LS_DIR)/$(BIN_DIR)
	make -C ./$(MBCH5_LS_DIR) -f $(MKFILE) BUILD=release >>./$(MBCH5_LS_DIR)/$(BIN_DIR)/mbch5ls.make.log   2>>./$(MBCH5_LS_DIR)/$(BIN_DIR)/mbch5ls.make.err
	make -C ./$(MBCH5_LS_DIR) -f $(MKFILE) BUILD=debug   >>./$(MBCH5_LS_DIR)/$(BIN_DIR)/mbch5ls_d.make.log 2>>./$(MBCH5_LS_DIR)/$(BIN_DIR)/mbch5ls_d.make.err
	# to run manually:
	# ./$(MBCH5_LS_DIR)/$(BIN_DIR)/mbch5ls --help
	# ./$(MBCH5_LS_DIR)/$(BIN_DIR)/mbch5ls_d --help

mbc_h5exp: mbc_static_lib
	mkdir -p $(MBCH5_EXP_DIR)/$(BIN_DIR)
	make -C ./$(MBCH5_EXP_DIR) -f $(MKFILE) BUILD=release >>./$(MBCH5_EXP_DIR)/$(BIN_DIR)/mbch5exp.make.log   2>>./$(MBCH5_EXP_DIR)/$(BIN_DIR)/mbch5exp.make.err
	make -C ./$(MBCH5_EXP_DIR) -f $(MKFILE) BUILD=debug   >>./$(MBCH5_EXP_DIR)/$(BIN_DIR)/mbch5exp_d.make.log 2>>./$(MBCH5_EXP_DIR)/$(BIN_DIR)/mbch5exp_d.make.err
	# to run manually:
	# ./$(MBCH5_EXP_DIR)/$(BIN_DIR)/mbch5exp --help
	# ./$(MBCH5_EXP_DIR)/$(BIN_DIR)/mbch5exp_d --help
	# ./$(MBCH5_EXP_DIR)/$(BIN_DIR)/mbch5exp ./$(MBCH5_EXP_DIR)/tests/null.h5:/XPAR_null_FI0009000681/t____ -dt ymd/hms.f
	# ./$(MBCH5_EXP_DIR)/$(BIN_DIR)/mbch5exp ./$(MBCH5_EXP_DIR)/tests/null.h5:/XPAR_null_FI0009000681/j_d01 -dt ymd/
	# ./$(MBCH5_EXP_DIR)/$(BIN_DIR)/mbch5exp ./$(MBCH5_EXP_DIR)/tests/ebf.h5:/ebf/EURIBOR1M/s_d01 -dt ymd-
	# ./$(MBCH5_EXP_DIR)/$(BIN_DIR)/mbch5exp ./$(MBCH5_EXP_DIR)/tests/foo.h5
	# ./$(MBCH5_EXP_DIR)/$(BIN_DIR)/mbch5exp ./$(MBCH5_EXP_DIR)/tests/nonexistent.h5

mbcs_h5ls: mbcs_lib
	mkdir -p $(MBCSH5_LS_DIR)/$(BIN_DIR)
	make -C ./$(MBCSH5_LS_DIR) -f $(MKFILE) BUILD=release >>./$(MBCSH5_LS_DIR)/$(BIN_DIR)/mbcsh5ls.make.log   2>>./$(MBCSH5_LS_DIR)/$(BIN_DIR)/mbcsh5ls.make.err
	make -C ./$(MBCSH5_LS_DIR) -f $(MKFILE) BUILD=debug   >>./$(MBCSH5_LS_DIR)/$(BIN_DIR)/mbcsh5ls_d.make.log 2>>./$(MBCSH5_LS_DIR)/$(BIN_DIR)/mbcsh5ls_d.make.err
	# to run manually:
	# ./$(MBCSH5_LS_DIR)/$(BIN_DIR)/Release_md/Mbcsh5ls -h
	# ./$(MBCSH5_LS_DIR)/$(BIN_DIR)/Debug_md/Mbcsh5ls -h

# -----------------------------------------------------------------------------------------------------
DEPS_LIBTYPE=./dependencies_prebuilt/gcc_$(LIBTYPE)
COLLECTDIR=./last_prebuilt/linux-$(LIBTYPE)-x64
COLLECTDIR_CPP=$(COLLECTDIR)/c++
COLLECTDIR_CAPI=$(COLLECTDIR)/capi
COLLECTDIR_CS=$(COLLECTDIR)/cs
COLLECTDIR_H5=$(COLLECTDIR)/h5

collect_last_prebuilt:
	# c++
	mkdir -p $(COLLECTDIR_CPP)/include/hdf5
	cp -t $(COLLECTDIR_CPP)/include/hdf5 $(DEPS_LIBTYPE)/include_hdf5/*.h
	mkdir -p $(COLLECTDIR_CPP)/include/zlib
	cp -t $(COLLECTDIR_CPP)/include/zlib $(DEPS_LIBTYPE)/include_zlib/*.h
	mkdir -p $(COLLECTDIR_CPP)/include/szip
	cp -t $(COLLECTDIR_CPP)/include/szip $(DEPS_LIBTYPE)/include_szip/*.h
	cp -rt $(COLLECTDIR_CPP)/include ./mbc/include/mbc
	mkdir -p $(COLLECTDIR_CPP)/lib
	cp -t $(COLLECTDIR_CPP)/lib $(DEPS_LIBTYPE)/lib/*.a ./mbc/lib_gcc_$(LIBTYPE)/*.a
	mkdir -p $(COLLECTDIR_CPP)/tests
	cp -t $(COLLECTDIR_CPP)/tests ./mbc_tests/bin_gcc_$(LIBTYPE)/mbc_tests ./mbc_tests/bin_gcc_$(LIBTYPE)/mbc_tests_d
	cd $(COLLECTDIR) && XZ_OPT=-9e tar cJf ./c++_$(shell date '+%Y-%m-%d_%H-%M-%S').tar.xz ./c++
	rm -rf $(COLLECTDIR_CPP)
	# c api
	mkdir -p $(COLLECTDIR_CAPI)/include
	cp -rt $(COLLECTDIR_CAPI)/include ./mbcdll/include/*.h
	cp -rt $(COLLECTDIR_CAPI) ./mbcdll/bin_gcc_$(LIBTYPE)/libmbc.so.1.0.1 ./mbcdll/bin_gcc_$(LIBTYPE)/libmbc.so ./mbcdll/bin_gcc_$(LIBTYPE)/libmbcd.so.1.0.1 ./mbcdll/bin_gcc_$(LIBTYPE)/libmbcd.so
	mkdir -p $(COLLECTDIR_CAPI)/tests
	cp -t $(COLLECTDIR_CAPI)/tests ./mbcdll_tests/bin_gcc_$(LIBTYPE)/mbcdll_tests ./mbcdll_tests/bin_gcc_$(LIBTYPE)/mbcdll_tests_d
	echo "#!/bin/sh" >$(COLLECTDIR_CAPI)/tests/mbcdll_tests.sh
	echo "LD_LIBRARY_PATH=D_LIBRARY_PATH:../ ./mbcdll_tests" >>$(COLLECTDIR_CAPI)/tests/mbcdll_tests.sh
	echo "#!/bin/sh" >$(COLLECTDIR_CAPI)/tests/mbcdll_tests_d.sh
	echo "LD_LIBRARY_PATH=D_LIBRARY_PATH:../ ./mbcdll_tests_d" >>$(COLLECTDIR_CAPI)/tests/mbcdll_tests_d.sh
	cd $(COLLECTDIR) && XZ_OPT=-9e tar cJf ./capi_$(shell date '+%Y-%m-%d_%H-%M-%S').tar.xz ./capi
	rm -rf $(COLLECTDIR_CAPI)
	# cs
	mkdir -p $(COLLECTDIR_CS)/release
	cp -t $(COLLECTDIR_CS)/release ./mbcs/bin_gcc_$(LIBTYPE)/Release_md/Mbcs.dll ./mbcs/bin_gcc_$(LIBTYPE)/Release_md/Mbcs.pdb ./mbcs/bin_gcc_$(LIBTYPE)/Release_md/Mbcs.xml ./mbcs/bin_gcc_$(LIBTYPE)/Release_md/mbc.so ./mbcs/bin_gcc_$(LIBTYPE)/Release_md/Mbcs.deps.json
	mkdir -p $(COLLECTDIR_CS)/debug
	cp -t $(COLLECTDIR_CS)/debug ./mbcs/bin_gcc_$(LIBTYPE)/Debug_md/Mbcs.dll ./mbcs/bin_gcc_$(LIBTYPE)/Debug_md/Mbcs.pdb ./mbcs/bin_gcc_$(LIBTYPE)/Debug_md/Mbcs.xml ./mbcs/bin_gcc_$(LIBTYPE)/Debug_md/mbcd.so ./mbcs/bin_gcc_$(LIBTYPE)/Debug_md/Mbcs.deps.json
	mkdir -p $(COLLECTDIR_CS)/tests/release
	cp -t $(COLLECTDIR_CS)/tests/release ./mbcs_tests/bin_gcc_$(LIBTYPE)/Release_md/*.*
	echo "#!/bin/sh" >$(COLLECTDIR_CS)/tests/release/MbcsTests.sh
	echo "dotnet vstest MbcsTests.dll" >>$(COLLECTDIR_CS)/tests/release/MbcsTests.sh
	mkdir -p $(COLLECTDIR_CS)/tests/debug
	cp -t $(COLLECTDIR_CS)/tests/debug ./mbcs_tests/bin_gcc_$(LIBTYPE)/Debug_md/*.*
	echo "#!/bin/sh" >$(COLLECTDIR_CS)/tests/debug/MbcsTests.sh
	echo "dotnet vstest MbcsTests.dll" >>$(COLLECTDIR_CS)/tests/debug/MbcsTests.sh
	cd $(COLLECTDIR) && XZ_OPT=-9e tar cJf ./cs_$(shell date '+%Y-%m-%d_%H-%M-%S').tar.xz ./cs
	rm -rf $(COLLECTDIR_CS)
	# h5
	mkdir -p $(COLLECTDIR_H5)/hdf5
	cp -t $(COLLECTDIR_H5) ./mbc_h5tools/mbch5ls/bin_gcc_$(LIBTYPE)/mbch5ls ./mbc_h5tools/mbch5exp/bin_gcc_$(LIBTYPE)/mbch5exp
	cp -t $(COLLECTDIR_H5)/hdf5 $(DEPS_LIBTYPE)/tools_hdf5/*
	cd $(COLLECTDIR) && XZ_OPT=-9e tar cJf ./h5_$(shell date '+%Y-%m-%d_%H-%M-%S').tar.xz ./h5
	rm -rf $(COLLECTDIR_H5)
