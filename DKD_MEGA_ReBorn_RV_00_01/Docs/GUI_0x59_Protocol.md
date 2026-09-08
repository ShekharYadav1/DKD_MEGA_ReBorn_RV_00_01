


# 0x6A Command Protocol - RGBC Auto-Zero Drift %

## Overview

Command `0x6A` is a **query command** sent by the GUI to a specific element board. The board replies with the **RGBC drift percentage** between the **OLD (hard-coded saved) `stan_0` standard** and the **NEW `stan_0` standard** (which was captured during the last auto-zero). This lets the GUI monitor how much the sensor's blank reference has drifted from its factory-default value after auto-zeroing.

The drift is reported per channel as a **signed 16-bit integer scaled by 100**.

## Concept

After each auto-zero, the board overwrites its runtime standard 0 (`opt_std_vars.stan_0_*`) with the live sensor reading, but the hard-coded saved value (`hrd_std_vars.stan_0_*`) stays unchanged.

```
drift% = ((NEW stan_0 - OLD stan_0) / OLD stan_0) * 100
```

- **NEW** = runtime standard (live reading captured during auto-zero), e.g. Red = 18774
- **OLD** = hard-coded saved standard, e.g. Red = 19509
- **Drift** = ((18774 - 19509) / 19509) * 100 = **-3.77%**  →  transmitted as **-377**

## Request (GUI -> MCU)

Payload bytes are not interpreted by the board for this command, so they can be zero-filled.

```
AA 99 0C FF <element_addr> 6A 00 00 00 00 00 00 99 AA 0D 0A
```

| Byte Index | Value | Meaning |
|---|---|---|
| 0 | 0xAA | Start ID 1 |
| 1 | 0x99 | Start ID 2 |
| 2 | 0x0C | LEN = 12 (total frame length) |
| 3 | 0xFF | SRC = GUI (MASTER_SYS) |
| 4 | <element_addr> | DEST = element board (see address map) |
| 5 | 0x6A | CMD |
| 6..11 | 0x00 | Payload (ignored), CRC1, CRC2 |
| 12 | 0x99 | End ID 1 |
| 13 | 0xAA | End ID 2 |
| 14 | 0x0D | CR |
| 15 | 0x0A | LF |

## Response (MCU -> GUI) - 24 Bytes

```
AA 99 18 <element_addr> FF 6A <statM> <statL> 00 00 <RH> <RL> <GH> <GL> <BH> <BL> <CH> <CL> <CRC1> <CRC2> 99 AA 0D 0A
```

| Byte Index | Size | Field | Meaning |
|---|---|---|---|
| 0 | 1 | 0xAA | Start ID 1 |
| 1 | 1 | 0x99 | Start ID 2 |
| 2 | 1 | 0x18 | LEN = 24 (total frame length) |
| 3 | 1 | SRC | Element address (the board that replied) |
| 4 | 1 | DEST | GUI address (0xFF) |
| 5 | 1 | 0x6A | Command echo |
| 6 | 1 | StatM | Status byte M |
| 7 | 1 | StatL | Status byte L (bit 0 set = fixed/ack) |
| 8 | 1 | 0x00 | Reserved |
| 9 | 1 | 0x00 | Reserved |
| 10..11 | 2 | Red drift | **signed** 16-bit big-endian, ×100 |
| 12..13 | 2 | Green drift | **signed** 16-bit big-endian, ×100 |
| 14..15 | 2 | Blue drift | **signed** 16-bit big-endian, ×100 |
| 16..17 | 2 | Clear drift | **signed** 16-bit big-endian, ×100 |
| 18 | 1 | CRC1 | Checksum byte 1 |
| 19 | 1 | CRC2 | Checksum byte 2 |
| 20..23 | 4 | End | `99 AA 0D 0A` |

**IMPORTANT:** The 4 drift values are **signed 16-bit** (`short`/`int16`). Decode as signed, NOT unsigned, or negative drift will be misread as a large positive number.

**Worked example (POTASSIUM board, after auto-zero to 18774/18788/20251/53110):**

```
AA 99 18 70 FF 6A 00 01 00 00 FE 87 FE 3E FE 76 FE 80 CRC1 CRC2 99 AA 0D 0A
                 |  |  |  |  |  |       || || || || || ||
                 |  |  |  |  |  |       || || || || || ||-- Clear = 0xFE80 = -384 = -3.84%
                 |  |  |  |  |  |       || || || || ++----  Blue  = 0xFE76 = -394 = -3.94%
                 |  |  |  |  |  |       || || +++--------   Green = 0xFE3E = -450 = -4.50%
                 |  |  |  |  |  |       ++++--------------   Red   = 0xFE87 = -377 = -3.77%
                 |  |  |  |  +-- reserved (0x00 0x00)
                 |  |  |  +----- StatL (0x01 = ack bit)
                 |  |  +-------- StatM
                 |  +----------- CMD echo = 0x6A
                 +-------------- DEST = GUI = 0xFF
                 +--------------- SRC = element = 0x70 (POTASSIUM)
```

## Reference Table - Potassium (element 0x70)

| Channel | OLD stan_0 | NEW stan_0 | Drift int16 | Hex (big-endian) | Drift % |
|---|---|---|---|---|---|
| Red | 19509 | 18774 | -377 | `FE 87` | **-3.77%** |
| Green | 19674 | 18788 | -450 | `FE 3E` | **-4.50%** |
| Blue | 21081 | 20251 | -394 | `FE 76` | **-3.94%** |
| Clear | 55231 | 53110 | -384 | `FE 80` | **-3.84%** |

## App Developer Instructions

1. **Parse order:** validate start IDs (`AA 99`), check `LEN` (0x18), verify end markers (`99 AA 0D 0A`) and CRC before trusting payload.
2. **Identify the element** using **SRC byte [3]** with the address map.
3. **Decode each drift value as SIGNED 16-bit big-endian:**
   ```c
   short redDrift   = (short)((byte10 << 8) | byte11);
   short greenDrift = (short)((byte12 << 8) | byte13);
   short blueDrift  = (short)((byte14 << 8) | byte15);
   short clearDrift = (short)((byte16 << 8) | byte17);

   double redPct   = redDrift   / 100.0;
   double greenPct = greenDrift / 100.0;
   double bluePct  = blueDrift  / 100.0;
   double clearPct = clearDrift / 100.0;
   ```
4. **Sign meaning:** negative drift = NEW standard is **lower** than the factory-saved OLD standard (sensor reading dropped). A negative percentage is normal and expected after auto-zero.
5. **Display suggestion:** show the signed value, e.g. `-3.77%`. Optionally color the value (e.g. green within ±5%, yellow/orange between ±5% and ±10%, red beyond ±10%) according to your tolerance policy.
6. **CRC:** use the same OR-based algorithm as 0x59 (see above). The board fills CRC1/CRC2 automatically.
7. **Common pitfalls:**
   - Decoding drift as **unsigned** (0xFE87 would read 65145 instead of -377).
   - Little-endian byte order (values are big-endian: hi byte first).
   - Treating the value directly as percentage (must divide by 100).
   - Not checking element via SRC address.