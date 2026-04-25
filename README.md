# FortranDep

Generates `make`-compatible dependency rules for Fortran source files, similarly to what `gcc -MMD` provides for C/C++.

## Usage

```
FortranDep [-p] <file>
```

### Example

Given,
```
!file.f90
module b
! [...]
end module b
module a
use b
use c

#include "d.inc"
! [...]
end module a
```
produces:
```
file.o:  file.f90  \
  b.mod  \
  a.mod  \
  c.mod  \
  d.inc
b.mod:  file.f90  \
  d.inc
a.mod:  file.f90  \
  b.mod  \
  c.mod  \
  d.inc
```

## Options

- `-p`: Add phony targets for each `#include` dependency. This avoids `make` errors if included files are removed without updating dependencies.

## Policy

Requires `gcc`.

### Supported constructs

- Fortran `use` statements (module dependencies).

- Fortran `module`/`submodule` tracking.

- We only guarantee correct results for valid Fortran.

- CPP `#include` directives. Ownership model: all targets in the file depend on all included files.

- Recursive include resolution.

- GNU Fortran style submodule dependencies. That is, `submodule (a) b` will depend on `a.smod`.

### Not supported

- Fortran `include` statements (F77-style).

- Fixed form Fortran.

- Intel Fortran style submodule dependencies. That is, `submodule (a) b` will not depend on `a.mod`.

## Installation

TODO
