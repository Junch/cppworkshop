@echo off

@mkdir _build >nul
pushd _build >nul
cmake ..
cmake --build . --config Release
REM cmake --build . --config Release -- /verbosity:detailed
popd >nul
