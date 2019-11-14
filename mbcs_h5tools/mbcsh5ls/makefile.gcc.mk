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

NAME=Mbcsh5ls
BIN_LIBTYPE=bin_gcc_$(LIBTYPE)

# =====================================================================================================
ifeq ($(BUILD),release)
	CONFIG=Release_md
else
ifeq ($(BUILD),debug)
	CONFIG=Debug_md
else
$(error The BUILD macro is undefined. Should be one of {release, debug})
endif
endif

# =====================================================================================================
BINDIR=$(BIN_LIBTYPE)/$(CONFIG)
BINNAME=$(BINDIR)/$(NAME)
BIN=$(BINNAME)

# -----------------------------------------------------------------------------------------------------
.PHONY: all

all: create_dirs $(BIN)

clean:
	rm -rf ./$(BINDIR)
	rm -f ./$(BIN_LIBTYPE)/$(NAME).make.log
	rm -f ./$(BIN_LIBTYPE)/$(NAME).make.err

create_dirs:
	mkdir -p ./$(BINDIR)

SRCLIST=Mbcsh5ls.cs

$(BIN): $(SRCLIST)
	# dotnet publish --configuration $(CONFIG) --runtime $(RUNTIME) --output $(BINDIR) -p:NoDependencies=true -p:Compiler=gcc -p:GccLibType=$(LIBTYPE) --verbosity normal -detailedSummary -ignoreProjectExtensions:.sln Mbcsh5ls.csproj
	dotnet build --configuration $(CONFIG) --runtime $(RUNTIME) --output $(BINDIR) -p:NoDependencies=true -p:Compiler=gcc -p:GccLibType=$(LIBTYPE) --verbosity normal -detailedSummary -ignoreProjectExtensions:.sln Mbcsh5ls.csproj

# -----------------------------------------------------------------------------------------------------
