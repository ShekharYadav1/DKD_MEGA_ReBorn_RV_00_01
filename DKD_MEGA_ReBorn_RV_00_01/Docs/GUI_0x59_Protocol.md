# 0x59 Command Protocol - Firmware / Calibration Signature Check

## Overview

Command `0x59` is a **query command** sent by the GUI (master) to a specific element board (slave). The board replies with its hard-coded `stan_0` (0 ppm blank) RGBC values. These values act as a **firmware version / calibration signature** for that element, letting the GUI verify which code build / calibration is running on the board.

Element identification is done via the **SRC address byte** in the response, NOT via the RGBC values (multiple elements share identical `stan_0` values).

## General Frame Format

All frames (request and response) use the same structure:

```
AA 99 LEN SRC DEST CMD PAYLOAD... CRC1 CRC2 99 AA 0D 0A
```

| Byte Index | Field | Meaning |
|---|---|---|
| 0 | Start ID 1 | 0xAA |
| 1 | Start ID 2 | 0x99 |
| 2 | LEN | Total frame length in bytes (includes itself and everything after) |
| 3 | SRC | Source address |
| 4 | DEST | Destination address |
| 5 | CMD | Command type |
| 6..N-5 | PAYLOAD | Command-specific data |
| N-4 | CRC1 | Checksum byte 1 |
| N-3 | CRC2 | Checksum byte 2 |
| N-2 | End ID 1 | 0x99 |
| N-1 | End ID 2 | 0xAA |
| N | End ID 3 | 0x0D (CR) |
| N+1 | End ID 4 | 0x0A (LF) |

## Request (GUI -> MCU)

The GUI sends the request to the desired element's address. Payload bytes are not interpreted by the board for this command, so they can be zero-filled.

```
AA 99 0C FF <element_addr> 59 00 00 00 00 00 00 99 AA 0D 0A
```

| Byte Index | Value | Meaning |
|---|---|---|
| 0 | 0xAA | Start ID 1 |
| 1 | 0x99 | Start ID 2 |
| 2 | 0x0C | LEN = 12 (total frame length) |
| 3 | 0xFF | SRC = GUI (MASTER_SYS) |
| 4 | <element_addr> | DEST = element board being queried (see address map) |
| 5 | 0x59 | CMD |
| 6..11 | 0x00 | Payload (ignored), CRC1, CRC2 |
| 12 | 0x99 | End ID 1 |
| 13 | 0xAA | End ID 2 |
| 14 | 0x0D | CR |
| 15 | 0x0A | LF |

## Response (MCU -> GUI) - 24 Bytes

```
AA 99 18 <element_addr> FF 59 <statM> <statL> 00 00 <R-hi> <R-lo> <G-hi> <G-lo> <B-hi> <B-lo> <C-hi> <C-lo> <CRC1> <CRC2> 99 AA 0D 0A
```

| Byte Index | Size | Field | Meaning |
|---|---|---|---|
| 0 | 1 | 0xAA | Start ID 1 |
| 1 | 1 | 0x99 | Start ID 2 |
| 2 | 1 | 0x18 | LEN = 24 (total frame length) |
| 3 | 1 | SRC | Element address (the board that replied) |
| 4 | 1 | DEST | GUI address (0xFF) |
| 5 | 1 | 0x59 | Command echo |
| 6 | 1 | StatM | Status byte M |
| 7 | 1 | StatL | Status byte L (bit 0 set = fixed/ack) |
| 8 | 1 | 0x00 | Reserved |
| 9 | 1 | 0x00 | Reserved |
| 10..11 | 2 | Red | `stan_0` Red value, big-endian (hi, lo) |
| 12..13 | 2 | Green | `stan_0` Green value, big-endian (hi, lo) |
| 14..15 | 2 | Blue | `stan_0` Blue value, big-endian (hi, lo) |
| 16..17 | 2 | Clear | `stan_0` Clear value, big-endian (hi, lo) |
| 18 | 1 | CRC1 | Checksum byte 1 |
| 19 | 1 | CRC2 | Checksum byte 2 |
| 20..23 | 4 | End | `99 AA 0D 0A` |

**Worked example (POTASSIUM board):**

