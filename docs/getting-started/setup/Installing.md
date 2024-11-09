---
title: Installing
---

# Installing

Qbs binaries are available for Windows, macOS, Linux, and FreeBSD.

On all platforms, Qbs binaries are part of the [Qt Creator][Qt Creator] and [Qt SDK][Qt SDK]
installers. You can find the `qbs` executable in the `bin` directory of
Qt Creator, or within the application bundle's `MacOS` directory on macOS.

Qbs can also be built locally from sources. For more information, see
[[Appendix A: Building Qbs]].

## Windows

The Qt Project provides prebuilt binaries for Windows (x86 and x64) at
<https://download.qt.io/official_releases/qbs/>. For commercial customers of
The Qt Company, the binaries are available in the [Qt Account][Qt Account].
The binaries are packaged in a .zip folder that can be extracted to a location
of your choice.

Qbs is also available as a [Chocolatey][Chocolatey] package, which can be installed in
the usual way:

```powershell
choco install qbs
```

The `.nupkg` file can also be downloaded directly from
<https://download.qt.io/official_releases/qbs/> for
[offline installation](https://chocolatey.org/security#organizational-use-of-chocolatey).

## macOS

Qbs can be conveniently installed on macOS with the [MacPorts][MacPorts] or [Homebrew][Homebrew]
package managers:

```sh
brew install qbs
```

or

```sh
port install qbs
```

## Linux and FreeBSD

Qbs is [available via the
package management systems](https://repology.org/metapackage/qbs/versions) of Linux distributions, and FreeBSD.
