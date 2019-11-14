MKFILE=makefile.msvc.mk

OBJ_DIR=obj_msvc
BIN_DIR=bin_msvc
LIB_DIR=lib_msvc

MBC_DIR=mbc
MBC_TESTS_DIR=mbc_tests
MBCDLL_DIR=mbcdll
MBCDLL_TESTS_DIR=mbcdll_tests
MBCS_DIR=mbcs
MBCS_TESTS_DIR=mbcs_tests

MBCH5_LS_DIR=mbc_h5tools\mbch5ls
MBCH5_EXP_DIR=mbc_h5tools\mbch5exp

MBCSH5_LS_DIR=mbcs_h5tools\mbcsh5ls

all: build

rebuild: clean build

build:  mbc_libs mbc_h5tools mbcs_h5tools

mbc_libs: mbc_static_lib mbc_static_lib_tests mbc_dynamic_lib mbc_dynamic_lib_tests mbcs_lib mbcs_lib_tests

mbc_h5tools: mbc_h5ls mbc_h5exp

mbcs_h5tools: mbcs_h5ls

clean: clean_obj clean_bin

clean_bin:
	if exist "$(MBC_DIR)\$(LIB_DIR)" rmdir /s /q "$(MBC_DIR)\$(LIB_DIR)"
	if exist "$(MBC_TESTS_DIR)\$(BIN_DIR)" rmdir /s /q "$(MBC_TESTS_DIR)\$(BIN_DIR)"
	if exist "$(MBCDLL_DIR)\$(BIN_DIR)" rmdir /s /q "$(MBCDLL_DIR)\$(BIN_DIR)"
	if exist "$(MBCDLL_TESTS_DIR)\$(BIN_DIR)" rmdir /s /q "$(MBCDLL_TESTS_DIR)\$(BIN_DIR)"

clean_obj:
	if exist "$(MBC_DIR)\$(OBJ_DIR)" rmdir /s /q "$(MBC_DIR)\$(OBJ_DIR)"
	if exist "$(MBC_TESTS_DIR)\$(OBJ_DIR)" rmdir /s /q "$(MBC_TESTS_DIR)\$(OBJ_DIR)"
	if exist "$(MBCDLL_DIR)\$(OBJ_DIR)" rmdir /s /q "$(MBCDLL_DIR)\$(OBJ_DIR)"
	if exist "$(MBCDLL_TESTS_DIR)\$(OBJ_DIR)" rmdir /s /q "$(MBCDLL_TESTS_DIR)\$(OBJ_DIR)"

mbc_static_lib:
	cd $(MBC_DIR)
	if not exist $(LIB_DIR) mkdir $(LIB_DIR)
	nmake /nologo /f $(MKFILE) RUNTIME=MT  >>$(LIB_DIR)\libmbcmt.nmake.log
	nmake /nologo /f $(MKFILE) RUNTIME=MD  >>$(LIB_DIR)\libmbcmd.nmake.log
	nmake /nologo /f $(MKFILE) RUNTIME=MTd >>$(LIB_DIR)\libmbcmtd.nmake.log
	nmake /nologo /f $(MKFILE) RUNTIME=MDd >>$(LIB_DIR)\libmbcmdd.nmake.log
	cd ..

mbc_static_lib_tests: mbc_static_lib
	cd $(MBC_TESTS_DIR)
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	nmake /nologo /f $(MKFILE) RUNTIME=MT  >>$(BIN_DIR)\mbc_tests_mt.nmake.log
	nmake /nologo /f $(MKFILE) RUNTIME=MD  >>$(BIN_DIR)\mbc_tests_md.nmake.log
	nmake /nologo /f $(MKFILE) RUNTIME=MTd >>$(BIN_DIR)\mbc_tests_mtd.nmake.log
	nmake /nologo /f $(MKFILE) RUNTIME=MDd >>$(BIN_DIR)\mbc_tests_mdd.nmake.log
	cd ..
	rem to run tests manually:
	rem .\$(MBC_TESTS_DIR)\$(BIN_DIR)\mbc_tests_mt.exe
	rem .\$(MBC_TESTS_DIR)\$(BIN_DIR)\mbc_tests_md.exe
	rem .\$(MBC_TESTS_DIR)\$(BIN_DIR)\mbc_tests_mtd.exe
	rem .\$(MBC_TESTS_DIR)\$(BIN_DIR)\mbc_tests_mdd.exe

