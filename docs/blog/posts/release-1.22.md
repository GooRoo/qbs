---
title: Qbs 1.22 Released
date: 2022-03-28
authors:
  - ABBAPOH
---

The [Qbs build tool](http://qbs.io) version 1.22.0 is available.

Qbs is a community-driven language-agnostic build automation system. It is fast
and offers an easy-to-learn language based upon QML.

## What's new

About 82 contributions went into this release since version 1.21.0. We have
selected a few items below. Have a look into the complete
[changelog](https://code.qt.io/cgit/qbs/qbs.git/tree/changelogs/changes-1.22.0.md)
if you are interested in more details.

<!-- more -->

## General

* A new Host service was introduced, providing information about the host system that
used to be available from the qbs module, but did not really belong there. In addition, some more
qbs module properties have moved to the FileInfo service.

* The *product* variable in *Export* items now points to the importing product, rather than
the exporting one. This change matches the behavior of the *product* variable in *Module* items.

* *Probe* items are now also available in *ModuleProvider* items.

## C/C++ Support

* Added support for the Open Watcom toolchain.
* Reduced unneeded re-linking on Linux by ignoring changes to weak symbols in library
  dependencies by default.
* Adapted to changes in Xcode 13.3 ([QBS-1693](https://bugreports.qt.io/browse/QBS-1693)).

## Try it

Qbs is available for download on the [download
page](https://download.qt.io/official_releases/qbs/1.22.0). Please
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
Qbs 1.22.0 is also included in Qt Creator 7.0.0 which was released recently.

## Contribute

If You are a happy user of Qbs, please tell others about it. But maybe you would
like to contribute something. Everything that makes Qbs better is highly
appreciated. Contributions may consist of reporting bugs or fixing them right
away. But also new features are very welcome. Your patches will be automatically
sanity-checked, built and verified on Linux, macOS and Windows by our CI bot.
Get started with instructions in the [Qbs Wiki](https://wiki.qt.io/Qbs).
Thanks to everybody who made the 1.22.0 release happen:

## Contributors
* Christian Kandeler
* Denis Shienkov
* Ivan Komissarov
* Jan Blackquill
* Leena Miettinen
* Marius Gripsgard
* Mitch Curtis
* Raphael Cotty
