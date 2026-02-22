---
trigger: always_on
---



# Akai Force Project Structure & Rules

### Source of Truth Files
- **MIDI CC Mapping**: `@/src/main.cpp` (Contains the `CC_MAPPING` array)
- **Akai Force Program**: `@/AddOns/txSex/TX81z-txsyx.xpm` (Contains the `<ParameterNames>` XML)

### 0-Indexing Rule
- All `<Parameter Index="...">` values in the `.xpm` must remain **0-indexed** (0 to 127).

### 18-Character Display Rule
- The `Name` attribute of every `<Parameter>` in the `.xpm` must be **18 characters or less**, including the range suffix.
