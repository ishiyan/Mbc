# externally defined macro: RUNTIME={MD, MDd}

BINDIR=bin_msvc
DLLNAME=mbcs

# =====================================================================================================
!IF "$(RUNTIME)" == "MD"
# release definitions
BINDIR=$(BINDIR)\Release_md
CONFIG=Release_md
LIBMBCDLL=mbc.dll
LIBMBCPDB=mbc.pdb
# =====================================================================================================
!ELSE IF "$(RUNTIME)" == "MDd"
# debug definitions
BINDIR=$(BINDIR)\Debug_md
CONFIG=Debug_md
LIBMBCDLL=mbcd.dll
LIBMBCPDB=mbcd.pdb
# =====================================================================================================
!ELSE
!ERROR The RUNTIME macro is undefined. Should be one of {MD, MDd}.
!ENDIF
# =====================================================================================================

DLLNAME=$(BINDIR)\$(DLLNAME)
DLL=$(DLLNAME).dll

# -----------------------------------------------------------------------------------------------------
all: create_dirs $(DLL)

clean:
	if exist $(DLL) del /f /s /q $(DLL)

create_dirs:
	if not exist $(BINDIR) mkdir $(BINDIR)

NATIVEMETHODSSRCLIST=NativeMethods\NativeMethods.CrtDebug.cs NativeMethods\NativeMethods.cs NativeMethods\NativeMethods.H5.Data.cs \
NativeMethods\NativeMethods.H5.File.cs NativeMethods\NativeMethods.H5.Instrument.cs NativeMethods\NativeMethods.H5.OhlcvData.cs \
NativeMethods\NativeMethods.H5.OhlcvPriceOnlyData.cs NativeMethods\NativeMethods.H5.QuoteData.cs NativeMethods\NativeMethods.H5.QuotePriceOnlyData.cs \
NativeMethods\NativeMethods.H5.ScalarData.cs NativeMethods\NativeMethods.H5.TradeData.cs NativeMethods\NativeMethods.H5.TradePriceOnlyData.cs

H5SRCLIST=H5\Callbacks.cs H5\Data.cs H5\DataInfo.cs H5\DataKind.cs H5\DataTimeFrame.cs H5\DataType.cs \
H5\DuplicateTimeTicks.cs H5\File.cs H5\FileDriver.cs H5\GroupInfo.cs H5\Instrument.cs H5\ObjectInfo.cs \
H5\Ohlcv.cs H5\OhlcvData.cs H5\OhlcvKind.cs H5\OhlcvPriceOnly.cs H5\OhlcvPriceOnlyData.cs H5\Quote.cs \
H5\QuoteData.cs H5\QuoteKind.cs H5\QuotePriceOnly.cs H5\QuotePriceOnlyData.cs H5\Scalar.cs \
H5\ScalarData.cs H5\ScalarKind.cs H5\TemporalEntity.cs H5\Trade.cs H5\TradeData.cs H5\TradeKind.cs \
H5\TradePriceOnly.cs H5\TradePriceOnlyData.cs

SRCLIST=CrtDebug.cs GlobalSuppressions.cs InternalsVisible.cs $(NATIVEMETHODSSRCLIST) $(H5SRCLIST)

$(DLL): $(SRCLIST)
	dotnet build --configuration $(CONFIG) --runtime win10-x64 --output $(BINDIR) -p:NoDependencies=true --verbosity detailed -detailedSummary -ignoreProjectExtensions:.sln Mbcs.csproj
	if exist "..\mbcdll\bin_msvc\$(LIBMBCDLL)" copy /Y "..\mbcdll\bin_msvc\$(LIBMBCDLL)" "$(BINDIR)\$(LIBMBCDLL)"
	if exist "..\mbcdll\bin_msvc\$(LIBMBCPDB)" copy /Y "..\mbcdll\bin_msvc\$(LIBMBCPDB)" "$(BINDIR)\$(LIBMBCPDB)"

# -----------------------------------------------------------------------------------------------------
