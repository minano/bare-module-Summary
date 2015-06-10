#/bin/bash

BATCH=$1
WAFER=$2
SENSOR=$3
moduleID=B$1-$2-$3
sensorID=S$1-$2-$3

if [ ${1} == '']; then
    echo "Please give me the moduleID like '32249X XX X'"
    exit 1   
fi

cd Inventory
touch $moduleID.csv

current_time=$(date +20%y-%m-%d_%Hh%Mm_%s)
echo "Type ROC wafer: "
read rocWafer

echo "Type CHIP0 (5 advacam position):"
read roc0
echo "Type CHIP1 (6 advacam position):"
read roc1
echo "Type CHIP2 (7 advacam position):"
read roc2
echo "Type CHIP3 (8 advacam position):"
read roc3
echo "Type CHIP4 (13 advacam position):"
read roc4
echo "Type CHIP5 (14 advacam position):"
 read roc5
echo "Type CHIP6 (15 advacam position):"
read roc6
echo "Type CHIP7 (16 advacam position):"
read roc7
echo "Type CHIP8 (12 advacam position):"
read roc8
echo "Type CHIP9 (11 advacam position):"
read roc9
echo "Type CHIP10 (10 advacam position):"
read roc10
echo "Type CHIP11 (9 advacam position):"
read roc11
echo "Type CHIP12 (4 advacam position):"
read roc12
echo "Type CHIP13 (3 advacam position):"
read roc13
echo "Type CHIP14 (2 advacam position):"
read roc14
echo "Type CHIP15 (1 advacam position):"
read roc15

cat > $moduleID.csv <<EOF
Sensor_id:$sensorID
Bare_module_ID:$moduleID
BB_company: Advacam
Comment: test bare module
Builton: $current_time
Module_assembly: #WaferXYID, RocPositionInBareModule 
${rocWafer}-${roc0},00
${rocWafer}-${roc1},01
${rocWafer}-${roc2},02
${rocWafer}-${roc3},03
${rocWafer}-${roc4},04
${rocWafer}-${roc5},05
${rocWafer}-${roc6},06
${rocWafer}-${roc7},07
${rocWafer}-${roc8},08
${rocWafer}-${roc9},09
${rocWafer}-${roc10},10
${rocWafer}-${roc11},11
${rocWafer}-${roc12},12
${rocWafer}-${roc13},13
${rocWafer}-${roc14},14
${rocWafer}-${roc15},15
type: NotReworked
center: CERN
EOF







