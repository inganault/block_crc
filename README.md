# block_crc

Calculate crc32 for each 1MB blocks of file, outputs to text (stdout).

Useful for comparing large block device.

## Build

- Install zlib and C compiler
- `make`

## Usage
```
./block_crc input_file [start_offset]

start_offset unit is block (1 MB)
```
