tests
=====

Tests are used to test WLA DX if it assembles and links the programs correctly.

Tests are run when you run `ctest`. Add `-V` to get verbose output. Add
`-R testname` to only run tests matching the regular expression.


Folder Structure
----------------

Each subfolder is the architecture (runs `wla-ARCH`) the test runner should
use. In those folders, each folder represents an "test". These should be
added to `CMakeLists.txt`. The `template_project` is an example project you
can copy and paste to get started. Don't forget to add to `CMakeLists.txt`
in this folder. Don't worry, you won't miss it.

The folder `all` tries assemble the files using all architectures, seperately.
Note that it is not wise to use assembly instructions since those differ. They
are useful to test non-assembly instructions.

Implementation Details
----------------------

### Phases

1. Set default values
2. Run `test.cmake`, if it exists
3. Get data from previous values (`setup()`, things like getting source files)
4. Start assembling, if it has been enabled
5. Check `${OUTPUT}` (`output.bin`) with `${EXPECTED}` (`expected.bin`), if
   the latter exists and hsa been enabled

### Details

`run_test.cmake` is the test runner, aka the script that executes the test.

If no source file has been set (`${SRCS}` is empty), it then either uses
`linkfile`, `Linkfile` or `${LINKFILE}`, if it exists, to get the objects
and replaces `.o` with `.s` to get the source files.
If an linkfile hasn't been found or specified, then it uses all `*.s` files.

Also, if an linkfile has been found/specified, then it also uses it to link
rather than to generate a new linkfile. Set `USE_CUSTOM_LINKFILE` to `NO` to
disable this. It also defines the link defines.

If `ASSEMBLE_NORMALLY` is `ON`, which is the default, then it assembles those
source files and links them together.

If `CHECK_FILE_EQUALS` is `ON`, which is the default if `ASSEMBLE_NORMALLY` is
`ON`, then it checks `${OUTPUT}` (`output.bin`) with `${EXPECTED}`
(`expected.bin`) if they're equal. If they don't match, the test fails.

### What you can do in `test.cmake`

Inside an `test.cmake` file, you can add:

* `set(ASSEMBLE_NORMALLY OFF)` to disable the test runner default stuff. Useful
  if you want to do custom stuff. It also disables the checking with the
  output binary with the expected output binary. To enable it again, use
  `set(CHECK_FILE_EQUALS ON)`.
* `setup()` to run the setup at that point, so you can set additional stuff
  after setup.
* `list(APPEND TEMP_FILES "one" "two")` to add additional files to be deleted
  after this test. By default, it keeps track of the `*.o` files and deletes
  them, and also the temp file(s) WLA creates, just in case.
* `set(OUTPUT "newoutputname.bin")` to set the output binary name to produce
  and also to check with the expected binary.
* `set(EXPECTED "expectedbinary.bin")` to set the expected binary to check with
  the output binary.
* `set(WLA_FLAGS -x)` to set the flags it should use for `wla-ARCH`. By default
  it's `-x`.
* `set(LINK_FLAGS -d)` to set the flags it should use for `wlalink`. By default
  it's `-d.`
* `set(DEFS key=value k=v)` to set defines for `wla-ARCH`.
* `set(LDEFS key=value k=v)` to set defines for `wlalink`. It's written to
  the linkfile.
* `set(SRCS one two three)` to set the source files. By default, see above text
  to see how it determines the default source files.
* `set(LINKFILE "linkfilename")` to set the linkfile it should use for parsing
  some stuff (source files, ...) and to use it when linking.
* `set(USE_CUSTOM_LINKFILE NO)` to not use the specefied or default linkfile,
  rather it generates its own one from the settings.

The file `test_common.cmake` is included in every test and serves as a small
collection of functions/macros to test stuff, like assembling and linking.
They're documented in the file and it should be pretty easy to understand.

