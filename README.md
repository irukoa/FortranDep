# FortranDep

Generates `make`-compatible dependency rules for Fortran source files, similarly to what `gcc -MMD` provides for C/C++.

## Usage

```
./$(PATH)/FortranDep [-p] [-d] [-l] [-h] <file> [GccArgs]
```

### Options

- `-p`: Add phony targets for each `#include` dependency. This avoids `make` errors if included files are removed without updating dependencies.
- `-d`: Add a dependency target for the source file (depends on the source and all included files).
- `-l`: Enable lax mode (object files depend on program units defined within them).
- `-h`: Print this help and exit.
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

- CPP `#include` directives. Ownership model: all targets in the file depend on all included files.

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
```fortran
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
  c.mod  \
  d.inc
b.mod:  file.f90  \
  d.inc
a.mod:  file.f90  \
  c.mod  \
  d.inc
```

## Known overshoots

<details>

`FortranDep` may occasionally report redundant dependencies (overshoots) when multiple program units are present in the same source file.

This happens because `FortranDep` is intentionally implemented as a lightweight single-pass lexer. In valid Fortran, standalone `end` statements are ambiguous: the lexer cannot always determine whether they terminate a `module`, `submodule`, or another program unit such as a procedure.

To reduce overshoots, `FortranDep` checks whether an `end` statement is followed by `module` or `submodule`. In those cases, dependency ownership is correctly returned to the object file.

For example:
```fortran
module a
end module
subroutine b
  use c
end
```
produces:
```
#OBJECT#:  #SOURCEFILE#  \
  c.mod
a.mod:  #SOURCEFILE#
```
while:
```fortran
module a
end
subroutine b
end
```
produces:
```
#OBJECT#:  #SOURCEFILE#  \
  c.mod
a.mod:  #SOURCEFILE#  \
  c.mod
```
In the second case, `c.mod` is conservatively attributed to both the object file and `a.mod`.

If avoiding these overshoots matters for your workflow, it is recommended to explicitly label `module` and `submodule` termination statements (`end module`, `end submodule`).

</details>
