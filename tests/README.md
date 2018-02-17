tests
=====

Tests are used to test WLA DX if it assembles and links the program correctly.

Tests are run when you do `cmake --build . --target test`. Change `test` with
`check` to get verbose output.



Details
-------

Each folder represents an test that WLA DX shoud build.

By default, `run_test.cmake` runs `test.cmake` in the folder, if it exist.
If it doesn't, it compiles all `*.s` files to object files and links them
all together. If there is a file called `expected.bin`, it compares the
outputted binary with the expected binary and throws an error if they don't
match.

The file `test-common.cmake` is included in every test and serves as a small
collection of functions/macros to test stuff, like assembling and linking.
They're documented in the file and it should be pretty easy to understand.

If you create an test, copy the template folder and do your stuff. You have
to add the folder to the `CMakeLists.txt`. Don't worry, you won't miss it.

