/*******************************************************************
TX81z CC to Sysex Translator for Akai Force/Raspberry PI forked from dxsex
Original Author: Amit Talwar https://www.amitszone.com
Github: https://github.com/intelliriffer
Fork designer Allan Klinbail (AI assistance provided)
*****************************************************************


SYSTEM EXCLUSIVE DATA FORMAT
The TX81Z has three types of System Exclusive message; Parameter Change messages, Bulk Data messages and
Dump Request messages.

PARAMETER CHANGE MESSAGES
These messages change the value of a parameter in TX81Z memory. There are
8 subgroups of Parameter Changes; VCED, ACED, PCED, Remote Switch,
Micro Tuning, Program Change, Effect data and System data.

VCED, ACED, PCED and Remote Switch parameter change messages have the
following format.
F0h        Exclusive
43h        I.D. number (Yamaha)
1nh        Basic receive channel
0ggggghh   ggggg = Group number, hh = Subgroup number
0ppppppp   ppppppp = Parameter number
0ddddddd   ddddddd = Data
F7h        End Of Exclusive

* VCED (Voice parameters compatible with DX21/27/100)
  ggggg = 00100 (4), hh = 10 (2)
  See p.71 for parameter numbers and data.

* ACED (Additional voice parameters for TX81Z)
  ggggg = 00100 (4), hh = 11 (3)
  See p.73 for parameter numbers and data.

* PCED (Performance parameters)
  ggggg = 00100 (4), hh = 11 (3)
  See p.74 for parameter numbers and data.

* Remote Switch (The same effect as pressing a switch on the TX81Z front
  panel, i.e., 'remote control'.)
  ggggg = 00100 (4), hh = 11 (3), ddddddd = 0 (off), 7F (on)
  See p.75 for switch numbers.

System Parameter Change (basic receive channel settings, etc.) and Effect
Parameter Change (data for delay, pan and chord) messages have the following
format.
F0h        Exclusive
43h        I.D. number (Yamaha)
1nh        Basic receive channel
0ggggghh   ggggg = 00100 (4), hh = 00 (0)
0ppppppp   ppppppp = 1111011 (123) = System Parameter
           ppppppp = 1111100 (124) = Effect Parameter
0kkkkkkk   kkkkkkk = Parameter number

Micro Tune parameter change messages have the following format.
F0h        Exclusive
43h        I.D. number (Yamaha)
1nh        Basic receive channel
0ggggghh   ggggg = 00100 (4), hh = 00
0ppppppp   ppppppp = 1111101 (125) OCT
           ppppppp = 1111110 (126) FULL
0kkkkkkk   kkkkkkk = key number
0hhhhhhh   hhhhhhh = note C#-1 to C7 (13-108)
           = data fine tuning 0 to +31, -31 to -1 (0-32, 33-63)
F7h        End Of Exclusive

Program Change Table parameter change messages have the following format.
The data is 0-184d, indicating the TX81Z memory to be selected in response to
the incoming program change number. 0-31 (I1-I32), 32-63 (A1-A32), 64-95
(B1-B32), 96-127 (C1-C32), 128-160 (D1-D32), 161-184 (PF1-PF24)

F0h        Exclusive
43h        I.D. number (Yamaha)
1nh        Basic receive channel
0ggggghh   ggggg = 00100 (4), hh = 00
0ppppppp   ppppppp = 1111111 (127)
0kkkkkkk   kkkkkkk = program change number
0hhhhhhh   hhhhhhh = data (high)
0lllllll   lllllll = data (low)
F7h        End Of Exclusive
*/
/* Source: TX81Z Owner’s Manual PDF, pp.68–69 (Parameter Change Messages section). [1](https://usa.yamaha.com/files/download/other_assets/9/316769/TX81ZE.pdf)

============================================================
 TX81Z — PARAMETER TABLES (Pages 71–73)
 Source: Yamaha TX81Z Owner’s Manual PDF
============================================================

------------------------------------------------------------
VOICE EDIT PARAMETERS (VCED)
------------------------------------------------------------
Parameter number | Parameter                       | LCD          | Data
-----------------|----------------------------------|--------------|-------------------------
0                | Attack Rate                      | AR           | 0–31
1                | Decay 1 Rate                     | D1R          | 0–31
2                | Decay 2 Rate                     | D2R          | 0–31
3                | Release Rate                     | RR           | 1–15
4                | Decay 1 Level                    | D1L          | 0–15
5                | Level Scaling                    | LS           | 0–99
6                | Rate Scaling                     | RS           | 0–3
7                | EG Bias Sensitivity              | EBS          | 0–7
8                | Amplitude Modulation Enable      | AME          | 0–1
9                | Key Velocity Sensitivity         | KVS          | 0–7
10               | Operator Output Level            | OUT          | 0–99
11               | Frequency (coarse)               | CRS          | 0–63
12               | Detune                            | DET          | 0–6 (3=center)
13–25            | Operator 3 parameters            | (same order) | (same ranges)
26–38            | Operator 2 parameters            | (same order) | (same ranges)
39–51            | Operator 1 parameters            | (same order) | (same ranges)
52               | Algorithm                         | ALG          | 0–7
53               | Feedback                          | Feedback     | 0–7
54               | LFO Speed                         | Speed        | 0–99
55               | LFO Delay                         | Delay        | 0–99
56               | Pitch Modulation Depth            | P Mod Depth  | 0–99
57               | Amp Modulation Depth              | A Mod Depth  | 0–99
58               | LFO Sync                          | Sync         | 0–1
59               | LFO Wave                          | Wave         | 0–3
60               | Pitch Mod Sensitivity             | P Mod Sens   | 0–7
61               | Amplitude Mod Sensitivity         | AMS          | 0–3
62               | Transpose                         | Middle C =   | 0–48 (24=center)
63               | Poly/Mono                         | Poly Mode    | 0–1
64               | Pitch Bend Range                  | P Bend Range | 0–12
65               | Portamento Mode                   | Full Time    | 0–1
66               | Portamento Time                   | Porta Time   | 0–99
67               | Foot Control Volume               | FC Volume    | 0–99
68               | Sustain                            | (none)       | 0–1
69               | Portamento                         | (none)       | 0–1
70               | Chorus (not used)                  | (none)       | Always 0
71               | Mod Wheel Pitch                   | MW Pitch     | 0–99
72               | Mod Wheel Amplitude               | MW Ampl      | 0–99
73               | Breath Control Pitch              | BC Pitch     | 0–99
74               | Breath Control Amplitude          | BC Ampl      | 0–99
75               | Breath Control Pitch Bias         | BC PitchBias | 0–99 (50=center)
76               | Breath Control EG Bias            | BC EG Bias   | 0–99
77–86            | Voice Name Characters 1–10        | ASCII        | 32–127
93               | Operators 4–1 On/Off (bit mask)   | —            | 0–15 (1=on)
(Parameters 87–92 are unused)

------------------------------------------------------------
ADDITIONAL VOICE EDIT PARAMETERS (ACED)
------------------------------------------------------------
Parameter number | Parameter                  | LCD    | Data
-----------------|-----------------------------|--------|----------------------------
0                | Fixed Frequency Mode        | FIX    | 0–1
1                | Fixed Frequency Range       |        | 0–7  (0=250 Hz … 7=32 kHz)
2                | Fine Frequency (fixed)      | FIN    | 0–15
3                | Operator Waveform           | OSW    | 0–7
4                | EG Shift                    | SHFT   | 0–3 (0=96dB, 1=48dB, 2=24dB, 3=12dB)
5–9              | Operator 3 extra params     | —      | same order/type as op4
10–14            | Operator 2 extra params     | —      | same order/type
15–19            | Operator 1 extra params     | —      | same order/type
20               | Reverb Rate                 | REV    | 0–7 (0=off, 7=fast)
21               | FC Pitch                    | FC Pitch | 0–99
22               | FC Amplitude                | FC Amplitude | 0–99

------------------------------------------------------------
PERFORMANCE EDIT PARAMETERS (PCED)
------------------------------------------------------------
Parameter number | Parameter               | LCD          | Data
-----------------|--------------------------|--------------|-------------------------
0                | Maximum Notes            | MAX NOTES    | 0–8
1                | Voice Number MSB         | —            | (0–127 encoded)
2                | Voice Number LSB         | 101–D32      | 0–127
3                | Receive Channel          | RECEIVE CH   | 0–16 (16=omni)
4                | Low Note Limit           | LIMIT/L      | 0–127 (C–2..G8)
5                | High Note Limit          | LIMIT/H      | 0–127 (C–2..G8)
6                | Instrument Detune        | INST DETUNE  | 0–14 (7=center)
7                | Note Shift               | NOTE SHIFT   | 0–48 (24=center)
8                | Volume                   | VOL          | 0–99
9                | Output Assign            | OUT ASSIGN   | 0–3  (0=off, 1=I, 2=II, 3=I+II)
10               | LFO Select               | LFO SELECT   | 0–3  (0=off, 1=inst1, 2=inst2, 3=vib)
11               | Micro Tune Enable        | —            | 0–1
12–23            | Instrument 2 parameters  | —            | Same structure
24–35            | Instrument 3 parameters  | —            | Same
36–47            | Instrument 4 parameters  | —            | Same
48–59            | Instrument 5 parameters  | —            | Same
60–71            | Instrument 6 parameters  | —            | Same
72–83            | Instrument 7 parameters  | —            | Same
84–95            | Instrument 8 parameters  | —            | Same
96               | Micro Tune Table          | MICTUN       | 0–12 (0=Oct, 1=Full, 2–12=presets)
97               | Assign Mode               | ASMODE       | 0–1 (0=norm, 1=altr)
98               | Effect Select             | EFSEL        | 0–3 (off/Delay/Pan/Chord)
99               | Key (for microtuning)     | KEY          | 0–11 (C–B)
100–109          | Performance Name chars 1–10 | ASCII      | 32–127

------------------------------------------------------------
REMOTE SWITCH PARAMETERS
------------------------------------------------------------
Parameter number | Parameter
-----------------|---------------------------
64               | POWER ON (reset)
65               | STORE
66               | UTILITY
67               | EDIT
68               | PLAY
69               | PARAMETER -1
70               | PARAMETER +1
71               | DATA ENTRY -1
72               | DATA ENTRY +1
73               | MASTER VOLUME -1
74               | MASTER VOLUME +1
75               | CURSOR

(Data: 0 = switch off, 127 = switch on)
*/
#include <map>
#include "RtMidi.h"
#include <chrono>
#include <csignal>
#include <ctime>
#include <sys/time.h>
#include <unistd.h>
const unsigned char nouts = 16;
using namespace std;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::system_clock;