mbc_dynamic_lib: mbc_static_lib
	cd $(MBCDLL_DIR)
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	nmake /nologo /f $(MKFILE) RUNTIME=MD  >>$(BIN_DIR)\mbc.nmake.log
	nmake /nologo /f $(MKFILE) RUNTIME=MDd >>$(BIN_DIR)\mbcd.nmake.log
	cd ..

mbc_dynamic_lib_tests: mbc_dynamic_lib
	cd $(MBCDLL_TESTS_DIR)
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	nmake /nologo /f $(MKFILE) RUNTIME=MD  >>$(BIN_DIR)\mbcdll_tests_md.nmake.log
	nmake /nologo /f $(MKFILE) RUNTIME=MDd >>$(BIN_DIR)\mbcdll_tests_mdd.nmake.log
	cd ..
	rem to run tests manually:
	rem .\$(MBCDLL_TESTS_DIR)\$(BIN_DIR)\mbcdll_tests_md.exe
	rem .\$(MBCDLL_TESTS_DIR)\$(BIN_DIR)\mbcdll_tests_mdd.exe

mbcs_lib: mbc_dynamic_lib
	cd $(MBCS_DIR)
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	nmake /nologo /f $(MKFILE) RUNTIME=MD  >>$(BIN_DIR)\mbcs.nmake.log
	nmake /nologo /f $(MKFILE) RUNTIME=MDd >>$(BIN_DIR)\mbcsd.nmake.log
	cd ..

mbcs_lib_tests: mbcs_lib
	cd $(MBCS_TESTS_DIR)
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	nmake /nologo /f $(MKFILE) RUNTIME=MD  >>$(BIN_DIR)\mbcs_tests.make.log
	nmake /nologo /f $(MKFILE) RUNTIME=MDd >>$(BIN_DIR)\mbcs_tests_d.make.log
	cd ..
	rem to run tests manually:
	rem dotnet vstest $(MBCS_TESTS_DIR)\$(BIN_DIR)\Release_md\MbcsTests.dll
	rem dotnet vstest $(MBCS_TESTS_DIR)\$(BIN_DIR)\Debug_md\MbcsTests.dll

mbc_h5ls: mbc_static_lib
	cd $(MBCH5_LS_DIR)
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	nmake /nologo /f $(MKFILE) RUNTIME=MT  >>$(BIN_DIR)\mbch5ls_mt.nmake.log
	nmake /nologo /f $(MKFILE) RUNTIME=MD  >>$(BIN_DIR)\mbch5ls_md.nmake.log
	nmake /nologo /f $(MKFILE) RUNTIME=MTd >>$(BIN_DIR)\mbch5ls_mtd.nmake.log
	nmake /nologo /f $(MKFILE) RUNTIME=MDd >>$(BIN_DIR)\mbch5ls_mdd.nmake.log
	cd ../..
	rem to run manually:
	rem .\$(MBCH5_LS_DIR)\$(BIN_DIR)\mbch5ls_mt.exe --help
	rem .\$(MBCH5_LS_DIR)\$(BIN_DIR)\mbch5ls_md.exe --help
	rem .\$(MBCH5_LS_DIR)\$(BIN_DIR)\mbch5ls_mtd.exe --help
	rem .\$(MBCH5_LS_DIR)\$(BIN_DIR)\mbch5ls_mdd.exe --help

