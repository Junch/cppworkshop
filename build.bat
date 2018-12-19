@mkdir _build >nul
pushd _build >nul
cmake ..
cmake --build . --config Debug -- /verbosity:detailed
REM cmake --build . --config Release -- /verbosity:detailed
popd >nul
