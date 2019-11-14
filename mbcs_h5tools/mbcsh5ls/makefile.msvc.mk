# externally defined macro: RUNTIME={MD, MDd}

BINDIR=bin_msvc
NAME=Mbcsh5ls

# =====================================================================================================
!IF "$(RUNTIME)" == "MD"
# release definitions
BINDIR=$(BINDIR)\Release_md
CONFIG=Release_md
# =====================================================================================================
!ELSE IF "$(RUNTIME)" == "MDd"
# debug definitions
BINDIR=$(BINDIR)\Debug_md
CONFIG=Debug_md
# =====================================================================================================
!ELSE
!ERROR The RUNTIME macro is undefined. Should be one of {MD, MDd}.
!ENDIF
# =====================================================================================================

BINNAME=$(BINDIR)\$(NAME)
BIN=$(BINNAME).exe

# -----------------------------------------------------------------------------------------------------
all: create_dirs $(BIN)

clean:
	if exist $(BIN) del /f /s /q $(BIN)

create_dirs:
	if not exist $(BINDIR) mkdir $(BINDIR)

SRCLIST=Mbcsh5ls.cs

$(BIN): $(SRCLIST)
	dotnet publish --configuration $(CONFIG) --runtime win10-x64 --output $(BINDIR) -p:NoDependencies=true --verbosity normal -detailedSummary -ignoreProjectExtensions:.sln Mbcsh5ls.csproj

# -----------------------------------------------------------------------------------------------------
