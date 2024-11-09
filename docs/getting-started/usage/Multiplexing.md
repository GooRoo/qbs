---
title: Multiplexing
---

# Multiplexing

Multiplexing is an advanced Qbs feature that allows a product to be
transparently built in multiple _passes_ along with an optional, final
_aggregate_ pass that allows the output artifacts of the initial passes
to be combined or otherwise operated on in some way.

The multiplexing feature is used to implement certain platform-specific
behavior: specifically, it allows applications and libraries on Apple
platforms to be compiled into _fat_ binaries containing multiple CPU
architectures, the creation of Apple frameworks containing multiple
_variants_ (for example, combined debug and release builds), and the
creation of Android application and library packages containing native
code built for multiple Android ABIs.

A product can be multiplexed over the [[qbs::architectures|qbs.architectures]] property (which maps to [[qbs::architecture|qbs.architecture]]), [[qbs::buildVariants|qbs.buildVariants]] property
(which maps to [[qbs::buildVariant|qbs.buildVariant]]), and [[qbs::profiles|qbs.profiles]] (which maps to [[Project::profile|Project.profile]]).

For example, to build a "fat" `iOS` binary containing two architectures, use the following
command:
```sh
qbs build modules.qbs.targetPlatform:ios modules.qbs.architectures:arm64,armv7a
```

!!! note

    The implementation details around multiplexing are subject to change.

Product multiplexing works by examining the
[[Product::multiplexByQbsProperties|Product.multiplexByQbsProperties]]
property, which can
be set to the list of properties your product should multiplex over. For
example, `multiplexByQbsProperties` might contain two strings,
`"architectures"` and `"buildVariants"`. Qbs evaluates the values of
`qbs.architectures` and `qbs.buildVariants`, which in turn might contain
the values `["x86", "x86_64"]` and `["debug", "release"]`. Qbs will build
all the possible configurations of the product: `(x86, debug)`,
`(x86, release)`, `(x86_64, debug)`, and `(x86_64, release)`.

If the [[Product::aggregate|Product.aggregate]] property is `true`, the
product will also be
built a fifth time, with the values of the multiplexed properties left
undefined. The aggregate product will have an automatic dependency on the
original four instances of the product, allowing it to collect their output
artifacts and to operate on them.

The aggregate product is used in situations where the target artifacts of
the individually multiplexed instances must be combined into one final
aggregate artifact that makes up the overall product.
Bundle products on Apple platforms use the aggregate product to create the
bundle artifacts (such as `Info.plist` and `PkgInfo`) that are independent
of a particular architecture or build variant. In addition, they use the
`lipo` tool to join together the built native code for different
architectures (such as `x86` and `x86_64`) into the final,
multi-architecture fat binary that the app bundle contains.
