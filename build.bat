@echo off

@mkdir _build >nul
pushd _build >nul
cmake ..
REM cmake --build . --config Debug
cmake --build . --config Release -- /verbosity:detailed
popd >nul
