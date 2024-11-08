---
title: Qbs 2.0 released
date: 2023-04-25
authors:
  - ckandeler
---

The [Qbs build tool](http://qbs.io) version 2.0.0 is available.

## What's new

If this release looks a bit short on features, that's because a lot of work was spent on one
rather daunting task, namely:

### New JavaScript backend

<!-- more -->

When the deprecation of QtScript was announced for Qt 6, we quickly started looking for a
light-weight replacement for our JavaScript backend, as the prospect of maintaining QtScript
with its complex JavaScriptCore dependency ourselves did not seem particularly appealing. However,
none of the candidates offered an API to intercept property accesses, which we absolutely require
for performance reasons. That changed when [QuickJS](https://bellard.org/quickjs/) came along, so
we could finally start porting, and just a couple of ~~days~~ ~~weeks~~ months later we had a
functional new backend.

From a user's point of view, there should not be any noticeable changes; in particular, the
performance appears to be about the same. However, QtScript was rather forgiving about accepting
undefined values in contexts where it didn't have to, so the new implementation might uncover
some glitches in your projects.

As a final point, the magnitude of this change gave us an excuse to bump the major version number, so we did.

### Removal of qmake project files

With our cmake project files having been feature complete for a while now (not to mention the
qbs ones), the qmake project files became mostly a maintenance burden and were removed.

## What's kinda new

Some more relevant changes since our last blog post:

* The new [Host](https://qbs.io/docs/jsextension-host/) extension offers information about
  the host system. Most of its functions used to be properties of the qbs module, but did not
  belong there.
* The new [Sanitizers.address](https://qbs.io/docs/qml-qbsmodules-sanitizers-address/) module
  provides [ASAN](https://github.com/google/sanitizers/wiki/AddressSanitizer) support for projects
  in a convenient manner.
* The FileInfo extension has an executableSuffix function now.
* ModuleProvider items now support Probes.
* We added support for the OpenWatcom toolchain.
* Android multi-arch packages are supported again wth Qt >= 6.3.

## What's next
While it's still a couple of months away, we can already say that the 2.1 release will see a
long-overdue rewrite of our project resolving code. If you think that's irrelevant to you as a
user, you might be surprised by the performance implications. More details in the next blog post.

## Try it

Qbs is available for download on the [download
page](https://download.qt.io/official_releases/qbs/1.23.0).

Please report issues in our [bug tracker](https://bugreports.qt.io/browse/QBS/).

Join our [Discord server](https://discord.gg/zhMHvC5GNa) for live discussions.

The [documentation](https://qbs.io/docs/index.html)
and [wiki](https://wiki.qt.io/Qbs) are also good places to get started.

Qbs is also available from a number of package repositories
([Chocolatey](https://chocolatey.org/packages/qbs),
[MacPorts](https://www.macports.org/ports.php?by=name&substr=qbs),
[Homebrew](https://formulae.brew.sh/formula/qbs)) and is updated on each
release by the Qbs development team. It can also be installed through
the native package management system on a number of Linux distributions.
Please find a complete overview on
[repology.org](https://repology.org/project/qbs/versions).

Qbs 2.0.0 is also included in Qt Creator 10.0.0.

### Contribute
If You are a happy user of Qbs, please tell others about it. But maybe you would
like to contribute something. Everything that makes Qbs better is highly
appreciated. Contributions may consist of reporting bugs or fixing them right
away. But also new features are very welcome. Your patches will be automatically
sanity-checked, built and verified on Linux, macOS and Windows by our CI bot.
Get started with instructions in the [Qbs Wiki](https://wiki.qt.io/Qbs).

Thanks to everybody who made the 2.0 release happen:

* Christian Kandeler
* Ivan Komissarov
* Orgad Shaneh
* Pino Toscano
