#!/bin/bash

echo "Compiling...."
 g++ -w -D__LINUX_ALSA__ -O3 -fPIC -Wno-unused-variable *.cpp -o bin/dxsex  -lm -ldl -lstdc++ -lasound -lpthread
echo  "All Done.."
echo "Remember to Copy the new bin/dxsex to your MockbaMod_SDCARD/AddOns/dxSex folder"

