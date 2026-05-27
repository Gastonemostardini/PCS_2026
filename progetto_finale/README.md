comando per compilazione test e avvio main
```shell
cmake -B build && cmake --build build && ctest --output-on-failure --test-dir build --verbose && ./build/main
```