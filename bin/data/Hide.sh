#!/bin/sh

#  Hide.sh
#  DeepListen
#
#  Created by gameover on 14/07/13.
#
# hide an application in the Finder

osascript <<END
tell application "Finder"
if exists application process "$1" then
set visible of application process "$1" to false
end if
set visible of application "Finder" to false
end tell
END