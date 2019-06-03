BUILDDIR=_build
mkdir ${BUILDDIR} > /dev/null
pushd ${BUILDDIR} > /dev/null

cmake .. -DCMAKE_BUILD_TYPE=Debug
make

popd > /dev/null
