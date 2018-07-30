mkdir _build > /dev/null
pushd _build > /dev/null
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -Dcoverage=ON ..
make
make test
ctest
make Test_coverage
popd > /dev/null