const string PORT_PREFIX = "TX";
void onMIDI(double deltatime, std::vector<unsigned char>* message, void* /*userData*/);
int limit(int val, int min, int max);
unsigned char validCC[14] = { 1, 2, 7, 10, 64, 66, 120, 121, 122, 123, 124, 125, 126, 127 };
void print();
void cleanup();
void listInports();
void initHWPORT();
void signalHandler(int signum);
string oPORTNAME = "";
bool HW_EXISTS = false;
void listOutPorts();
long long getSecs();
int getOutPort(std::string str);
int getInPort(std::string str);
long long nextCheck = 0;
void sendMessage(vector<unsigned char>* message);

void updateAlgos(int algo);
bool isSet(int n, int k); // bit checker
vector<unsigned char> BASE_SYX { 0xF0, 0x43, 0x10, 0, 0, 0, 0xF7 };

// TX-81Z Algorithm Structure:
// Algo 1: 4->3->2->1  (only OP1 is carrier)           = 0b0001 = 1
// Algo 2: (4+3)->2->1 (only OP1 is carrier)           = 0b0001 = 1
// Algo 3: 4->(3+2)->1 (only OP1 is carrier)           = 0b0001 = 1
// Algo 4: (4->3)+(2->1) (OP1 and OP3 are carriers)    = 0b0101 = 5
// Algo 5: (4->2)+(4->1)+(3->1) (OP1 is carrier)       = 0b0001 = 1
// Algo 6: (4->2)+(3->2)+(2->1) (only OP1 is carrier)  = 0b0001 = 1
// Algo 7: 4+3+2->1 (only OP1 is carrier)              = 0b0001 = 1
// Algo 8: 4+3+2+1 (all are carriers)                  = 0b1111 = 15
const int ALGOS[8] = {
    1,    // Algorithm 0 (1): OP1 carrier
    1,    // Algorithm 1 (2): OP1 carrier
    1,    // Algorithm 2 (3): OP1 carrier
    5,    // Algorithm 3 (4): OP1 and OP3 carriers
    1,    // Algorithm 4 (5): OP1 carrier
    1,    // Algorithm 5 (6): OP1 carrier
    1,    // Algorithm 6 (7): OP1 carrier
    15    // Algorithm 7 (8): all 4 operators are carriers
};
enum BPOS {
    GROUP = 3,
    PARAMETER = 4,
    DATA = 5

};
enum CCTYPES {
    SYSTEM,
    SYSEX,
    SKIP,
    CC,
    MACRO
};

