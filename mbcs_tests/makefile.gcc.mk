# externally defined macro: BUILD={debug, release} LIBTYPE={glibc, musl}
ifeq ($(LIBTYPE),glibc)
	# linux-x64 ubuntu.18.04-x64
	RUNTIME=linux-x64
else
ifeq ($(LIBTYPE),musl)
	RUNTIME=linux-musl-x64
else
$(error The LIBTYPE macro is undefined. Should be one of {glibc, musl})
endif
endif

NAME=MbcsTests
BIN_LIBTYPE=bin_gcc_$(LIBTYPE)

# =====================================================================================================
ifeq ($(BUILD),release)
	CONFIG=Release_md
	LIBMBCSO=mbc.so
	LIBMBCSO_VERSIONED=libmbc.so.1.0.1
	LIBMBCDLL=mbc.dll
	LIBMBCPDB=mbc.pdb
	TEST=mbcs_tests
else
ifeq ($(BUILD),debug)
	CONFIG=Debug_md
	LIBMBCSO=mbcd.so
	LIBMBCSO_VERSIONED=libmbcd.so.1.0.1
	LIBMBCDLL=mbcd.dll
	LIBMBCPDB=mbcd.pdb
	TEST=mbcs_tests_d
else
$(error The BUILD macro is undefined. Should be one of {release, debug})
endif
endif

# =====================================================================================================
BINDIR=$(BIN_LIBTYPE)/$(CONFIG)
DLLNAME=$(BINDIR)/$(NAME)
DLL=$(DLLNAME).dll

# -----------------------------------------------------------------------------------------------------
.PHONY: all

all: create_dirs $(DLL)

clean:
	rm -f ./$(BINDIR)
	rm -f ./$(BIN_LIBTYPE)/$(TEST).testresults.log
	rm -f ./$(BIN_LIBTYPE)/$(TEST).testresults.err
	rm -f ./$(BIN_LIBTYPE)/$(TEST).make.log
	rm -f ./$(BIN_LIBTYPE)/$(TEST).make.err

create_dirs:
	mkdir -p ./$(BINDIR)

H5SRCLIST=H5/H5Tests.cs H5/H5Tests.Data.cs H5/H5Tests.Enum.cs H5/H5Tests.File.cs \
H5/H5Tests.File.Exploratory.cs H5/H5Tests.Instrument.cs H5/H5Tests.TemporalEntity.cs \
H5/H5Tests.OhlcvData.cs H5/H5Tests.OhlcvData.Exploratory.cs \
H5/H5Tests.OhlcvPriceOnlyData.cs H5/H5Tests.OhlcvPriceOnlyData.Exploratory.cs \
H5/H5Tests.QuoteData.cs H5/H5Tests.QuoteData.Exploratory.cs \
H5/H5Tests.QuotePriceOnlyData.cs H5/H5Tests.QuotePriceOnlyData.Exploratory.cs \
H5/H5Tests.ScalarData.cs H5/H5Tests.ScalarData.Exploratory.cs \
H5/H5Tests.TradeData.cs H5/H5Tests.TradeData.Exploratory.cs \
H5/H5Tests.TradePriceOnlyData.cs H5/H5Tests.TradePriceOnlyData.Exploratory.cs

SRCLIST=$(H5SRCLIST)

$(DLL): $(SRCLIST)
	dotnet build --configuration $(CONFIG) --runtime $(RUNTIME) --output $(BINDIR) -p:NoDependencies=true -p:Compiler=gcc -p:GccLibType=$(LIBTYPE) --verbosity normal -detailedSummary -ignoreProjectExtensions:.sln MbcsTests.csproj
	# if [ -e "../mbcdll/$(BIN_LIBTYPE)/$(LIBMBCSO_VERSIONED)" ]; then cp -f ../mbcdll/$(BIN_LIBTYPE)/$(LIBMBCSO_VERSIONED) $(BINDIR)/$(LIBMBCSO); fi;
	# if [ -e "../mbcdll/bin_msvc/$(LIBMBCDLL)" ]; then cp -f ../mbcdll/bin_msvc/$(LIBMBCDLL) $(BINDIR)/$(LIBMBCDLL); fi;
	# if [ -e "../mbcdll/bin_msvc/$(LIBMBCPDB)" ]; then cp -f ../mbcdll/bin_msvc/$(LIBMBCPDB) $(BINDIR)/$(LIBMBCPDB); fi;
	-dotnet vstest $(DLL) >$(BIN_LIBTYPE)/$(TEST).testresults.log 2>$(BIN_LIBTYPE)/$(TEST).testresults.err
	# dotnet test --configuration $(CONFIG) --runtime $(RUNTIME) -p:NoDependencies=true ./MbcsTests.csproj

# -----------------------------------------------------------------------------------------------------
