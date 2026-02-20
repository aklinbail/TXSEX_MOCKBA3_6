#!/bin/sh

appname=dxsex
appTitle=DxSex
appDir=dxSex

################ NO NEED TO EDIT BELOW THIS LINE ###############

mmPath=$(cat /dev/shm/.mmPath)
. $mmPath/MockbaMod/env.sh

runDir="$mmPath/AddOns/"
installroot="$mmPath/AddOns/$appDir/"
runScript="$runDir/run_$appname.sh"
mode=$1

echo "
***********************************************************
*   $appTitle AddOn Manager for Mockba Mod      *
***********************************************************
"
if [ "$mode" == "UNINSTALL" ]; then

    if [[ -e "$installroot" ]]; then
        killall $appname 2>/dev/null
        rm -rf "$installroot/"
        rm -f "$runScript" &
        >/dev/null
        echo "<<<< $appTitle has been UnInstalled."
        echo
    fi
fi

if [ "$mode" == "DISABLE" ]; then

    killall "$appname"

    rm -f "$runScript"
    echo "$appTitle has been disabled from Auto Launch"
fi

if [ "$mode" == "ENABLE" ]; then

    cp -f "$installroot/run_$appname.sh" "$runScript"
    "$runScript"
    echo "$appTitle has been enabled for Auto Launch"

fi
