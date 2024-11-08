---
title: Qbs 2.0.2 released
date: 2023-06-25
authors:
  - ABBAPOH
---

The [Qbs build tool](http://qbs.io) version 2.0.2 is available.

## What's new

<!-- more -->

This is a minor bugfix release that contains some fixes such as:

* Fixed handling of non-string exceptions
  ([QBS-1734](https://bugreports.qt.io/projects/QBS/issues/QBS-1734)).
* Fixed Utilities.versionCompare() for four-segment versions numbers
  ([QBS-1733](https://bugreports.qt.io/projects/QBS/issues/QBS-1733)). This fixes compiler
  feature detection for the clang-cl toolchain.
* Fixed support for static plugins and Qt >= 6.5 as well as improved handling of .prf files which
  provide info about Qt libraries. This fixes build for the iOS platform with the recent Qt
  versions ([QBS-1732](https://bugreports.qt.io/projects/QBS/issues/QBS-1732)).

Sources, binaries, change log etc can be found
[here](https://download.qt.io/official_releases/qbs/2.0.2/).

This release of qbs is also part of Qt Creator 10.0.2.

Also, Qbs binaries are available via different package managers such as
[Chocolatey](https://community.chocolatey.org/packages/qbs),
[Brew](https://formulae.brew.sh/formula/qbs), [macports](https://ports.macports.org/port/qbs/) and
[others](https://repology.org/metapackage/qbs/versions).

Thanks to everybody who made the 2.0.2 release happen:

# Contributors
* Christian Kandeler
* Ivan Komissarov
* Leon Buckel
