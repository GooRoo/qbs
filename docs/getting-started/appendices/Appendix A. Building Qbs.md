---
title: 'Appendix A: Building Qbs'
---

# Appendix A: Building Qbs

Qbs can be [[Installing|installed from binary packages]] or built from
sources, as described in this appendix. In addition, this appendix describes
how to use Docker images for developing Qbs.

## Supported Platforms

Qbs can be installed and run on the following platforms:


- Windows 7, or later
- Linux (tested on Debian 8 and 9, Ubuntu 16.04, OpenSuSE 13.2, and
        Arch Linux)
- macOS 10.7, or later


## System Requirements

To build Qbs from the source, you need:


- Qt 5.15, or later
- Windows: MinGW with GCC 4.9 or Microsoft Visual Studio 2015,
        or later
- Linux: GCC 4.9, or later, or Clang 3.9.0, or later
- macOS: Xcode 6.2, or later


An installed toolchain has to match the one that Qt was compiled with.

### Documentation

Building the Qbs documentation requires Python 2.7 or 3.2 or above,
as well as some third party Python modules. These can be installed via `pip`:

```sh
pip install beautifulsoup4 lxml
```

Regenerating the man page requires the `help2man` tool.

## Building Qbs with СMake

To build Qbs, enter the following commands:

```sh
mkdir build && cd build
cmake -DQt5_DIR=${QT_DIR}/lib/cmake/Qt5/ ..
make
```

Where `${QT_DIR}` is the directory where Qt is installed. Passing the `Qt5_DIR` option
is not necessary if `qmake` is present in `PATH`.

Depending on your platform, you might use `mingw32-make`, `nmake`, or
`jom` instead of `make`.

