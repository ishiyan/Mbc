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

NAME=Mbcs
BIN_LIBTYPE=bin_gcc_$(LIBTYPE)

# =====================================================================================================
ifeq ($(BUILD),release)
	CONFIG=Release_md
	LIBMBCSO=mbc.so
	LIBMBCSO_VERSIONED=libmbc.so.1.0.1
	LIBMBCDLL=mbc.dll
	LIBMBCPDB=mbc.pdb
else
ifeq ($(BUILD),debug)
	CONFIG=Debug_md
	LIBMBCSO=mbcd.so
	LIBMBCSO_VERSIONED=libmbcd.so.1.0.1
	LIBMBCDLL=mbcd.dll
	LIBMBCPDB=mbcd.pdb
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
	rm -rf ./$(BINDIR)

create_dirs:
	mkdir -p ./$(BINDIR)

H5SRCLIST=H5/Callbacks.cs H5/Data.cs H5/DataInfo.cs H5/DataKind.cs H5/DataTimeFrame.cs H5/DataType.cs \
H5/DuplicateTimeTicks.cs H5/File.cs H5/FileDriver.cs H5/GroupInfo.cs H5/Instrument.cs H5/ObjectInfo.cs \
H5/Ohlcv.cs H5/OhlcvData.cs H5/OhlcvKind.cs H5/OhlcvPriceOnly.cs H5/OhlcvPriceOnlyData.cs H5/Quote.cs \
H5/QuoteData.cs H5/QuoteKind.cs H5/QuotePriceOnly.cs H5/QuotePriceOnlyData.cs H5/Scalar.cs \
H5/ScalarData.cs H5/ScalarKind.cs H5/TemporalEntity.cs H5/Trade.cs H5/TradeData.cs H5/TradeKind.cs \
H5/TradePriceOnly.cs H5/TradePriceOnlyData.cs

NATIVEMETHODSSRCLIST=NativeMethods/NativeMethods.CrtDebug.cs NativeMethods/NativeMethods.cs NativeMethods/NativeMethods.H5.Data.cs \
NativeMethods/NativeMethods.H5.File.cs NativeMethods/NativeMethods.H5.Instrument.cs NativeMethods/NativeMethods.H5.OhlcvData.cs \
NativeMethods/NativeMethods.H5.OhlcvPriceOnlyData.cs NativeMethods/NativeMethods.H5.QuoteData.cs NativeMethods/NativeMethods.H5.QuotePriceOnlyData.cs \
NativeMethods/NativeMethods.H5.ScalarData.cs NativeMethods/NativeMethods.H5.TradeData.cs NativeMethods/NativeMethods.H5.TradePriceOnlyData.cs

SRCLIST=CrtDebug.cs GlobalSuppressions.cs InternalsVisible.cs $(NATIVEMETHODSSRCLIST) $(H5SRCLIST)

$(DLL): $(SRCLIST)
	dotnet build --configuration $(CONFIG) --runtime $(RUNTIME) --output $(BINDIR) -p:NoDependencies=true -p:Compiler=gcc -p:GccLibType=$(LIBTYPE) --verbosity detailed -detailedSummary -ignoreProjectExtensions:.sln Mbcs.csproj
	# if [ -e "../mbcdll/$($(BIN_LIBTYPE))/$(LIBMBCSO_VERSIONED)" ]; then cp -f ../mbcdll/$(BIN_LIBTYPE)/$(LIBMBCSO_VERSIONED) $(BINDIR)/$(LIBMBCSO); fi;
	# if [ -e "../mbcdll/bin_msvc/$(LIBMBCDLL)" ]; then cp -f ../mbcdll/bin_msvc/$(LIBMBCDLL) $(BINDIR)/$(LIBMBCDLL); fi;
	# if [ -e "../mbcdll/bin_msvc/$(LIBMBCPDB)" ]; then cp -f ../mbcdll/bin_msvc/$(LIBMBCPDB) $(BINDIR)/$(LIBMBCPDB); fi;

# -----------------------------------------------------------------------------------------------------
