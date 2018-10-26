#!/bin/bash

HELP=false
VERBOSE=false
COVERAGE=OFF

while getopts vhc opt
do
    case ${opt}
    in
        v) VERBOSE=true ;;
        h) HELP=true    ;;
        c) COVERAGE=ON ;;
    esac
done

BUILDDIR=_build
if [ ${COVERAGE} = ON ]; then
    BUILDDIR=_build_coverage
fi

echo VERBOSE=${VERBOSE}
echo HELP=${HELP}
echo COVERAGE=${COVERAGE}
echo BUILDDIR=${BUILDDIR}

if [ ${HELP} = true ]; then
    echo "-v Output verbose information in making"
    echo "-h Display help information"
    echo "-c Build and run the version to get the code coverage"
    exit 1
fi

mkdir ${BUILDDIR} > /dev/null
pushd ${BUILDDIR} > /dev/null
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -Dcoverage=${COVERAGE} ..

if [ ${VERBOSE} = true ]; then
    make VERBOSE=1
else
    make
fi

if [ ${COVERAGE} = ON ]; then
    make test
    ctest
    make Test_coverage
fi

popd > /dev/null
