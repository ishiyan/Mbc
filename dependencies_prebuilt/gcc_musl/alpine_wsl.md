# Alpine Linux in WSL

- Download `Alpine.zip` from `https://github.com/yuk7/AlpineWSL`
- Open a shell, unzip `Alpine.zip` in the `c:\Alpine` folder
- Run `Alpine.exe`, Alpine will be registered with WSL
- Run `Alpine.exe` once more, you will be logged in as a root
- Run `Alpine.exe clean` to uninstall
- Run `Alpine.exe help` to print usage

Useful links:
https://wiki.alpinelinux.org/wiki/Category_talk:Developer_Documentation


## Install components

- Midnight commander `apk mc` (useful to edit the `/etc/apk/repositories` file below)

- Change your repository to use "edge": insert as first line into `/etc/apk/repositories`: `http://dl-4.alpinelinux.org/alpine/edge/main/`
- Optionally add `http://dl-4.alpinelinux.org/alpine/edge/testing/`

- Update and upgrade

```bash
apk update
apk upgrade
```

- Build toolchain: `apk add build-base curl zip unzip tar sudo make cmake gdb linux-headers`

## Install DotNet Core SDK

Based on `https://github.com/dotnet/dotnet-docker/blob/master/3.0/sdk/alpine3.10/amd64/Dockerfile`
Create and execute (`ash dotnetcore-sdk-3.0.00-alpine-3.10.sh`) the following `dotnetcore-sdk-3.0.00-alpine-3.10.sh` shell script

```bash
#!bin/ash

apk add --no-cache icu-libs

# You may add these to /etc/profile
export DOTNET_SYSTEM_GLOBALIZATION_INVARIANT=false
export LC_ALL=en_US.UTF-8
export LANG=en_US.UTF-8

DOTNET_SDK_VERSION=3.0.100
wget -O dotnet.tar.gz https://dotnetcli.blob.core.windows.net/dotnet/Sdk/$DOTNET_SDK_VERSION/dotnet-sdk-$DOTNET_SDK_VERSION-linux-musl-x64.tar.gz
dotnet_sha512='b2b42c7e33bdb492c7f25c615cfc57c9ca3222d4492d59829f2d683cb40a3d18d648195d21f4e519fd187f40d69e2977ccc3d993c5aefc5cb0a23cd496f344dc'
echo "$dotnet_sha512  dotnet.tar.gz" | sha512sum -c -
mkdir -p /usr/share/dotnet
tar -C /usr/share/dotnet -xzf dotnet.tar.gz
ln -s /usr/share/dotnet/dotnet /usr/bin/dotnet
rm dotnet.tar.gz

# Unset the value from the base image
export ASPNETCORE_URLS=
# Enable correct mode for dotnet watch
export DOTNET_USE_POLLING_FILE_WATCHER=true
# Skip extraction of XML docs - helps performance
export NUGET_XMLDOC_MODE=skip

# Trigger first run experience by running arbitrary cmd
dotnet --info

# Install PowerShell global tool
export POWERSHELL_VERSION=7.0.0-preview.4
export POWERSHELL_DISTRIBUTION_CHANNEL=PSDocker-DotnetCoreSDK-Alpine-3.10

wget -O PowerShell.Linux.Alpine.$POWERSHELL_VERSION.nupkg https://pwshtool.blob.core.windows.net/tool/$POWERSHELL_VERSION/PowerShell.Linux.Alpine.$POWERSHELL_VERSION.nupkg
powershell_sha512='25dc93d87e4a0ae94fd161ff3b2d26b0e0eb5ae69a913500f7816debd705d1b84fcebb147b8a02d5fc53cf2718ec3d65dc0f2b73e0e4b917599b6562cf140c39'
echo "$powershell_sha512  PowerShell.Linux.Alpine.$POWERSHELL_VERSION.nupkg" | sha512sum -c -
mkdir -p /usr/share/powershell
dotnet tool install --add-source / --tool-path /usr/share/powershell --version $POWERSHELL_VERSION PowerShell.Linux.Alpine
rm PowerShell.Linux.Alpine.$POWERSHELL_VERSION.nupkg
ln -s /usr/share/powershell/pwsh /usr/bin/pwsh
# To reduce image size, remove the copy nupkg that nuget keeps.
find /usr/share/powershell -print | grep -i '.*[.]nupkg$' | xargs rm
# Add ncurses-terminfo-base to resolve psreadline dependency
apk add --no-cache ncurses-terminfo-base

# You may add this to /etc/profile
export NET_CLI_TELEMETRY_OPTOUT=1
```

## Install NodeJS

Execute `apk add nodejs npm`

