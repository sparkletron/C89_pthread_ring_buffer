# Thread safe C89 ring buffer

Software Library for creating thread safe C89 ring buffers.

author: Jay Convertino

date: 2016.12.01

license: MIT

## Release Versions
### Current
  - release_v1.4.0, Fix for read started before write and write ends blocking in one pass.

### Past
  - 1.3 - Added private blocking check method to the blocking read/write.
  - 1.2 - Code cleanup, added const.
  - 1.1 - Added timed wait_until to blocking read/write.

## Requirements
  - GCC
  - pthread library

## Building
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
