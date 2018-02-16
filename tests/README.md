tests
=====

Tests are used to test WLA DX if it assembles and links the program correctly.

Tests are run when you do `ctest`. Add `-V` to get verbose output. Add
`-R testname` to add a test (has regex support).


Details
-------

Each folder represents an test that WLA DX shoud build.

By default, `run_test.cmake` runs `test.cmake` in the folder, if it exists.
If it doesn't, it assembles all `*.s` files to object files and links them
all together, either by generating a custom linkfile or using a provided
linkfile (either named `linkfile` or `Linkfile`).
If there is a file called `expected.bin`, it compares the outputted binary with
the expected binary and throws an error if they don't match.

Inside an `test.cmake` file, you can add:

* `set(ASSEMBLE_NORMALLY OFF)` to disable the test runner default stuff. Useful
  if you want to do more complicated stuff (like custom stuff). It also forces
  to disable checking the output with the expected output. In that case, use
  `set(CHECK_FILE_EQUALS ON)`.
* `set(OUTPUT "newoutputname.bin")` to set the output binary to check with the
  expected binary.
* `set(EXPECTED "expectedbinary.bin")` to set the expected binary to check with
  the output binary.
* 

The file `test-common.cmake` is included in every test and serves as a small
collection of functions/macros to test stuff, like assembling and linking.
They're documented in the file and it should be pretty easy to understand.

If you create an test, copy the `template_prject` folder and do your stuff.
You have to add the folder to the `CMakeLists.txt`.
Don't worry, you won't miss it.

