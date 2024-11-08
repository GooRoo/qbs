---
title: Qbs 1.19.0 released
date: 2021-06-03
authors:
  - rweickelt
---

The [Qbs build tool](http://qbs.io) version 1.19.0 is available.

Qbs is a community-driven language-agnostic build automation system. It is fast
and offers an easy-to-learn language based upon QML.

## What's new

About 151 contributions went into this release since version 1.18.0. We have
selected a few items below. Have a look into the complete
[changelog](https://code.qt.io/cgit/qbs/qbs.git/tree/changelogs/changes-1.19.0.md)
if you are interested in more details.

<!-- more -->

### Codesign module

A new [codesign module](https://doc.qt.io/qbs/qml-qbsmodules-codesign.html) that implements
code signing for Apple, Android and Windows platforms has been added.

On Apple platforms, it is now possible to sign application bundles as well as standalone
executables. By default, Qbs uses Xcode specs to understand whether it should enable codesigning.
For example, codesigning is now enabled for iOS (non-simulator) builds, but not enabled for
macOS, matching Xcode behavior. To sign an application for the Darwin platform, one
should specify the codesign.signingIdentity and codesign.provisioningProfile properties:

```qml
CppApplication {
    files: "main.m"
    codesign.enableCodeSigning: true
    codesign.provisioningProfile: "my provisioning profile name"
    codesign.signingIdentity: "Apple Development: My Team Name"
}
```
Unlike Xcode, Qbs does not support auto-detection of the signingIdentity based on the
provisioningProfile (yet).

For more information, see the [How-To section](https://doc.qt.io/qbs/howtos.html#how-do-i-sign-an-application-for-an-apple-platform)

On the Android platform, it is possible to sign Aab and Apk packages. A typical example can be
written as follows:

```qml
QtGuiApplication {
    files: "main.cpp"
    codesign.enableCodeSigning: true
    codesign.keystorePath: "path to the keyStore file"
    codesign.keystorePassword: "keystore password"
    codesign.keyPassword: "key password"
    codesign.keyAlias: "key alias"
}
```

On Windows, to sign a standalone executable using the
[signtool.exe](https://docs.microsoft.com/en-us/dotnet/framework/tools/signtool-exe), one can
specify the codesign.certificatePath and codesign.certificatePassword properties:

```qml
CppApplication {
    files: "main.m"
    codesign.enableCodeSigning: true
    codesign.certificatePath: "C:\certs\mycert.pfx"
    codesign.certificatePassword: "p@ssw0rd"
}
```

### C/C++ Support

- Qbs now supports multiple MSVC compiler versions installed in one Visual Studio installation
  as well as multiple Windows SDK versions (QBS-1498). Also, multiple compiler versions are
  properly detected by qbs setup-toolchains - Qbs will create a separate profile for every
  compiler version.

- Added support for the HCS08 architecture to SDCC and IAR toolchains (QBS-1631, QBS-1629) as
  well as support for the HCS12 architecture to GCC and IAR toolchains (QBS-1630, QBS-1550).

- Auto-detection of the Keil toolchains has been fixed.

### Other
- Added support for Android NDK 22.1.7171670.
- Added support for Xcode 12.5 (QBS-1644).
- Fixed support for Qt 6.1 (QBS-1636).

Try it
------

Qbs is available for download on the
[download page](https://download.qt.io/official_releases/qbs/1.19.0).
Please report issues in our [bug tracker](https://bugreports.qt.io/browse/QBS/). Join our [Discord
server](https://discord.gg/zhMHvC5GNa) for live discussions. You can use our
[mailing list](https://lists.qt-project.org/mailman/listinfo/qbs)
for questions and discussions. The [documentation](https://doc.qt.io/qbs/index.html)
and [wiki](https://wiki.qt.io/Qbs) are also good places to get started.
Qbs is also available from a number of package repositories
([Chocolatey](https://chocolatey.org/packages/qbs),
[MacPorts](https://www.macports.org/ports.php?by=name&substr=qbs),
[Homebrew](https://formulae.brew.sh/formula/qbs)) and is updated on each
release by the Qbs development team. It can also be installed through
the native package management system on a number of Linux distributions.
Please find a complete overview on
[repology.org](https://repology.org/project/qbs/versions).
Qbs 1.19.0 is also included in Qt Creator 4.15.0.

Contribute
----------

If You are a happy user of Qbs, please tell others about it. But maybe you would
like to contribute something. Everything that makes Qbs better is highly
appreciated. Contributions may consist of reporting bugs or fixing them right
away. But also new features are very welcome. Your patches will be automatically
sanity-checked, built and verified on Linux, macOS and Windows by our CI bot.

Get started with instructions in the [Qbs Wiki](https://wiki.qt.io/Qbs).

Thanks to everybody who made the 1.19.0 release happen:

- Andrey Filipenkov
- Denis Shienkov
- Christian Kandeler
- Jan Blackquill
- Jake Petroules
- Ivan Komissarov
- Max Bespalov
- Mitch Curtis
- Orgad Shaneh
- Raphaël Cotty
- Richard Weickelt
