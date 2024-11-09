---
title: 'Target Platforms'
---

# Target Platforms

Qbs has built-in support for building applications for all major platforms.
Platform support is implemented as a set of [[List of Modules|modules]]
that products depend on. The following topics describe the features specific
to a particular platform, point out things to look out for, and provide tips
for fully benefiting from the Qbs functions.

In addition to the platforms explicitly listed below, Qbs should generally
work on other UNIX and Unix-like platforms but these are not regularly
tested or officially supported.

Qbs recognizes the existence of at least AIX, HP-UX, Solaris, FreeBSD,
NetBSD, OpenBSD, GNU Hurd, and Haiku, but provides no explicit support
(except some minimal support for FreeBSD).

For instructions on how to setup the target platform, see the
[[qbs::targetPlatform|qbs.targetPlatform]] property.
