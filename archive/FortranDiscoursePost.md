# A small, portable Fortran dependency generator (modules, submodules, and CPP includes)

Hi all,

I wanted to share a small tool I’ve been using privately for some time and see whether it might be useful to others. It’s a lightweight Fortran dependency extractor focused specifically on *modules and submodules*. The tool emits *Make-compatible* dependency rules:

- `use` statements: `.mod` dependencies,
- `module` / `submodule` definitions: `.mod` / `.smod` targets,
- CPP `#include` directives (including recursive includes via preprocessing).

For example, for:
```fortran
!file.f90
module a
  use b
  ![...]
end

submodule(a) c
  use d
  ![...]
end

submodule(e) f
  use a
  ![...]
end
```
running `FortranDep -l file.f90` produces:
```shell
file.o:  file.f90  \
  b.mod  \
  d.mod  \
  e.smod
a.mod:  file.f90  \
  b.mod
a@c.smod:  file.f90  \
  d.mod
e@f.smod:  file.f90  \
  e.smod
```

The motivation is simple: Fortran compilers already compute module dependencies internally, but this information is rarely exposed in a form that traditional build systems can consume. This tool exists to expose that information in a usable way (similarly to `gcc -MMD` in the C/C++ world). It is fully portable and can be used out of the box.

Repository:

https://github.com/irukoa/FortranDep

### Context

In recent years, tools like `fpm` have made it much easier to build and manage Fortran projects, especially for new codebases. However, many projects—particularly in HPC and scientific computing—still rely on existing Make-based workflows, where introducing a full build system is not always practical.

At the same time, there are a number of dependency generators and scripts floating around (often Python-based or compiler-specific), but in my experience they tend to:

- only partially handle module dependencies,
- ignore submodules or CPP `#include` s, or
- be tightly coupled to a specific compiler

This tool is an attempt to fill that gap with something small, portable, and predictable.

---

A few notes on scope and intent:

-   It works well with *gfortran*, which is what I primarily use.
-   It does *not* aim to support every compiler dialect or extension.
-   It does not replace `fpm` or any existing build system.
-   It’s designed for people who already manage builds with Make and want reliable module dependency tracking.

At the moment it’s a small, self-contained utility I’ve been using successfully on medium-sized codebases.

Thanks for reading.
