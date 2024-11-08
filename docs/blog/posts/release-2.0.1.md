---
title: Qbs 2.0.1 released
date: 2023-05-11
authors:
  - ABBAPOH
---

The [Qbs build tool](http://qbs.io) version 2.0.1 is available.

## What's new

<!-- more -->

This is a minor bugfix release that contains several important fixes such as:

* Fixed crash when importing missing JavaScript file
  ([QBS-1730](https://bugreports.qt.io/projects/QBS/issues/QBS-1730)).
* Fixed building applications with mingw toolchain and Qt6
  ([QBS-1724](https://bugreports.qt.io/projects/QBS/issues/QBS-1724)).
* Added support for Xcode 14.3.
* Fixed codesigning on macOS ([QBS-1722](https://bugreports.qt.io/projects/QBS/issues/QBS-1722)).
* Fixed detecting Xcode via the xcode-select tool.
* Partially fixed Qt support on iOS for Qt6 (Qt versions up to 6.3 should work now correctly)
  ([QBS-1732](https://bugreports.qt.io/projects/QBS/issues/QBS-1732)).
* Qt is no longer being set up in qbspkgconfig provider when cross compiling
  ([QBS-1717](https://bugreports.qt.io/projects/QBS/issues/QBS-1717)).

As usual, sources, binaries, change log etc can be found
[here](https://download.qt.io/official_releases/qbs/2.0.1/).

This release of qbs is also part of Qt Creator 10.0.1.

Also, Qbs binaries are available via different package managers such as
[Chocolatey](https://community.chocolatey.org/packages/qbs),
[Brew](https://formulae.brew.sh/formula/qbs), [macports](https://ports.macports.org/port/qbs/) and
[others](https://repology.org/metapackage/qbs/versions).

Thanks to everybody who made the 2.0.1 release happen:

* Christian Kandeler
* Ivan Komissarov
