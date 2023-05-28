cmake -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DCF_ENABLE_COV=ON
cmake --build build
cd build && ctest
grcov tests --binary-path tests --source-dir tests --output-path lcov.info --output-type lcov
genhtml -o coverage_report --branch-coverage lcov.info