```
AA 99 18 70 FF 59 00 01 00 00 4C 35 4C DA 52 59 D7 BF CRC1 CRC2 99 AA 0D 0A
                 |  |  |  |  |  |  |  |  |  |  |       || ||
                 |  |  |  |  |  |  |  |  |  |  |       || ||-- Clear = 0xD7BF = 55231
                 |  |  |  |  |  |  |  |  |  |  +-------+------  Blue  = 0x5259 = 21081
                 |  |  |  |  |  |  |  |  |  +-------------- Green = 0x4CDA = 19674
                 |  |  |  |  |  |  |  |  +----------------- Red   = 0x4C35 = 19509
                 |  |  |  |  |  +-- reserved (0x00 0x00)
                 |  |  |  |  +----- StatL (0x01 = ack bit)
                 |  |  |  +-------- StatM
                 |  |  +----------- CMD echo = 0x59
                 |  +-------------- DEST = GUI = 0xFF
                 +----------------- SRC = element = 0x70 (POTASSIUM)
```

## Address Map

| Address | Element |
|---|---|
| 0x10 | MAGNESIUM |
| 0x20 | IRON |
| 0x30 | COPPER |
| 0x40 | ZINC |
| 0x50 | BORON |
| 0x60 | SULPHUR |
| 0x70 | POTASSIUM |
| 0x80 | PHOSPHORUS |
| 0x90 | NITROGEN |
| 0xA0 | ORGANIC_CARBON |
| 0xFF | MASTER (GUI) |

## CRC Algorithm

`n = (LEN - 8) / 2` pairs

**Example for LEN = 0x18 (24):** `n = (24 - 8) / 2 = 8`

- **CRC1** = bitwise-OR of bytes at indices `(i * 2) + 2` for `i = 0..n-1`
  - For n=8: indices `2, 4, 6, 8, 10, 12, 14, 16`
  - = LEN | DEST | StatM | reserved0 | R-hi | G-hi | B-hi | C-hi
- **CRC2** = bitwise-OR of bytes at indices `(i * 2) + 3` for `i = 0..n-1`
  - For n=8: indices `3, 5, 7, 9, 11, 13, 15, 17`
  - = SRC | CMD | StatL | reserved1 | R-lo | YG-lo | B-lo | C-lo

```c
// Reference (as implemented in firmware check_crc())
uint8_t n = (LEN - 8) / 2;
uint8_t CRC1 = 0;
for (uint8_t i = 0; i < n; i++) {
    CRC1 |= frame[(i * 2) + 2];
}
uint8_t CRC2 = 0;
for (uint8_t i = 0; i < n; i++) {
    CRC2 |= frame[(i * 2) + 3];
}
// CRC1 -> frame[18], CRC2 -> frame[19]   (for LEN = 0x18)
```

**Note:** CRC is a simple bitwise-OR checksum, NOT a standard CRC-8. Implement it as-is.

## Element `stan_0` Signature Values (current)

`stan_0` = 0 ppm (blank) RGBC reference for the element, stored in `save_sys_info.bk_var.hrd_std_vars` (primary) at compile time via `init_hrd_strd()`.

| Address | Element | Red | Green | Blue | Clear |
|---|---|---|---|---|---|
| 0x10 | MAGNESIUM | 18270 | 14522 | 16275 | 51293 |
| 0x20 | IRON | 18270 | 14522 | 16275 | 51293 |
| 0x30 | COPPER | 18270 | 14522 | 16275 | 51293 |
| 0x40 | ZINC | 18270 | 14522 | 16275 | 51293 |
| 0x50 | BORON | 18270 | 14522 | 16275 | 51293 |
| 0x60 | SULPHUR | 18994 | 19019 | 21154 | 53306 |
| 0x70 | POTASSIUM | 19509 | 19674 | 21081 | 55231 |
| 0x80 | PHOSPHORUS | 16604 | 15462 | 18440 | 47486 |
| 0x90 | NITROGEN | 17073 | 16682 | 17398 | 48515 |
| 0xA0 | ORGANIC_CARBON | 18270 | 14522 | 16275 | 51293 |

When the firmware version/calibration changes, these values are updated, so a mismatch between expected and received values flags a different code build.

## App Developer Instructions

1. **Parse order:** validate start IDs (`AA 99`), check `LEN` (0x18), verify end markers (`99 AA 0D 0A`) and CRC before trusting payload.
2. **Identify the element** using **SRC byte [3]** with the address map. Do NOT identify the element from the RGBC values (6 elements share the same `stan_0` set).
3. **Reconstruct RGBC values** as big-endian: `value = (hi << 8) | lo`.
4. **Verify the signature:** compare the 4 values against the expected set for that element/version. Different values = different code/calibration build.
5. **CRC:** use the OR-based algorithm above (not standard CRC).
6. **Common pitfalls:**
   - Treating RGBC values as element identifiers.
   - Little-endian byte order (values are big-endian: hi byte first).
   - Expecting a standard CRC-8 checksum.
   - Ignoring the SRC address and trusting only CMD/DEST.