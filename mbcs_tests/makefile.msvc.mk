# externally defined macro: RUNTIME={MD, MDd}

BINDIR=bin_msvc
NAME=MbcsTests

# =====================================================================================================
!IF "$(RUNTIME)" == "MD"
# release definitions
BINDIR=$(BINDIR)\Release_md
CONFIG=Release_md
TEST=mbcs_tests
# =====================================================================================================
!ELSE IF "$(RUNTIME)" == "MDd"
# debug definitions
BINDIR=$(BINDIR)\Debug_md
CONFIG=Debug_md
TEST=mbcs_tests_d
# =====================================================================================================
!ELSE
!ERROR The RUNTIME macro is undefined. Should be one of {MD, MDd}.
!ENDIF
# =====================================================================================================

DLLNAME=$(BINDIR)\$(NAME)
DLL=$(DLLNAME).dll

# -----------------------------------------------------------------------------------------------------
all: create_dirs $(DLL)

clean:
	if exist $(DLL) del /f /s /q $(DLL)
	if exist $(BINDIR)\..\$(TEST).nmake.log del /f /s /q $(BINDIR)\..\$(TEST).nmake.log
	if exist $(BINDIR)\..\$(TEST).nmake.err del /f /s /q $(BINDIR)\..\$(TEST).nmake.err
	if exist $(BINDIR)\..\$(TEST).testresults.log del /f /s /q $(BINDIR)\..\$(TEST).testresults.log
	if exist $(BINDIR)\..\$(TEST).testresults.err del /f /s /q $(BINDIR)\..\$(TEST).testresults.err

create_dirs:
	if not exist $(BINDIR) mkdir $(BINDIR)

H5SRCLIST=H5\H5Tests.cs H5\H5Tests.Data.cs H5\H5Tests.Enum.cs H5\H5Tests.File.cs \
H5\H5Tests.File.Exploratory.cs H5\H5Tests.Instrument.cs H5\H5Tests.TemporalEntity.cs \
H5\H5Tests.OhlcvData.cs H5\H5Tests.OhlcvData.Exploratory.cs \
H5\H5Tests.OhlcvPriceOnlyData.cs H5\H5Tests.OhlcvPriceOnlyData.Exploratory.cs \
H5\H5Tests.QuoteData.cs H5\H5Tests.QuoteData.Exploratory.cs \
H5\H5Tests.QuotePriceOnlyData.cs H5\H5Tests.QuotePriceOnlyData.Exploratory.cs \
H5\H5Tests.ScalarData.cs H5\H5Tests.ScalarData.Exploratory.cs \
H5\H5Tests.TradeData.cs H5\H5Tests.TradeData.Exploratory.cs \
H5\H5Tests.TradePriceOnlyData.cs H5\H5Tests.TradePriceOnlyData.Exploratory.cs

SRCLIST=$(H5SRCLIST)

$(DLL): $(SRCLIST)
	dotnet build --configuration $(CONFIG) --runtime win10-x64 --output $(BINDIR) -p:NoDependencies=true --verbosity normal -detailedSummary -ignoreProjectExtensions:.sln MbcsTests.csproj
	dotnet vstest $(DLL) >$(BINDIR)\..\$(TEST).testresults.log 2>$(BINDIR)\..\$(TEST).testresults.err
	rem dotnet test --configuration $(CONFIG) --runtime win10-x64 -p:NoDependencies=true MbcsTests.csproj

# -----------------------------------------------------------------------------------------------------
