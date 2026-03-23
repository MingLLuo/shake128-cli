# SHAKE128 CLI

## Compiling Programs
- All program can be compiled using `gcc` or `clang` on Linux/MacOS.
- Using `Makefile` is recommended(provide friendly interface for compiling programs).
- To compile the programs, just run `make` in the terminal.
- `./shake128` can be used to run the SHAKE128 program.

## Version History
- 1st Version: Basic implementation, with char array as state representation.
  - Advantages: 
    1. Simple to implement. 
    2. Easy to debug because of byte-level representation. 
    3. Each `memcpy` won't change original state in each calling, which is helpful for debugging using gdb.
    4. Define plenty of macros for sizes, offsets, etc., making code more readable.
  - Disadvantages: 
    1. Slow performance due to memory allocation, cause cache misses and extra memory operations.
    2. Bitwise operations need careful handling of byte boundaries. Like ROL across multiple bytes. If forget the risk of modulo operation, bugs may occur.(like `-1%5` will be `-1` in C/C++).
    3. Ugly code😇
- 2st Version: Improved implementation, with uint64_t array as state representation.(Take the `Step by Step` section's suggestion)
  - Advantages:
    1. Faster performance, less memory operations, better cache performance, operation is done locally, Create `RC` table for round constants to avoid recomputation.
    2. Simpler bitwise operations, no need to care about byte boundaries.
    3. Better error handling, Cleaner code.
  - Disadvantages:
    1. Parameters are less flexible, hard to support flexible state size. 

## References
- [FIPS 202 Standard](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf)
- [Keccak Team's Official Website](https://keccak.team/)
- [Golang's old implementation of SHAKE128(BACKUP)](https://github.com/MingLLuo/OW-ChCCA-KEM/tree/main/pkg/sha3)