struct CC_MAPPING {
    //  int x = 0;
    CC_MAPPING(CCTYPES TYPE, int CC, int MIN, int MAX, int GROUP, int PARAMETER)
        : TYPE(TYPE)
        , CC(CC)
        , MIN(MIN)
        , MAX(MAX)
        , GROUP(GROUP)
        , PARAMETER(PARAMETER) { };
    CCTYPES TYPE = SKIP;
    int CC = 0;
    int MIN = 0;
    int MAX = 99;
    int GROUP = 0;
    int PARAMETER = 0;
};
/*vector<CC_MAPPING> CARRIERS;
vector<CC_MAPPING> MODULATORS;*/

struct ENVS {
    vector<int> CARRIERS;
    vector<int> LCARRIERS;
    vector<int> MODULATORS;
    vector<int> LMODULATORS;
};
ENVS ATTACK, DECAY, SUSTAIN, RELEASE;

CC_MAPPING MAP[128] = {
    // Global/Voice Parameters (CC 0-27) - GROUP values: 18=VCED, 19=ACED
    {SYSEX, 0, 0, 1, 18, 63},     // 0  Poly Mono mode
    {SYSEX, 1, 0, 48, 18, 62},    // 1  Transpose
    {CC, 2, 0, 127, 0, 0},        // 2  Breath
    {SYSEX, 3, 0, 99, 18, 54},    // 3  LFO SPEED
    {CC, 4, 0, 127, 0, 0},        // 4  Foot
    {CC, 5, 0, 127, 0, 0},        // 5  Portamento
    {SYSEX, 6, 0, 99, 18, 55},    // 6  LFO DELAY
    {CC, 7, 0, 127, 0, 0},        // 7  Volume
    {SYSEX, 8, 0, 99, 18, 56},    // 8  LFO PMD
    {SYSEX, 9, 0, 99, 18, 57},    // 9  LFO AMD
    {CC, 10, 0, 127, 0, 0},       // 10 PAN
    {SYSEX, 11, 0, 12, 18, 64},   // 11 Pitch Bend Range
    {SYSEX, 12, 0, 3, 18, 59},    // 12 LFO WAVE
    {SYSEX, 13, 0, 1, 18, 58},    // 13 LFO Sync
    {SYSEX, 14, 0, 7, 18, 60},    // 14 LFO PMS
    {SYSEX, 15, 0, 3, 18, 61},    // 15 LFO AMS
    {SYSEX, 16, 0, 1, 18, 65},    // 16 Portamento Mode
    {SYSEX, 17, 0, 99, 18, 66},   // 17 Portamento Time
    {SYSEX, 18, 0, 99, 18, 67},   // 18 FC Volume
    {SYSEX, 19, 0, 1, 18, 68},    // 19 Sustain
    {SYSEX, 20, 0, 99, 18, 69},   // 20 Portamento
    {SYSEX, 21, 0, 99, 18, 71},   // 21 Mod Wheel Pitch
    {SYSEX, 22, 0, 99, 18, 72},   // 22 Mod Wheel Amplitude
    {SYSEX, 23, 0, 7, 19, 20},    // 23 Reverb Rate - ACED param
    {SYSEX, 24, 0, 99, 19, 21},   // 24 FC Pitch - ACED param
    {SYSEX, 25, 0, 99, 19, 22},   // 25 FC Amplitude - ACED param
    {SYSEX, 26, 0, 7, 18, 52},    // 26 Algorithm
    {SYSEX, 27, 0, 7, 18, 53},    // 27 Feedback

    // Reserved for future global parameters (CC 28-59)
    {SKIP, 28, 0, 127, 0, 0},
    {SKIP, 29, 0, 127, 0, 0},
    {SKIP, 30, 0, 127, 0, 0},
    {SKIP, 31, 0, 127, 0, 0},
    {SKIP, 32, 0, 127, 0, 0},
    {SKIP, 33, 0, 127, 0, 0},
    {SKIP, 34, 0, 127, 0, 0},
    {SKIP, 35, 0, 127, 0, 0},
    {SKIP, 36, 0, 127, 0, 0},
    {SKIP, 37, 0, 127, 0, 0},
    {SKIP, 38, 0, 127, 0, 0},
    {SKIP, 39, 0, 127, 0, 0},
    {SKIP, 40, 0, 127, 0, 0},
    {SKIP, 41, 0, 127, 0, 0},
    {SKIP, 42, 0, 127, 0, 0},
    {SKIP, 43, 0, 127, 0, 0},
    {SKIP, 44, 0, 127, 0, 0},
    {SKIP, 45, 0, 127, 0, 0},
    {SKIP, 46, 0, 127, 0, 0},
    {SKIP, 47, 0, 127, 0, 0},
    {SKIP, 48, 0, 127, 0, 0},
    {SKIP, 49, 0, 127, 0, 0},
    {SKIP, 50, 0, 127, 0, 0},
    {SKIP, 51, 0, 127, 0, 0},
    {SKIP, 52, 0, 127, 0, 0},
    {SKIP, 53, 0, 127, 0, 0},
    {SKIP, 54, 0, 127, 0, 0},
    {SKIP, 55, 0, 127, 0, 0},
    {SKIP, 56, 0, 127, 0, 0},
    {SKIP, 57, 0, 127, 0, 0},
    {SKIP, 58, 0, 127, 0, 0},
    {SKIP, 59, 0, 127, 0, 0},

    // Standard MIDI CC pass-through (CC 60-66)
    {CC, 60, 0, 127, 0, 0},       // 60
    {CC, 61, 0, 127, 0, 0},       // 61
    {CC, 62, 0, 127, 0, 0},       // 62
    {CC, 63, 0, 127, 0, 0},       // 63
    {CC, 64, 0, 127, 0, 0},       // 64 Sustain pedal
    {SKIP, 65, 0, 127, 0, 0},     // 65
    {CC, 66, 0, 127, 0, 0},       // 66 Sostenuto

    // Operator-specific parameters - OP4 (CC 67-78)
    {SYSEX, 67, 0, 31, 18, 0},    // 67 OP4 Attack Rate (AR) - VCED
    {SYSEX, 68, 0, 31, 18, 1},    // 68 OP4 Decay 1 Rate (D1R) - VCED
    {SYSEX, 69, 0, 31, 18, 2},    // 69 OP4 Decay 2 Rate (D2R) - VCED
    {SYSEX, 70, 0, 15, 18, 3},    // 70 OP4 Release Rate (RR) - VCED
    {SYSEX, 71, 0, 15, 18, 4},    // 71 OP4 Decay 1 Level (D1L) - VCED
    {SYSEX, 72, 0, 99, 18, 10},   // 72 OP4 Output Level (OUT) - VCED
    {SYSEX, 73, 0, 63, 18, 11},   // 73 OP4 Frequency Coarse (CRS) - VCED
    {SYSEX, 74, 0, 6, 18, 12},    // 74 OP4 Detune (DET) - VCED
    {SYSEX, 75, 0, 99, 18, 5},    // 75 OP4 Level Scaling (LS) - VCED
    {SYSEX, 76, 0, 7, 18, 9},     // 76 OP4 Key Velocity Sens (KVS) - VCED
    {SYSEX, 77, 0, 7, 19, 3},     // 77 OP4 Waveform (OSW) - ACED
    {SYSEX, 78, 0, 1, 19, 0},     // 78 OP4 Fixed Frequency Mode - ACED

    // Operator-specific parameters - OP3 (CC 79-90)
    {SYSEX, 79, 0, 31, 18, 13},   // 79 OP3 Attack Rate (AR) - VCED
    {SYSEX, 80, 0, 31, 18, 14},   // 80 OP3 Decay 1 Rate (D1R) - VCED
    {SYSEX, 81, 0, 31, 18, 15},   // 81 OP3 Decay 2 Rate (D2R) - VCED
    {SYSEX, 82, 0, 15, 18, 16},   // 82 OP3 Release Rate (RR) - VCED
    {SYSEX, 83, 0, 15, 18, 17},   // 83 OP3 Decay 1 Level (D1L) - VCED
    {SYSEX, 84, 0, 99, 18, 23},   // 84 OP3 Output Level (OUT) - VCED
    {SYSEX, 85, 0, 63, 18, 24},   // 85 OP3 Frequency Coarse (CRS) - VCED
    {SYSEX, 86, 0, 6, 18, 25},    // 86 OP3 Detune (DET) - VCED
    {SYSEX, 87, 0, 99, 18, 18},   // 87 OP3 Level Scaling (LS) - VCED
    {SYSEX, 88, 0, 7, 18, 22},    // 88 OP3 Key Velocity Sens (KVS) - VCED
    {SYSEX, 89, 0, 7, 19, 8},     // 89 OP3 Waveform (OSW) - ACED
    {SYSEX, 90, 0, 1, 19, 5},     // 90 OP3 Fixed Frequency Mode - ACED

    // Operator-specific parameters - OP2 (CC 91-102)
    {SYSEX, 91, 0, 31, 18, 26},   // 91 OP2 Attack Rate (AR) - VCED
    {SYSEX, 92, 0, 31, 18, 27},   // 92 OP2 Decay 1 Rate (D1R) - VCED
    {SYSEX, 93, 0, 31, 18, 28},   // 93 OP2 Decay 2 Rate (D2R) - VCED
    {SYSEX, 94, 0, 15, 18, 29},   // 94 OP2 Release Rate (RR) - VCED
    {SYSEX, 95, 0, 15, 18, 30},   // 95 OP2 Decay 1 Level (D1L) - VCED
    {SYSEX, 96, 0, 99, 18, 36},   // 96 OP2 Output Level (OUT) - VCED
    {SYSEX, 97, 0, 63, 18, 37},   // 97 OP2 Frequency Coarse (CRS) - VCED
    {SYSEX, 98, 0, 6, 18, 38},    // 98 OP2 Detune (DET) - VCED
    {SYSEX, 99, 0, 99, 18, 31},   // 99 OP2 Level Scaling (LS) - VCED
    {SYSEX, 100, 0, 7, 18, 35},   // 100 OP2 Key Velocity Sens (KVS) - VCED
    {SYSEX, 101, 0, 7, 19, 13},   // 101 OP2 Waveform (OSW) - ACED
    {SYSEX, 102, 0, 1, 19, 10},   // 102 OP2 Fixed Frequency Mode - ACED

    // Operator-specific parameters - OP1 (CC 103-114)
    {SYSEX, 103, 0, 31, 18, 39},  // 103 OP1 Attack Rate (AR) - VCED
    {SYSEX, 104, 0, 31, 18, 40},  // 104 OP1 Decay 1 Rate (D1R) - VCED
    {SYSEX, 105, 0, 31, 18, 41},  // 105 OP1 Decay 2 Rate (D2R) - VCED
    {SYSEX, 106, 0, 15, 18, 42},  // 106 OP1 Release Rate (RR) - VCED
    {SYSEX, 107, 0, 15, 18, 43},  // 107 OP1 Decay 1 Level (D1L) - VCED
    {SYSEX, 108, 0, 99, 18, 49},  // 108 OP1 Output Level (OUT) - VCED
    {SYSEX, 109, 0, 63, 18, 50},  // 109 OP1 Frequency Coarse (CRS) - VCED
    {SYSEX, 110, 0, 6, 18, 51},   // 110 OP1 Detune (DET) - VCED
    {SYSEX, 111, 0, 99, 18, 44},  // 111 OP1 Level Scaling (LS) - VCED
    {SYSEX, 112, 0, 7, 18, 48},   // 112 OP1 Key Velocity Sens (KVS) - VCED
    {SYSEX, 113, 0, 7, 19, 18},   // 113 OP1 Waveform (OSW) - ACED
    {SYSEX, 114, 0, 1, 19, 15},   // 114 OP1 Fixed Frequency Mode - ACED

    // Reserved/Special purpose (CC 115-119)
    {SKIP, 115, 0, 127, 0, 0},    // 115
    {SKIP, 116, 0, 127, 0, 0},    // 116
    {SKIP, 117, 0, 127, 0, 0},    // 117
    {SKIP, 118, 0, 127, 0, 0},    // 118
    {SKIP, 119, 0, 127, 0, 0},    // 119

    // System messages (CC 120-127)
    {SYSTEM, 120, 0, 127, 0, 0},  // 120 All Sound Off
    {SYSTEM, 121, 0, 127, 0, 0},  // 121 Reset All Controllers
    {SYSTEM, 122, 0, 127, 0, 0},  // 122 Local Control
    {SYSTEM, 123, 0, 127, 0, 0},  // 123 All Notes Off
    {SYSTEM, 124, 0, 127, 0, 0},  // 124 Omni Mode Off
    {SYSTEM, 125, 0, 127, 0, 0},  // 125 Omni Mode On
    {SYSTEM, 126, 0, 127, 0, 0},  // 126 Mono Mode On
    {SYSTEM, 127, 0, 127, 0, 0},  // 127 Poly Mode On
};
RtMidiIn* midiIn = 0;
RtMidiOut* SYX = 0;
RtMidiOut* HWOUT = 0;

