---
hide:
  - footer
  - navigation
  - toc
---

### Overview

Qbs is an open source, cross-platform build system that simplifies the process of building software.

Key features:

- Simple, easy-to-write build recipes
- Fast build system with parallel builds and tests
- Portable build recipes
- First-class support for many platforms

Starting a new project is as simple as writing a short file:

```qml
CppApplication {
    files: [
        "*.cpp"
    ]
}
```
And running **`qbs`**.
