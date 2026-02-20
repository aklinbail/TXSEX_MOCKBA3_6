dxSEX - CC to Sysex Converter for DX7, VolcaFM, Dexed for Akai Force
It kinda Works like an editor, automator for DX7 based synths.

Author: Amit Talwar
Github Repo (Source Code and Downloads): https://github.com/intelliriffer/DXSEX

version: 0.2.0
Installation: 
Copy this dxSex folder to your MockbaMod AddOns Folder.

USAGE:
1: EDIT THE DX-MIDI-PORT.txt file and add the Name of the Midi Port  connected in use with DX/Dexed Device.
the port name can be found running the following command: aconnect -o or better dxsex -ports
for example from ths output:
# aconnect -o

client 16: 'Akai Pro Force' [type=kernel,card=0]
    0 'Akai Pro Force Public'
    1 'Akai Pro Force Private'
    2 'Akai Pro Force MIDI Port'
client 20: 'iConnectAUDIO4+' [type=kernel,card=1]
    0 'CLK/DIN         '
    1 'PC              '
    2 'Jupiter XM      '
    3 'SUB37           '
    4 'TD3             '

You would select: "Akai Pro Force MIDI Port" as the port and put in the file , if using Force Midi Out Port with Din Cables. If you are connected to a USB device use corresponding port.


 2: On Your Akai Device Load the Provided Midi Map File (DEXED - Raspi.xpm) onto a Midi Track.

 3: Set the Midi track output to DXCC and ch 1.
 
4: Setup for H/W Device or Dexed Standalone/vst/au to listen for midi on the respective port for sysex...

 Changing Parameters on Akai Screen/Macro Knobs (set to screen mode) Should Change Mapped Params on your Device..