# block_crc

Calculate CRC32 for each 1MB blocks of file, outputs to text (stdout).

Useful for comparing large block device.

## Build

- Install zlib and C compiler
- `make`

## Usage
```
./block_crc input_file [start_offset]

start_offset unit is block (1 MB)
```

### Example: Comparing block device
```sh
block_crc /dev/sda1 > sda1_crc.txt
block_crc /dev/sdb1 > sdb1_crc.txt
diff sda1_crc.txt sdb1_crc.txt
```
