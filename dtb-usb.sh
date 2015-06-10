#/bin/sh -f
sudo chmod 777 /dev/bus/usb/
export bus=`lsusb | grep Future | awk '{print $2}'`
export port=`lsusb | grep Future | awk '{print substr($4,1,3)}'`

echo /dev/bus/usb/${bus}/${port}
sudo ls -l /dev/bus/usb/${bus}/${port} 
sudo chmod 666 /dev/bus/usb/${bus}/${port}
sudo ls -l /dev/bus/usb/${bus}/${port}
