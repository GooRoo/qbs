---
title: Qbs 1.23 Released
date: 2022-06-20
authors:
  - ABBAPOH
---

The [Qbs build tool](http://qbs.io) version 1.23.0 is available.

Qbs is a community-driven language-agnostic build automation system. It is fast
and offers an easy-to-learn language based upon QML.

## What's new

About 36 contributions went into this release since version 1.22.0. We have
selected a few items below. Have a look into the complete
[changelog](https://code.qt.io/cgit/qbs/qbs.git/tree/changelogs/changes-1.23.0.md)
if you are interested in more details.

<!-- more -->

## C/C++ Support

* Added new module *Sanitizers.address* for simple cross-platform ASan configuration.
* Added support for c17 and c2x values in *cpp.cLanguageVersion*.
* Added support for *cpp.cLanguageVersion* for the MSVC toolchain.
* Fix passing linker scripts to iar and keil toolchains (QBS-1704).

## Qt Support

* Adapted to new location of several tools such as qscxmlc and qhelpgenerator in Qt 6.3.

## Try it

Qbs is available for download on the [download
page](https://download.qt.io/official_releases/qbs/1.23.0). Please
report issues in our [bug tracker](https://bugreports.qt.io/browse/QBS/). Join our [Discord
server](https://discord.gg/zhMHvC5GNa) for live discussions. We are not
using IRC anymore. You can use our [mailing list](https://lists.qt-project.org/mailman/listinfo/qbs)
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
Qbs 1.23.0 is also included in Qt Creator 8.0.0 which was released recently.

## Contribute

If You are a happy user of Qbs, please tell others about it. But maybe you would
like to contribute something. Everything that makes Qbs better is highly
appreciated. Contributions may consist of reporting bugs or fixing them right
away. But also new features are very welcome. Your patches will be automatically
sanity-checked, built and verified on Linux, macOS and Windows by our CI bot.
Get started with instructions in the [Qbs Wiki](https://wiki.qt.io/Qbs).
Thanks to everybody who made the 1.23.0 release happen:

## Contributors
* Christian Kandeler
* Dmitry Shachnev
* Ivan Komissarov
* Max Bespalov
* Orgad Shaneh
