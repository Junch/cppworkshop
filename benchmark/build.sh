BUILDDIR=_build
mkdir ${BUILDDIR} > /dev/null
pushd ${BUILDDIR} > /dev/null

cmake ..
make

popd > /dev/null
