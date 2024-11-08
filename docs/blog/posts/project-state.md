---
title: The State of the Project
date: 2023-05-25
authors:
  - ABBAPOH
---

# The State of the Project

It's been a while since The Qt Company
[discontinued](https://www.qt.io/blog/2018/10/29/deprecation-of-qbs) development of the Qbs project
and it became an open-source project, and it seems the current status is not clear for many people.

<!-- more -->

Qbs is now maintained by a small group of enthusiasts and we had 14 releases with lots of features
since The Qt Company discontinued the development. Originally, Qbs was planned to be used as a
build system for Qt itself, which is clearly not the case now and Qt is now built with CMake.
Therefore, we changed our focus to be the general build tool for C/C++
(which wasn't very hard as Qbs is designed to be independent of programming language).
We added support for several BareMetal compilers such [Keil](https://www.keil.com),
[IAR](https://www.iar.com), [Digital Mars](https://www.digitalmars.com) and others. If you are
developing for BareMetal, we encourage you to try Qbs! As for the Qt ecosystem, Qbs keeps up with
the latest Qt versions and as a Qt user, you can use any of the three build systems for your
projects - Qbs, CMake and QMake. There are several open source projects that use Qbs (the most
notable is [Tiled](https://www.mapeditor.org)) as well as some private companies.
We also implemented the
[QBS plugin](https://marketplace.visualstudio.com/items?itemName=qbs-community.qbs-tools) for
Visual Studio Code that allows to use Qbs not only in QtCreator. The plugin supports all basic
features such as syntax highlighting and debugging and is pretty convenient for
development of Qbs itself.

Qbs still uses The Qt Company's infrastructure for development which they kindly provided - such as
[Gerrit](https://codereview.qt-project.org/q/project:qbs%252Fqbs+status:open) for code review
and [Jira](https://bugreports.qt.io/browse/QBS/) for bug tracking. We considered migrating to
GitHub entirely, but decided that it would take too much effort for little gain, as we are already
using GitHub actions for [our CI](https://github.com/qbs/qbs/actions) as well as GitHub
[pages](https://github.com/qbs/qbs.io) for the [Qbs website](https://qbs.io).


## Future plans

Recently, we had a major [release](./release-2.0.0.md) which replaced the QtScript
backend (based on enormous JavaScriptCore) with a light-weight
[QuickJS](https://bellard.org/quickjs/) Engine. As mentioned in the post, we will have a new
implementation of the module loading in 2.1 release - the old code was quite obscure and had severe
performance issues with huge projects. Another big feature we are working on is the management of
external dependencies - searching for libraries in the system, as well as support for package
managers such as Conan. We want to make this flexible so the users would have full control from
where modules come - from package manager, system or built from source as a part of the project.
We are also planning to add support for C++ Modules since most compilers finally support them.

## Try it

We always encourage you to try Qbs in your projects. If you run into a bug or have a feature
request, report it in our [bug tracker](https://bugreports.qt.io/browse/QBS/). Or, if you are
willing to implement it yourself, please upload the fix to the upstream, see the
[Contributing](https://github.com/qbs/qbs/blob/master/CONTRIBUTING.md) page for details.
