# Thread safe C89 ring buffer

Software Library for creating thread safe C89 ring buffers.

author: Jay Convertino

date: 2016.12.01

license: MIT

## Release Versions
### Current
  Tag: release_v1.5.0

  - 1.5.0 - Major fix for element bug. For some reason I didn't divide the return.
            1.4.0 added a bug where len will be wrong if element size is not 1 for
            its fix.

### Past
  - 1.4.0 - Fix for read started before write and write ends blocking in one pass.
  - 1.3.0 - Added private blocking check method to the blocking read/write.
  - 1.2.0 - Code cleanup, added const.
  - 1.1.0 - Added timed wait_until to blocking read/write.

## Requirements
  - GCC
  - pthread library

## Building with CMAKE
### library only
  1. mkdir build
  2. cd build
  3. cmake ../
    - use -DBUILD_SHARED_LIBS=OFF option for static library.
    - use -DBUILD_SHARED_LIBS=ON option for shared library.
    - use -DBUILD_EXAMPLES=ON option for examples to be built as well.
  4. make

## Building with Makefile (OLD)
  - make libringbuffer.so for dynamic
  - make libringbuffer.a  for static
  - make exe for test applications
  - make for all

## Documentation
  - See doxygen generated document

## Example Code
  - See eg/src/ directory for examples.

### Currect Examples
  - file_cp = file copy example program
