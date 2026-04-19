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
b.mod: file.f90
a.mod: file.f90 b.mod c.mod d.inc
file.o: b.mod a.mod
```

## Options

- `-p`: Add phony targets for each `#include` dependency. This avoids `make` errors if included files are removed without updating dependencies.

## Policy

### Supported constructs

- Fortran `use` statements (module dependencies).

- Fortran `module`/`submodule` tracking.

- We only guarantee correct results for valid Fortran.

- CPP `#include` directives.

- Recursive include resolution.

### Not supported

- Fortran `include` statements (F77-style).

- Fixed form Fortran.

## Installation

TODO