int main(int argc, char* argv[])
{
    midiIn = new RtMidiIn();
    midiIn->setCallback(&onMIDI);
    midiIn->ignoreTypes(false, false, true); // dont ignore clocK
    SYX = new RtMidiOut();
    HWOUT = new RtMidiOut();
    signal(SIGINT, signalHandler);
    //
    if (argc > 1) {
        string cmd(argv[1]);
        cout << "Command: " << cmd << endl;
        if (cmd == "-ports") {
            listOutPorts();
            cout << endl
                 << endl;
            //     listInports();
            cleanup();
        }

        if (cmd == "-p") {
            if (argc < 3) {
                cout << "Error ! Please Provide Midi Port Name to bind to!" << endl;
                cleanup();
            }
            oPORTNAME = string(argv[2]);
            initHWPORT();
        }
    }

    if (oPORTNAME == "") {
        SYX->openVirtualPort(PORT_PREFIX + "SYX");
        cout << "txsex => Created Virtual Output Port: " << PORT_PREFIX << "SYX" << endl;
    }
    midiIn->openVirtualPort(PORT_PREFIX + "CC");
    cout << "txsex => Created Virtual Input Port: " << PORT_PREFIX << "CC" << endl;
    cout << "Send Your CC Commands to PORT: " << PORT_PREFIX << "CC" << endl;
    updateAlgos(0);
    while (true) // the main loop
    {

        if (oPORTNAME != "") {
            long elapsed = getSecs() - nextCheck;
            if (elapsed >= 2) {
                int pid = getOutPort(oPORTNAME);
                if (pid == -1) {
                    HW_EXISTS = false;
                } else {
                    if (HW_EXISTS == false) {
                        initHWPORT();
                    }
                }
                nextCheck = getSecs() + 2;
            }
        }

        usleep(5000);
    }
    cleanup();
    return 0;
}