Alternatively, you can use the
[Ninja](https://cmake.org/cmake/help/latest/generator/Ninja.html) generator:
```sh
cmake -GNinja -DQt5_DIR=${QT_DIR}/lib/cmake/Qt5/ ..
ninja
```

### CMake Configure Options

Qbs recognizes the following CMake options (passed to CMake in the form of `-DOPTION=value`)
to customize the build:

| Option                         | Notes                              | Default value |
| ------------------------------ | ---------------------------------- | ------------- |
| `WITH_TESTS` {.nw}             | Enable autotests.                  | `ON`          |
| `WITH_UNIT_TESTS` {.nw}        | Enable additional autotests.       | `OFF`         |
| `INSTALL_PUBLIC_HEADERS` {.nw} | Whether to install public headers. | `ON`          |


### Using ccache with CMake

To enable using [ccache](https://ccache.dev) when building Qbs, pass the following options
to CMake:
```sh
cmake -DQt5_DIR=${QT_DIR}/lib/cmake/Qt5/ -DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache ..
```

## Building Qbs with QMake

To build Qbs, enter the following command:

```sh
qmake -r qbs.pro && make
```

Depending on your platform, you might use `mingw32-make`, `nmake`, or
`jom` instead of `make`.

Installation by using `make install` is usually not needed. It is however
possible, by entering the following command.

```sh
make install INSTALL_ROOT=$INSTALL_DIRECTORY
```

### QMake Configure Options

Qbs recognizes the following qmake CONFIG options to customize the build:

| Option                        | Notes                                                                                                                                 |
| ----------------------------- | ------------------------------------------------------------------------------------------------------------------------------------- |
| `qbs_enable_unit_tests` {.nw} | Enable additional autotests.                                                                                                          |
| `qbs_disable_rpath` {.nw}     | Disable the use of rpath. This can be used when packaging Qbs for distributions which do not permit the use of rpath, such as Fedora. |
| `qbs_no_dev_install` {.nw}    | Exclude header files from installation, that is, perform a  non-developer build.                                                      |
| `qbs_no_man_install` {.nw}    | Exclude the man page from installation.                                                                                               |


In addition, you can set the `QBS_SYSTEM_SETTINGS_DIR` environment variable
before running qmake to specify a custom location for Qbs to look for its
system-level settings.

## Building Qbs with Qbs

It is also possible to build Qbs with the previously installed Qbs version.
To build Qbs, enter the following command in the source directory:
```sh
qbs
```
This will use the `defaultProfile` or pick up the Qt version and the toolchain from the `PATH`
if the `defaultProfile` is not set. See [[Configuring Profiles and Preferences]] for details
about profiles.

To run automatic tests, the `autotest-runner` product should be built:
```sh
qbs build -p autotest-runner
```
Qbs will use an empty profile when running tests which means it will try to autodetect
toolchains, Qt versions and other things based on the system environment.
It is possible to specify which profile should be used during the test-run by passing the
`QBS_AUTOTEST_PROFILE` environment variable.
This variable should be set prior to building Qbs itself; otherwise the `resolve` command
should be used to update the environment stored in the buildgraph:
```sh
export QBS_AUTOTEST_PROFILE=qt
qbs resolve
qbs build -p autotest-runner
```

It is also possible to set up a separate profile for a particular testsuite.
A profile for the `tst_blackbox_android` suite can be set up as follows:
```sh
qbs setup-android pie
export QBS_AUTOTEST_PROFILE_BLACKBOX_ANDROID=pie
```

It might be useful to set up the directory with the Qbs settings to isolate the test
environment:
```sh
export QBS_AUTOTEST_SETTINGS_DIR=~/path/to/qbs/settings
```

### Qbs Build Options

The `qbsbuildconfig` module can be used to customize the build.
Properties of that module can be passed using command line as follows:
```sh
qbs build modules.qbsbuildconfig.enableAddressSanitizer:true
```

Qbs recognizes the following properties:

| Property                       | Default value                                         | Notes                                                                                                                                                      |
| ------------------------------ | ----------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `enableAddressSanitizer` {.nw} | `false`                                               | Whether to use address sanitizer or not. Enabling this option will add the `-fsanitize=address` flag.                                                      |
| `enableUnitTests` {.nw}        | `false`                                               | Enable additional autotests. Enabling this option will export some symbols that would otherwise be private.                                                |
| `enableRPath` {.nw}            | `true`                                                | Use this property to disable the use of rpath. This can be used when packaging Qbs for distributions which do not permit the use of rpath, such as Fedora. |
| `installApiHeaders` {.nw}      | `true`                                                | Holds whether to install the header files for the Qbs libraries or not. This option is required to build against the Qbs libraries.                        |
| `enableBundledQt` {.nw}        | `false`                                               | Holds whether the Qt libraries that Qbs depends on will be bundled with Qbs during the `install` step. This option is only implemented on macOS.           |
| `libDirName` {.nw}             | `"lib"`                                               | Directory name used by `libInstallDir` and `importLibInstallDir` properties.                                                                               |
| `appInstallDir` {.nw}          | `"bin"`                                               | Relative directory path under the install prefix path to put application binaries.                                                                         |
| `libInstallDir` {.nw}          | `"bin"` on Windows, `libDirName` otherwise            | Relative directory path under the install prefix path where to put shared libraries (excluding plugins, see the `relativePluginsPath` property).           |
| `importLibInstallDir` {.nw}    | `libDirName`                                          | Relative directory path under the install prefix path where to put import libs.                                                                            |
| `libexecInstallDir` {.nw}      | `appInstallDir` on Windows, `"libexec/qbs"` otherwise | Relative directory path under the install prefix path where to put auxiliary binaries executed by the Qbs libraries.                                       |
| `systemSettingsDir` {.nw}      | `undefined`                                           | Directory that will be used by Qbs to store its settings. If not specified, a default platform-dependent directory is used.                                |
| `installManPage` {.nw}         | `true` on Unix, `false` otherwise                     | Whether to install man pages.                                                                                                                              |
| `installHtml` {.nw}            | `true`                                                | Whether to install HTML help pages.                                                                                                                        |
| `installQch` {.nw}             | `false`                                               | Whether to install qch files. See [The Qt Help Framework](https://doc.qt.io/qt-5/qthelp-framework.html) for details about qch files.                       |
| `generatePkgConfigFiles` {.nw} | auto-detected                                         | Whether to generate files for pkg-config.                                                                                                                  |
| `generateQbsModules` {.nw}     | auto-detected                                         | Whether to generate Qbs modules for the exported Qbs libraries. Use this when building another product against Qbs libraries using Qbs as build system.    |
| `docInstallDir` {.nw}          | `"share/doc/qbs/html"`                                | Relative directory path under the install prefix path where to put documentation.                                                                          |
| `pkgConfigInstallDir` {.nw}    | `libDirName` + `"/pkgconfig"`                         | Relative directory path under the install prefix path where to put pkg-config files.                                                                       |
| `qbsModulesBaseDir` {.nw}      | `libDirName` + `"/qbs/modules"`                       | Relative directory path under the install prefix path where to put Qbs modules. Applies only when `generateQbsModules` is `true`.                          |
| `relativeLibexecPath` {.nw}    | `"../"` + `libexecInstallDir`                         | Path to the auxiliary binaries relative to the application binary.                                                                                         |
| `relativePluginsPath` {.nw}    | `"../"` + `libDirName`                                | Path to plugin libraries relative to the application binary.                                                                                               |
| `relativeSearchPath` {.nw}     | `".."`                                                | Relative path to the directory where to look for Qbs development modules and items.                                                                        |
| `libRPaths` {.nw}              | auto-detected                                         | List of rpaths.                                                                                                                                            |
| `resourcesInstallDir` {.nw}    | `""`                                                  | Relative directory path under the install prefix path where to put shared resources  like documentation, Qbs user modules and items.                       |
| `pluginsInstallDir` {.nw}      | `libDirName` + `"/qbs/plugins"`                       | Relative path to the directory where to put plugins to.                                                                                                    |

## Using Docker

A set of Docker images for developing Qbs (which are maintained by the Qbs team) is available
[on Docker Hub](https://hub.docker.com/u/qbsbuild/).
Both Windows 10 and Debian Linux container types are available.

!!! note

    The source code for the Qbs development Docker images is located in the `docker/`
    directory of the Qbs source tree, if you wish to build them yourself.

### Linux Containers

The easiest way to get started is to build Qbs using a Linux container. These types of
containers are supported out of the box on all the supported host platforms: Windows, macOS,
and Linux.

The images provide everything that is necessary to build and test Qbs:

- Qt SDK for building Qbs with `qmake`
- Latest stable release of Qbs for building Qbs with Qbs

We are using docker-compose for building and running the Docker images because it simplifies
the Docker command line and ensures that the correct image tag is used. All available images
are listed in the `docker-compose.yml` file in the project root directory.

Run the following command to download the Qbs development image based on Ubuntu 20.04
_Focal:_

```sh
docker-compose pull focal
```

You can then create a new container with the Qbs source directory mounted from your host
machine's file system, by running:

```sh
docker-compose run --rm focal
```

You will now be in an interactive Linux shell where you can develop and build Qbs.

### Windows Containers

To build Qbs for Windows using Windows containers, your host OS must be running Windows 10 Pro
and have Hyper-V enabled. [Switch your Docker environment to use Windows containers](https://docs.docker.com/docker-for-windows/#switch-between-windows-and-linux-containers).

We are using docker-compose for building and running the Docker images because it simplifies
the Docker command line and ensures that the correct image tag is used. All available images
are listed in the `docker-compose.yml` file in the project root directory.

Run the following command to download the Qbs development image based on Windows 10:

```sh
docker-compose pull windows
```

You can then create a new container with the Qbs source directory mounted from your host
machine's file system, by running:

```sh
docker-compose run --rm windows
```

If you want to use Windows containers on a macOS or Linux host, you will have to create a
virtual machine running Windows 10 and register it with `docker-machine`. There is at least
[one Open Source project](https://github.com/StefanScherer/windows-docker-machine)
that helps to facilitate this by using using Packer, Vagrant, and VirtualBox.

The `docker run` command to spawn a Windows container on a Unix host will look slightly
different (assuming `windows` is the name of the Docker machine associated with the Windows
container hosting VM):

```sh
eval $(docker-machine env windows)
docker-compose run --rm windows
```

### Building Release Packages

Release packages for Qbs for Windows can be built using the following command on Windows:

```sh
docker-compose run --rm windows cmd /c scripts\make-release-archives
```

For building release packages for Windows on macOS or Linux:

```sh
eval $(docker-machine env windows)
docker-compose run --rm windows cmd /c scripts\\make-release-archives
```
