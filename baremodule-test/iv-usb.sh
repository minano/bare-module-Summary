#/bin/bash

sudo stty -F /dev/ttyUSB0 57600 cs8 clocal cread cr3 echo iexten hupcl
sudo chmod a+r /dev/ttyUSB0
sudo chmod a+w /dev/ttyUSB0
sudo chmod a+x /dev/ttyUSB0