void onMIDI(double deltatime, std::vector<unsigned char>* message, void* /*userData*/) // handles incomind midi
{
    // cout << "MIDI MESSAGE" << endl;

    unsigned char byte0 = (int)message->at(0);
    unsigned char typ = byte0 & 0xF0;
    unsigned char ch = byte0 & 0x0F;
    uint size = message->size();
    if (size == 1 || byte0 == 0xF0 || typ != 0xB0) // sysex or clock or non cc
    {
        sendMessage(message);
    } else {
        int mCC = (int)message->at(1);
        CC_MAPPING C = MAP[mCC];

        //  cout << "MAP: " << C.TYPE << " Param: " << C.PARAMETER << endl;
        if (C.TYPE == CC || C.TYPE == SYSTEM) {
            // cout << "CC: " << mCC << endl;
            message->at(1) = C.CC; // remap incoming CC to target CC as in MAP.
            sendMessage(message);
            return;
        }
        // ... existing code ...

        if (C.TYPE == SYSEX) {
            int value = limit(message->at(2), C.MIN, C.MAX);
            if (C.CC == 76) { // we have algorithm update
                updateAlgos(value);
            }

            vector<unsigned char> oSYX = BASE_SYX;

            oSYX.at(BPOS::GROUP) = C.GROUP;
            oSYX.at(BPOS::PARAMETER) = C.PARAMETER;
            oSYX.at(BPOS::DATA) = value;

            // DEBUG OUTPUT
            //cout << "CC: " << mCC << " GROUP: " << (int)C.GROUP << " (0x" << hex << (int)C.GROUP << dec << ") PARAM: " << C.PARAMETER << " VAL: " << value << endl;

            sendMessage(&oSYX);
        }

        // ... existing code ...
        if (C.TYPE == MACRO) {
            ENVS ENV;
            switch (C.PARAMETER) {
            case 0:
                ENV = ATTACK;
                break;
            case 1:
                ENV = DECAY;
                break;
            case 2:
                ENV = SUSTAIN;
                break;
            case 3:
                ENV = RELEASE;
                break;
            }
            vector<int> params;
            switch (C.GROUP) {
            case 0:
                params = ENV.CARRIERS;
                break;
            case 1:
                params = ENV.MODULATORS;
                break;
            case 2:
                params = ENV.LCARRIERS;
                break;
            case 3:
                params = ENV.LMODULATORS;
                break;
            }

            for (size_t i = 0; i != params.size(); i++) {
                //  cout << "Macro: item " << params.at(i) << endl;
                vector<unsigned char>* mmessage = message;
                mmessage->at(1) = (unsigned char)params.at(i);
                onMIDI(deltatime, mmessage, 0);
            }
        }
    }
}

