# txSex - CC to SYSEX Converter for TX-81z - it probably works for other 4 op DX synths too. 
## This is a fork of DX Sex and the copyright notice is in the name of the original author

#### This version does not startup automatically yet. - will be fixed soon. Or feel free to fix the run_txsex.sh scrip

### dxSex - CC to SYSEX Converter for DX7, VolcaFM, Dexed for Akai Force MoackbaMod / Raspberry Pi/Mac.


####  **[Some Nerd Stats from 40,000 Unique Presets](Most%20Popular%20DX7%20Algos.txt)

### Note: Not All Params Have Been Mapped. (Well most voice ones are - just missing a few, which I may get around to. 
Please view the code in main.cpp to check mappings for the time being until it’s documented.

## Installation:
**For Mockba Modded Akai Force:**

1. With Amit Talwar's NoderServer App ( https://github.com/amittalwar/NoderServer this should be available soon!!!) 
  
   1. Launch the NoderServer App in Browser and go to Modules and enable DxSex. By default, it will output to the Physical MIDI DIN Port. To change the Target,
   2. You can list available Ports by running "dxsex -ports" command in the console. 
   3. If you dont see dxSex in your Addons, you can copy dxSex from the PRovided AddOns Folder to yor McokbaMod SDCard/AddOns Folder.

  
 * If you run dxSex without providing a physical hardware output port name, it will create a virtual dxsyx port that you will have to manually connect to outputs via aconnect commands.

2. On your Akai Device, load the provided Midi Map File (TX81Z-txsyx.xpm) onto a Midi Track.
3. If you have installed it directly on your Akai Device, set the Midi track output to DXCC and ch 1.
 Changig Parameters on Akai Screen/Macro Knobs (set to screen mode) should change Mapped Params on your Device..

 **For Mac:**
 1. Run the script compile_mac.sh in src folder to build for Mac.
 2. Run it without any parameters and it should create midi ports that you can use in your DAW.
 3. If you want to control an external synth. give its hardware port name as -p./com   Parameter.


### A note on MIDI Buffer Full errors:
These are common and can be ignored.
The TX81z has a very small buffer on a small processor. 
This app actually thins out the sends. So I've only seen these now on Operator Ratio, Fine Tune and Level with the current build.
Yes, I know they are annoying - but if you've ever used your TX-81z with a capable controller - you've seen them before.
At least Yamaha was smart and the device keeps operating and you can still keep editing your sound.
(just push the edit button twice on the hardware to clear the message)