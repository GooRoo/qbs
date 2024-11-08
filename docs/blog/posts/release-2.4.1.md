---
title: Qbs 2.4.1 released
date: 2024-08-08
authors:
  - ABBAPOH
---

The [Qbs build tool](http://qbs.io) version 2.4.1 is available.

## What's new

<!-- more -->

This is a minor bugfix release that contains some fixes such as:

* Fix LibraryProbe to take into account libraries both with and without the "lib" prefix. This
  fixes the Conan provider for MinGW.
* Conan module provider no longer sets platform to "none" for baremetal toolchains
  ([QBS-1795](https://bugreports.qt.io/projects/QBS/issues/QBS-1795)).

Sources, binaries, etc can be found
[here](https://download.qt.io/official_releases/qbs/2.4.1/).

This release of qbs is also part of Qt Creator 14.0.1.

Also, Qbs binaries are available via different package managers such as
[Chocolatey](https://community.chocolatey.org/packages/qbs),
[Brew](https://formulae.brew.sh/formula/qbs), [macports](https://ports.macports.org/port/qbs/) and
[others](https://repology.org/metapackage/qbs/versions).

### Contributors
Thanks to everybody who made the 2.4.1 release happen:

* Christian Kandeler
* Ivan Komissarov