int limit(int v, int min, int max)
{
    if (v < min)
        v = min;
    if (v > max)
        v = max;
    return v;
}

void listInports()
{
    uint nPorts = midiIn->getPortCount();
    cout << "************ INPUTS ************" << endl;
    for (uint i = 0; i < nPorts; i++) {
        std::string portName = midiIn->getPortName(i);
        std::cout << portName << "\n";
    }
}
void listOutPorts()
{
    uint nPorts = SYX->getPortCount();
    cout << "************ Midi Outputs ************" << endl;
    for (uint i = 0; i < nPorts; i++) {
        std::string portName = SYX->getPortName(i);
        std::cout << portName << "\n";
    }
}
void cleanup()
{
    delete midiIn;
    delete SYX;
    HWOUT->closePort();
    delete HWOUT;
    exit(0);
}

int getinPort(std::string str)
{
    int nPorts = midiIn->getPortCount();
    for (int i = 0; i < nPorts; i++) {
        std::string portName = midiIn->getPortName(i);
        size_t found = portName.find(str);
        if (found != string::npos) {
            return i;
        }
    }
    return 99;
}
int getOutPort(std::string str)
{
    int nPorts = SYX->getPortCount();
    for (int i = 0; i < nPorts; i++) {
        std::string portName = SYX->getPortName(i);
        size_t found = portName.find(str);
        if (found != string::npos) {
            return i;
        }
    }
    return -1;
}
void initHWPORT()
{
    int oid = getOutPort(oPORTNAME);
    if (oid != -1) {
        if (HWOUT->isPortOpen()) {
            HWOUT->closePort();
        }
        try {
            HWOUT->openPort((unsigned int)oid, PORT_PREFIX + "SYX");
            HW_EXISTS = true;
            cout << "Opened HW Port (" << SYX->getPortName(oid) << " as " << PORT_PREFIX << "SYX) for Output with ID: " << oid << endl;
        } catch (...) {
            HW_EXISTS = false;
            cout << "Error Opening: " << SYX->getPortName(oid) << "for Output" << endl;
        }
    } else {
        HW_EXISTS = false;
        cout << oPORTNAME << "Not Available Yet" << endl;
    }
}
void sendMessage(vector<unsigned char>* message)
{
    if (oPORTNAME == "")
        SYX->sendMessage(message);
    else {

        try {
            HWOUT->sendMessage(message);
        } catch (...) {
            cout << "Error Sendind Midi to: " << oPORTNAME << endl;
        }
    }
}
long long getSecs() // gets time since epch in seconds
{
    auto t1 = std::chrono::system_clock::now();
    long long us = duration_cast<seconds>(t1.time_since_epoch()).count();
    return us;
}
void signalHandler(int signum)
{
    cout << "Interrupt signal (" << signum << ") received.\n";
    cout << "Process dxsex Terminiated!" << endl;
    cleanup();
}