mbc_h5exp: mbc_static_lib
	cd $(MBCH5_EXP_DIR)
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	nmake /nologo /f $(MKFILE) RUNTIME=MT  >>$(BIN_DIR)\mbch5exp_mt.nmake.log
	nmake /nologo /f $(MKFILE) RUNTIME=MD  >>$(BIN_DIR)\mbch5exp_md.nmake.log
	nmake /nologo /f $(MKFILE) RUNTIME=MTd >>$(BIN_DIR)\mbch5exp_mtd.nmake.log
	nmake /nologo /f $(MKFILE) RUNTIME=MDd >>$(BIN_DIR)\mbch5exp_mdd.nmake.log
	cd ../..
	rem to run manually:
	rem .\$(MBCH5_EXP_DIR)\$(BIN_DIR)\mbch5exp_mt.exe --help
	rem .\$(MBCH5_EXP_DIR)\$(BIN_DIR)\mbch5exp_md.exe --help
	rem .\$(MBCH5_EXP_DIR)\$(BIN_DIR)\mbch5exp_mtd.exe --help
	rem .\$(MBCH5_EXP_DIR)\$(BIN_DIR)\mbch5exp_mdd.exe --help
	rem .\$(MBCH5_EXP_DIR)\$(BIN_DIR)\mbch5exp_mt.exe .\$(MBCH5_EXP_DIR)\tests\null.h5:/XPAR_null_FI0009000681/t____ -dt ymd/hms.f
	rem .\$(MBCH5_EXP_DIR)\$(BIN_DIR)\mbch5exp_mt.exe .\$(MBCH5_EXP_DIR)\tests\null.h5:/XPAR_null_FI0009000681/j_d01 -dt ymd/
	rem .\$(MBCH5_EXP_DIR)\$(BIN_DIR)\mbch5exp_mt.exe .\$(MBCH5_EXP_DIR)\tests\ebf.h5:/ebf/EURIBOR1M/s_d01 -dt ymd-
	rem .\$(MBCH5_EXP_DIR)\$(BIN_DIR)\mbch5exp_mt.exe .\$(MBCH5_EXP_DIR)\tests\foo.h5
	rem .\$(MBCH5_EXP_DIR)\$(BIN_DIR)\mbch5exp_mt.exe .\$(MBCH5_EXP_DIR)\tests\nonexistent.h5

mbcs_h5ls: mbcs_lib
	cd $(MBCSH5_LS_DIR)
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	nmake /nologo /f $(MKFILE) RUNTIME=MD  >>$(BIN_DIR)\mbcsh5ls.nmake.log
	nmake /nologo /f $(MKFILE) RUNTIME=MDd >>$(BIN_DIR)\mbcsh5ls_d.nmake.log
	cd ../..
	rem to run manually:
	rem .\$(MBCSH5_LS_DIR)\$(BIN_DIR)\Release_md\Mbcsh5ls.exe -h
	rem .\$(MBCSH5_LS_DIR)\$(BIN_DIR)\Debug_md\Mbcsh5ls.exe -h

# -----------------------------------------------------------------------------------------------------
SevenZip=..\..\dependencies_prebuilt\msvc\7z -tzip -mx9
DEPS_LIBTYPE=.\dependencies_prebuilt\msvc
COLLECTDIR=.\last_prebuilt\win10-x64
COLLECTDIR_CPP=$(COLLECTDIR)\c++
COLLECTDIR_CAPI=$(COLLECTDIR)\capi
COLLECTDIR_CS=$(COLLECTDIR)\cs
COLLECTDIR_H5=$(COLLECTDIR)\h5

