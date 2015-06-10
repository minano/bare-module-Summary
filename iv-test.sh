#/bin/bash

BATCH=$1
WAFER=$2
SENSOR=$3
moduleID=B$1-$2-$3

if [ ${1} == '' ]; then
    echo "Please give me the moduleID like '32249X XX X'"
    exit 1   
fi

if [ ! -e bbtest/$moduleID ]; then
    mkdir /home/pixel_dev/bbtest/${moduleID}
else
    echo "Folder for bare module exists."
fi


cd pxar/pxar/data/DigitalModuleTest
OLD="hvOn 0"
NEW="hvOn 1"
sed -i "25s/$OLD/$NEW/g" configParameters.dat 

cd ../..
bin/pXar -d data/DigitalModuleTest -t IV -p "port=/dev/ttyUSB0"

cd data/DigitalModuleTest
sed -i "25s/$NEW/$OLD/g" configParameters.dat
cd

echo "Copying iv.root..."

cp /home/pixel_dev/pxar/pxar/data/DigitalModuleTest/pxar.root /home/pixel_dev/bbtest/${moduleID}/iv.root

echo "and ivCurve.log..."
cp /home/pixel_dev/pxar/pxar/data/DigitalModuleTest/ivCurve.log /home/pixel_dev/bbtest/${moduleID}/ivCurve.log

echo "Done!"