void updateAlgos(int algo)
{
    // Clear all envelope lists for carriers and modulators
    ATTACK.CARRIERS.clear();
    DECAY.CARRIERS.clear();
    SUSTAIN.CARRIERS.clear();
    RELEASE.CARRIERS.clear();
    ATTACK.MODULATORS.clear();
    DECAY.MODULATORS.clear();
    SUSTAIN.MODULATORS.clear();
    RELEASE.MODULATORS.clear();
    ATTACK.LCARRIERS.clear();
    DECAY.LCARRIERS.clear();
    SUSTAIN.LCARRIERS.clear();
    RELEASE.LCARRIERS.clear();
    ATTACK.LMODULATORS.clear();
    DECAY.LMODULATORS.clear();
    SUSTAIN.LMODULATORS.clear();
    RELEASE.LMODULATORS.clear();

    // Base CC numbers for TX-81Z operator parameters (reorganized layout)
    // Operators mapped to: OP4=CC67, OP3=CC79, OP2=CC91, OP1=CC103
    struct
    {
        int ATTACK = 67;    // Base CC for Attack Rate (AR)
        int DECAY1 = 68;    // Base CC for Decay 1 Rate (D1R)
        int DECAY2 = 69;    // Base CC for Decay 2 Rate (D2R)
        int RELEASE = 70;   // Base CC for Release Rate (RR)
        int D1LEVEL = 71;   // Base CC for Decay 1 Level (D1L)
        int OUTPUT = 72;    // Base CC for Output Level
    } index;

    // Each operator block is 12 CCs apart (67, 79, 91, 103)
    const int OP_SPACING = 12;

    // Loop through 4 operators
    // i=1 maps to OP4 (CC 67+), i=2 to OP3 (CC 79+), i=3 to OP2 (CC 91+), i=4 to OP1 (CC 103+)
    for (int i = 1; i <= 4; i++) {
        int opOffset = (i - 1) * OP_SPACING;

        if (isSet(ALGOS[algo], i)) {
            // This operator is a CARRIER (produces audible output)
            ATTACK.CARRIERS.push_back(index.ATTACK + opOffset);
            DECAY.CARRIERS.push_back(index.DECAY1 + opOffset);
            SUSTAIN.CARRIERS.push_back(index.DECAY2 + opOffset);
            RELEASE.CARRIERS.push_back(index.RELEASE + opOffset);

            // Level parameters (D1L as attack level, OUTPUT as sustain level)
            ATTACK.LCARRIERS.push_back(index.D1LEVEL + opOffset);
            DECAY.LCARRIERS.push_back(index.D1LEVEL + opOffset);
            SUSTAIN.LCARRIERS.push_back(index.OUTPUT + opOffset);
            RELEASE.LCARRIERS.push_back(index.OUTPUT + opOffset);
        } else {
            // This operator is a MODULATOR (modulates other operators)
            ATTACK.MODULATORS.push_back(index.ATTACK + opOffset);
            DECAY.MODULATORS.push_back(index.DECAY1 + opOffset);
            SUSTAIN.MODULATORS.push_back(index.DECAY2 + opOffset);
            RELEASE.MODULATORS.push_back(index.RELEASE + opOffset);

            ATTACK.LMODULATORS.push_back(index.D1LEVEL + opOffset);
            DECAY.LMODULATORS.push_back(index.D1LEVEL + opOffset);
            SUSTAIN.LMODULATORS.push_back(index.OUTPUT + opOffset);
            RELEASE.LMODULATORS.push_back(index.OUTPUT + opOffset);
        }
    }
    // std::cout << "TX-81Z 4-Op Algos Updated (reorganized CC layout)" << endl;
}

bool isSet(int n, int k)
{

    return ((n & (1 << (k - 1))) > 0);
}
