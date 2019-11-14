# WSL (Ubuntu)

## Location

The Ubuntu FS lives on your Windows FS on the path
`C:\Users\ishiyan\AppData\Local\Packages\CanonicalGroupLimited.Ubuntu18.04onWindows_79rhkp1fndgsc\LocalState\rootfs`.
The root is `C:\Users\ishiyan\AppData\Local\Packages\CanonicalGroupLimited.Ubuntu18.04onWindows_79rhkp1fndgsc`.

`cd /mnt/c/mBrane/Hdf5`

## Update software

```bash
# download the package lists from the repositories
sudo apt update

# fetch new versions of packages, press Y when prompted
sudo apt upgrade

# this will remove any packages that are no longer needed
sudo apt autoremove
```

## Install GNU compilers

Install multiple `gcc/g++`. The latest version of gcc which is 9.x.x is available from the Ubuntu Toolchain PPA.

```bash
sudo apt install software-properties-common
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt install gcc-7 g++-7 gcc-8 g++-8 gcc-9 g++-9
sudo apt install gdb make cmake

gcc --version
g++ --version
make --version
cmake --version
```

The commands below will configure alternative for each version and associate a priority with it.
The default version is the one with the highest priority, in our case that is gcc-9.

```bash
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90 --slave /usr/bin/g++ g++ /usr/bin/g++-9 --slave /usr/bin/gcov gcov /usr/bin/gcov-9
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 80 --slave /usr/bin/g++ g++ /usr/bin/g++-8 --slave /usr/bin/gcov gcov /usr/bin/gcov-8
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 70 --slave /usr/bin/g++ g++ /usr/bin/g++-7 --slave /usr/bin/gcov gcov /usr/bin/gcov-7
```

Later if you want to change the default version use the `update-alternatives` command.

```bash
sudo update-alternatives --config gcc

There are 3 choices for the alternative gcc (providing /usr/bin/gcc).

  Selection    Path            Priority   Status
------------------------------------------------------------
* 0            /usr/bin/gcc-9   90        auto mode
  1            /usr/bin/gcc-7   70        manual mode
  2            /usr/bin/gcc-8   80        manual mode
  3            /usr/bin/gcc-9   90        manual mode

Press <enter> to keep the current choice[*], or type selection number:
```

## Archiving

Archive prebuilt hdf5 folder:

```bash
cd $wrmb/mbc/mbc
tar -zcf ./hdf5_prebuilt_msvc.tar.gz ./hdf5_prebuilt_msvc
tar -jcf ./hdf5_prebuilt_msvc.tar.bz2 ./hdf5_prebuilt_msvc
```

Archive prebuilt hdf5 folder with date:

```bash
cd $wrmb/mbc/mbc
tar -zcf ./hdf5_prebuilt_msvc19.22_hdf5-1.10.5_szip-2.1.1_zlib-1.2.11_$(date '+%Y-%m-%d').tar.gz ./hdf5_prebuilt_msvc
tar -jcf ./hdf5_prebuilt_msvc19.22_hdf5-1.10.5_szip-2.1.1_zlib-1.2.11_$(date '+%Y-%m-%d').tar.bz2 ./hdf5_prebuilt_msvc
tar --xz -cf ./hdf5_prebuilt_msvc19.22_hdf5-1.10.5_szip-2.1.1_zlib-1.2.11_$(date '+%Y-%m-%d').tar.xz ./hdf5_prebuilt_msvc
```

Or, even better,

```bash
GZIP=-9 tar czf ./hdf5_prebuilt_msvc19.22_hdf5-1.10.5_szip-2.1.1_zlib-1.2.11_$(date '+%Y-%m-%d').tar.gz ./hdf5_prebuilt_msvc
BZIP2=-9 tar cjf ./hdf5_prebuilt_msvc19.22_hdf5-1.10.5_szip-2.1.1_zlib-1.2.11_$(date '+%Y-%m-%d').tar.bz2 ./hdf5_prebuilt_msvc
XZ_OPT=-9e tar cJf ./hdf5_prebuilt_msvc19.22_hdf5-1.10.5_szip-2.1.1_zlib-1.2.11_$(date '+%Y-%m-%d').tar.xz ./hdf5_prebuilt_msvc
```

## Install .Net Core on WSL

Taken from `https://dotnet.microsoft.com/learn/dotnet/hello-world-tutorial/intro#linuxubuntu`.

### Register Microsoft key and feed

Before installing .NET, you'll need to register the Microsoft key, register the product repository, and install required dependencies. This only needs to be done once per machine.
Open a terminal and run the following commands:

```bash
wget -q https://packages.microsoft.com/config/ubuntu/18.04/packages-microsoft-prod.deb -O packages-microsoft-prod.deb
sudo dpkg -i packages-microsoft-prod.deb
```

### Install the .NET SDK

Update the products available for installation, then install the .NET SDK.
In your terminal, run the following commands:

```bash
sudo add-apt-repository universe
sudo apt-get install apt-transport-https
sudo apt-get update
sudo apt-get install dotnet-sdk-2.2
```

If you receive an error message similar to Unable to locate package dotnet-sdk-2.2, run the following commands.

```bash
sudo dpkg --purge packages-microsoft-prod && sudo dpkg -i packages-microsoft-prod.deb
sudo apt-get update
sudo apt-get install dotnet-sdk-2.2
```

If that doesn't work, you can run a manual install with the following commands.

```bash
sudo apt-get install -y gpg
wget -qO- https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > microsoft.asc.gpg
sudo mv microsoft.asc.gpg /etc/apt/trusted.gpg.d/
wget -q https://packages.microsoft.com/config/ubuntu/18.04/prod.list
sudo mv prod.list /etc/apt/sources.list.d/microsoft-prod.list
sudo chown root:root /etc/apt/trusted.gpg.d/microsoft.asc.gpg
sudo chown root:root /etc/apt/sources.list.d/microsoft-prod.list
sudo apt-get install -y apt-transport-https
sudo apt-get update
sudo apt-get install dotnet-sdk-2.2
```

### Check everything installed correctly

Once you've installed, open a **new** terminal and run the following command: `dotnet --info`
