@echo off

@mkdir _build64 >nul
pushd _build64 >nul
cmake -A x64 ..
REM cmake --build . --config Debug
cmake --build . --config Release -- /verbosity:detailed
popd >nul
