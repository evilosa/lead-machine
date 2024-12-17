# Atmega8

## Fuse high byte
```bash
7) RSTDISBL - Select if PC6 is I/O pin or RESET pin #1 (unprogrammed, PC6 is RESET-pin)
6) WDTON - Watchdog timer always on # 1 (unprogrammed, WDT enabled by WDTCR)
5) SPIEN - Enable Serial Program and Data Downloading # 0 (programmed, SPI prog. enabled)
4) CKOPT - Oscillator options # 1 (unprogrammed)
3) EESAVE - EEPROM memory is preserved through the Chip Erase # 1 (unprogrammed, EEPROM not preserved)
2) BOOTSZ1 - Select Boot Size (see Table 82 on page 213 for details) # 0 (programmed)
1) BOOTSZ0 - Select Boot Size (see Table 82 on page 213 for details) # 0 (programmed)
0) BOOTRST - Select Reset Vector - # 1 (unprogrammed)
```

## Fuse low byte
```bash
7) BODLEVEL - Brown out detector trigger level - # 1 (unprogrammed)
6) BODEN - Brown out detector enable #1 (unprogrammed, BOD disabled)
5) SUT1 - Select start-up time # 1 (unprogrammed)
4) SUT0 - Select start-up time # 0 (programmed)
3) CKSEL3 - Select Clock source # 0 (programmed)
2) CKSEL2 - Select Clock source # 0 (programmed)
1) CKSEL1 - Select Clock source # 0 (programmed)
0) CKSEL0 - Select Clock source # 1 (unprogrammed)
```

## Needed fuse values
hFuse = 0xD9 = 1101 1001
lFuse = 0xEF = 1110 1111 # all CKSELx = 1 to use external crystal 4-8Mhz

You can use next make commands to read/write fuses
```bash
make read_fuses # to read values to hfuse.hex and lfuse.hex files
make write_fuses # to write fuses to chip from hfuse.hex and lfuse.hex files
```