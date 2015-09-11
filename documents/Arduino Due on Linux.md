###Arduino Due on Linux

It might a problem to upload the project to the Arduino Due from Eclipse IDE. So instead of upload it from Eclipse it is possible to upload it from the terminal. The following is a short guide how to upload the project with the help from a Bash script.

#### Guide

1. Open Gedit and save the script file as bossac.sh.
2. Add this to bossac.sh:
  + #!/bin/bash
  + # executing bash command without backticks
  + $echo bossac --port=<portToArduino> -R -e -w -v -b <Directory to folder to .bin file>/<Filename>.bin
3. Navigate to the bossac.sh directory with the terminal.
4. Write the command:
  + chmod +x bossac.sh
5. Before running the script press the erase button on Arduino Due card, if no "no device found on <port>"
6. Start the script by type:
  + ./bossac.sh
  
