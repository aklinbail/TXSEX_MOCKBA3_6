# dxSex - CC to SYSEX Converter for DX7, VolcaFM, Dexed for Akai Force MoackbaMod / Raspberry Pi/Mac.

####  **[Some Nerd Stats from 40,000 Unique Presets](Most%20Popular%20DX7%20Algos.txt)

### Note: Not All Params Have Been Mapped.
Please view the code in main.cpp to check mappings for the time being until it’s documented.

## Installation:
**For Mockba Modded Akai Force:**

1. With My NodeServer App this add-on should be available in the add-on manager(force 3.3 and below) 
  
   1. Launch the NoderServer App in Browser and go to Modules and enable DxSex. By default, it will output to the Physical MIDI DIN Port. To change the Target,
   2. You can list available Ports by running "dxsex -ports" command in the console. 
   3. If you dont see dxSex in your Addons, you can copy dxSex from the PRovided AddOns Folder to yor McokbaMod SDCard/AddOns Folder.

  
 * If you run dxSex without providing a physical hardware output port name, it will create a virtual dxsyx port that you will have to manually connect to outputs via aconnect commands.

2. On your Akai Device, load the provided Midi Map File (DEXED - Raspi.xpm) onto a Midi Track.
3. If you have installed it directly on your Akai Device, set the Midi track output to DXCC and ch 1.
 Changig Parameters on Akai Screen/Macro Knobs (set to screen mode) should change Mapped Params on your Device..

 **For Mac:**
 1. Run the script compile_mac.sh in src folder to build for Mac.
 2. Run it without any parameters and it should create midi ports that you can use in your DAW.
 3. If you want to control an external synth. give its hardware port name as -p./com   Parameter.
