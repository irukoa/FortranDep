# FortranDep

Generates `make`-compatible dependency rules for Fortran source files, similarly to what `gcc -MMD` provides for C/C++.

## Usage

```
./$(PATH)/FortranDep [-p] [-d] [-l] <file> [GccArgs]
```

### Options

- `-p`: Add phony targets for each `#include` dependency. This avoids `make` errors if included files are removed without updating dependencies.
- `-d`: Add a dependency target for the source file (depends on the source and all included files).
- `-l`: Enable lax mode (object files do not depend on program units defined within them).
- `GccArgs`: list of arguments to pass to `gcc`. Applied when preprocessing.

## Installation

This tool is fully portable and does not require system-wide installation. To build the core executable, run:
```
make PROFILE=release && cp bin/FortranDepCore.x ./
```
The shell frontend `FortranDep` **must be co-located** with the compiled binary `FortranDepCore.x`. Both files are expected to live in the same directory at runtime—this is a strict requirement. You are, however, free to copy or move this pair of files anywhere (for example into a project-local `tools/` directory) as long as they remain together. This makes the tool well-suited as a project-specific dependency with no installation step.

## Policy

Requires `gcc`.

Dependencies may be over-approximated when multiple program units are present in a single file.

### Supported constructs

- Fortran `use` statements (module dependencies).

- Fortran `module`/`submodule` tracking.

- We only guarantee correct results for valid Fortran.

- CPP `#include` directives (searched with `-I.`). Ownership model: all targets in the file depend on all included files.

- Recursive include resolution.

- GNU Fortran style submodule dependencies. That is, `submodule (a) b` will depend on `a.smod`.

### Not supported

- Fortran `include` statements (F77-style).

- Fixed form Fortran.

- Intel Fortran style submodule dependencies. That is, `submodule (a) b` will not depend on `a.mod`.

## Prebuilt binaries

Prebuilt binaries are available in the project releases. You can download `FortranDep` and `FortranDepCore.x` directly and use them without building. May require `chmod +x`.

As with the source build, both files must be co-located in the same directory to function correctly.

## Example

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