collect_last_prebuilt:
	rem c++ ----------------------------
	if not exist $(COLLECTDIR_CPP)\include\hdf5 mkdir $(COLLECTDIR_CPP)\include\hdf5
	xcopy /Q /R /Y /E $(DEPS_LIBTYPE)\include_hdf5\*.h $(COLLECTDIR_CPP)\include\hdf5
	if not exist $(COLLECTDIR_CPP)\include\zlib mkdir $(COLLECTDIR_CPP)\include\zlib
	xcopy /Q /R /Y /E $(DEPS_LIBTYPE)\include_zlib\*.h $(COLLECTDIR_CPP)\include\zlib
	if not exist $(COLLECTDIR_CPP)\include\szip mkdir $(COLLECTDIR_CPP)\include\szip
	xcopy /Q /R /Y /E $(DEPS_LIBTYPE)\include_szip\*.h $(COLLECTDIR_CPP)\include\szip
	if not exist $(COLLECTDIR_CPP)\include\mbc mkdir $(COLLECTDIR_CPP)\include\mbc
	xcopy /Q /R /Y /E mbc\include\mbc $(COLLECTDIR_CPP)\include\mbc
	if not exist $(COLLECTDIR_CPP)\lib mkdir $(COLLECTDIR_CPP)\lib
	xcopy /Q /R /Y /E mbc\lib_msvc\*.lib $(COLLECTDIR_CPP)\lib
	xcopy /Q /R /Y /E $(DEPS_LIBTYPE)\lib\*.lib $(COLLECTDIR_CPP)\lib
	if not exist $(COLLECTDIR_CPP)\tests mkdir $(COLLECTDIR_CPP)\tests
	xcopy /Q /R /Y /E mbc_tests\bin_msvc\*.exe $(COLLECTDIR_CPP)\tests
	cd $(COLLECTDIR)
	$(SevenZip) a c++_$(DATETIME).zip c++
	rmdir /S /Q c++
	cd ..\..
	rem c api ----------------------------
	if not exist $(COLLECTDIR_CAPI)\include mkdir $(COLLECTDIR_CAPI)\include
	xcopy /Q /R /Y /E mbcdll\include\*.h $(COLLECTDIR_CAPI)\include
	xcopy /Q /R /Y /E mbcdll\bin_msvc\*.dll $(COLLECTDIR_CAPI)
	xcopy /Q /R /Y /E mbcdll\bin_msvc\*.lib $(COLLECTDIR_CAPI)
	xcopy /Q /R /Y /E mbcdll\bin_msvc\*.exp $(COLLECTDIR_CAPI)
	xcopy /Q /R /Y /E mbcdll\bin_msvc\*.pdb $(COLLECTDIR_CAPI)
	if not exist $(COLLECTDIR_CAPI)\tests mkdir $(COLLECTDIR_CAPI)\tests
	xcopy /Q /R /Y /E mbcdll_tests\bin_msvc\*.exe $(COLLECTDIR_CAPI)\tests
	xcopy /Q /R /Y /E mbcdll_tests\bin_msvc\*.dll $(COLLECTDIR_CAPI)\tests
	if not exist $(COLLECTDIR_CAPI)\crt mkdir $(COLLECTDIR_CAPI)\crt
	xcopy /Q /R /Y /E $(DEPS_LIBTYPE)\crt\*.* $(COLLECTDIR_CAPI)\crt
	cd $(COLLECTDIR)
	$(SevenZip) a capi_$(DATETIME).zip capi
	rmdir /S /Q capi
	cd ..\..
	rem cs ----------------------------
	if not exist $(COLLECTDIR_CS)\release mkdir $(COLLECTDIR_CS)\release
	xcopy /Q /R /Y /E mbcs\bin_msvc\Release_md\*.* $(COLLECTDIR_CS)\release
	if not exist $(COLLECTDIR_CS)\debug mkdir $(COLLECTDIR_CS)\debug
	xcopy /Q /R /Y /E mbcs\bin_msvc\Debug_md\*.* $(COLLECTDIR_CS)\debug
	if not exist $(COLLECTDIR_CS)\tests\release mkdir $(COLLECTDIR_CS)\tests\release
	xcopy /Q /R /Y /E mbcs_tests\bin_msvc\Release_md\*.* $(COLLECTDIR_CS)\tests\release
	echo dotnet vstest MbcsTests.dll >$(COLLECTDIR_CS)\tests\release\MbcsTests.bat
	if not exist $(COLLECTDIR_CS)\tests\debug mkdir $(COLLECTDIR_CS)\tests\debug
	xcopy /Q /R /Y /E mbcs_tests\bin_msvc\Debug_md\*.* $(COLLECTDIR_CS)\tests\debug
	echo dotnet vstest MbcsTests.dll >$(COLLECTDIR_CS)\tests\debug\MbcsTests.bat
	cd $(COLLECTDIR)
	$(SevenZip) a cs_$(DATETIME).zip cs
	rmdir /S /Q cs
	cd ..\..
	rem h5 ----------------------------
	if not exist $(COLLECTDIR_H5)\hdf5 mkdir $(COLLECTDIR_H5)\hdf5
	copy /Y mbc_h5tools\mbch5ls\bin_msvc\mbch5ls_mt.exe $(COLLECTDIR_H5)\mbch5ls.exe
	copy /Y mbc_h5tools\mbch5exp\bin_msvc\mbch5exp_mt.exe $(COLLECTDIR_H5)\mbch5exp.exe
	xcopy /Q /R /Y /E $(DEPS_LIBTYPE)\tools_hdf5\*.* $(COLLECTDIR_H5)\hdf5
	cd $(COLLECTDIR)
	$(SevenZip) a h5_$(DATETIME).zip h5
	rmdir /S /Q h5
	cd ..\..
	rem nuget package ----------------------------
	cd mbcs
	nuget pack Mbcs.nuspec
	cd ..
	move /Y mbcs\*.nupkg last_prebuilt
