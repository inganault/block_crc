block_crc: block_crc.c
	cc -lz -O2 -o block_crc block_crc.c

.PHONY: clean
clean:
	rm block_crc
