# TX81Z Parameter Ranges

Based on the `CC_MAPPING` array in `src/main.cpp`, here is the breakdown of the parameters grouped by their maximum values (since `MIN` is uniformly `0` for all mapped params).

## 1. Range of 1 (Min: 0, Max: 1)
These parameters essentially act as toggles or two-state selections.

* **CC 0:** Poly/Mono mode
* **CC 13:** LFO Sync
* **CC 16:** Portamento Mode
* **CC 19:** Sustain (SYSEX version)
* **CC 20:** Portamento
* **CC 79:** OP4 Fixed Frequency Mode
* **CC 92:** OP3 Fixed Frequency Mode
* **CC 105:** OP2 Fixed Frequency Mode
* **CC 118:** OP1 Fixed Frequency Mode

## 2. Range of Less Than 20 (1 < Max < 20)
These parameters have a small step range.

* **CC 11:** Pitch Bend Range (Max 12)
* **CC 12:** LFO Wave (Max 3)
* **CC 14:** LFO PMS [Pitch Modulation Sensitivity] (Max 7)
* **CC 15:** LFO AMS [Amplitude Modulation Sensitivity] (Max 3)
* **CC 23, 44:** Reverb Rate (Max 7)
* **CC 26:** Algorithm (Max 7)
* **CC 27:** Feedback (Max 7)
* **CC 47, 50, 53, 56:** OP4/3/2/1 Fixed Frequency Range (Max 7)
* **CC 43:** Operator On/Off (Max 15)
* **CC 48, 51, 54, 57:** OP4/3/2/1 Fine Frequency (Max 15)
* **CC 49, 52, 55, 58:** OP4/3/2/1 EG Shift (Max 3)
* **CC 70, 83, 96, 109:** OP4/3/2/1 Release Rate / RR (Max 15)
* **CC 71, 84, 97, 110:** OP4/3/2/1 Decay 1 Level / D1L (Max 15)
* **CC 74, 87, 100, 113:** OP4/3/2/1 Detune (Max 6) 
* **CC 76, 89, 102, 115:** OP4/3/2/1 Rate Scaling (Max 3)
* **CC 77, 90, 103, 116:** OP4/3/2/1 Key Velocity Sensitivity (Max 7)
* **CC 78, 91, 104, 117:** OP4/3/2/1 Waveform (Max 7)

## 3. Range of More Than 20 (Max ≥ 20)
These parameters have wider granular values, commonly mapping to standard 0-99/127 setups.

* **Max 31:**
  * **CC 67, 80, 93, 106:** OP4/3/2/1 Attack Rate (AR) 
  * **CC 68, 81, 94, 107:** OP4/3/2/1 Decay 1 Rate (D1R)
  * **CC 69, 82, 95, 108:** OP4/3/2/1 Decay 2 Rate (D2R)

* **Max 48:**
  * **CC 1, 28:** Transpose 

* **Max 63:**
  * **CC 73, 86, 99, 112:** OP4/3/2/1 Frequency Coarse (CRS)

* **Max 99:**
  * **CC 3:** LFO Speed
  * **CC 6:** LFO Delay
  * **CC 8:** LFO PMD [Pitch Modulation Depth]
  * **CC 9:** LFO AMD [Amplitude Modulation Depth]
  * **CC 17:** Portamento Time
  * **CC 18:** FC Volume
  * **CC 21, 22:** Mod Wheel Pitch & Amplitude
  * **CC 24, 45:** FC Pitch
  * **CC 25, 46:** FC Amplitude
  * **CC 29, 30:** BC Pitch & Amplitude
  * **CC 31:** BC Pitch Bias
  * **CC 32:** BC EG Bias
  * **CC 72, 85, 98, 111:** OP4/3/2/1 Output Level
  * **CC 75, 88, 101, 114:** OP4/3/2/1 Level Scaling

* **Max 127:**
  * **CC 2:** Breath
  * **CC 4:** Foot
  * **CC 5:** Portamento (Continuous CC)
  * **CC 7:** Volume
  * **CC 10:** Pan
  * **CC 59-66:** Standard MIDI Controls (e.g., CC 64 Sustain, CC 66 Sostenuto)
  * **CC 119-127:** System Flags (All Notes Off, Reset Ctrls, Omni/Mono/Poly modes)

## 4. Special Ranges
These parameters have non-zero minimum boundaries.

* **CC 33-42:** Voice Name Characters 1-10 (Min: 32, Max: 127)
