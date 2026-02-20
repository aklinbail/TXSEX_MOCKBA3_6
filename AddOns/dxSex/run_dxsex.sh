
#!/bin/sh
############################################################
# Copy this File to $mmPath/AddOns to Launch Automatically
###########################################################

# Set up the environment
mmPath=$(cat /dev/shm/.mmPath)
. $mmPath/MockbaMod/env.sh
port="$(cat $mmPath/AddOns/dxSex/DX-MIDI-PORT.txt)"
if test "$1" == "kill"; then
    killall dxsex 2>/dev/null
else
  $mmPath/AddOns/dxSex/dxsex -p "$port"  2>/dev/null   &
fi

