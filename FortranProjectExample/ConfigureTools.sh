#!/bin/bash

set -x

cd ..
make
cp FortranDep FortranProjectExample/tools/FortranDep
cp bin/FortranDepCore.x FortranProjectExample/tools/FortranDepCore.x
