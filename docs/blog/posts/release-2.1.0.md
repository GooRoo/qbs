---
title: Qbs 2.1.0 released
date: 2023-07-27
authors:
  - ckandeler
---

The [Qbs build tool](http://qbs.io) version 2.1.0 is available.

## What's new

<!-- more -->

Just as the [[release-2.0.0|2.0 release]] was mostly about the JavaScript
back-end switch, this one also features a major rewrite of the implementation, though with more
direct impact on users.

### New project resolving implementation

You may have heard that once a new software project turns from proof of concept to actual product,
it is considered good practice to throw away the dirty, hacky prototype and implement everything
from scratch, using only the most aesthetically pleasing algorithms and code so shiny it almost
blinds you.

You may also have heard about the real world, with its deadlines, resource constraints and lazy
developers, so you might not be surprised to learn that this rewrite did not happen for the
[first official qbs release](https://www.qt.io/blog/2013/05/31/qbs-1-release).

And so the original code was kept, never being given the scrutiny it deserved and having to carry
more and more features on its narrow shoulders.

"But why should I care as a user?", you may ask. That's a fair question, and the answer lies in the
somewhat underappreciated relationship between code complexity and performance: The less
straightforward the code, the higher the likelihood that it not only contains outright bugs, but
also that it will be egregiously inefficient, simply because you cannot spot the performance traps
among all the distractions.

In fact, it was exactly this performance issue that made us finally take the decision to rewrite
the project resolving code from scratch, as it was becoming clear that there was non-linear
behavior lurking deep down in some inscrutable recursive code. In case you are wondering:
"Project resolving" is the phase that turns the textual representation of the qbs files into a
build graph, after which the actual building starts.

So, what are the results? From a developer's point of view, we now have code that human beings
can understand and that won't scare away possible contributors anymore, while users should notice
a considerable speed-up when resolving medium-to-large projects. For instance, Qt Creator's qbs
project files load about 40% faster now, while for some projects with deep and complex dependencies
between modules and/or products we have observed a speed-up of 90% and more. If you felt in the
past that your projects were resolving too slowly, chances are that this release will fix that.

### Other changes

For the full list of fixes and features in this release, please refer to the
[change log](https://download.qt.io/official_releases/qbs/2.1.0/changes-2.1.0.md).

## What's next
Another nice feature of the new implementation is that it appears to lend itself to
parallelization, which was completely impossible before. We are currently investigating this; if
it works out, we might see another considerable speed-up. (Again, this is about project resolving.
The actual building is of course already parallelized.)

## Try it

Qbs is available for download on the
[download page](https://download.qt.io/official_releases/qbs/2.1.0/).

Please report issues in our [bug tracker](https://bugreports.qt.io/browse/QBS/).

Join our [Discord server](https://discord.gg/zhMHvC5GNa) for live discussions.

You can use our [mailing list](https://lists.qt-project.org/mailman/listinfo/qbs) for questions
and discussions.

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

Qbs 2.1.0 is also included in Qt Creator 11.0.0.

### Contribute
If You are a happy user of Qbs, please tell others about it. But maybe you would
like to contribute something. Everything that makes Qbs better is highly
appreciated. Contributions may consist of reporting bugs or fixing them right
away. But also new features are very welcome. Your patches will be automatically
sanity-checked, built and verified on Linux, macOS and Windows by our CI bot.
Get started with instructions in the [Qbs Wiki](https://wiki.qt.io/Qbs).

Thanks to everybody who made the 2.1 release happen:

* Andrey Filipenkov
* Christian Kandeler
* Denis Shienkov
* Ivan Komissarov
* Marc Mutz
* Raphael Cotty
* Thiemo van Engelen
