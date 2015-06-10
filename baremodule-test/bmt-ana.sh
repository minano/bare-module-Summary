#/bin/bash

BATCH=$1
WAFER=$2
SENSOR=$3
moduleID=B$1-$2-$3

if [ ${1} == '' ]; then
    echo "Please give me the moduleID like '32249X XX X'"
    exit 1   
fi

cd ~pixel_dev/bbtest
root -b -l  <<EOF
.L prepareRootFile.C
prepareRootFile("$moduleID");
.q
EOF

cd ~pixel_dev/pixel/digTest
current_time=$(date +_20%y-%m-%d_%Hh%Mm_%s)
echo "Current Time : $current_time"
next_current_time=$(date +-20%y-%m-%d_%Hh%Mm_%s)
echo "Current Time : $next_current_time"
 
new_folderName=$moduleID$current_time
echo "new Bare Module folder: " "$new_folderName"
 
mkdir $new_folderName

echo "Type operator: "
read operator
echo "Type the temperature: "
read temp
echo "Type the humidity: "
read hum

cd $new_folderName
cat > bareModuleInfo.txt <<EOF
Laboratory: CERN
Operator: $operator
Temperature: $temp
RH: $hum
BBcut: 35
EOF

cd ..

cp configParameters.dat.default $new_folderName/configParameters.dat
cd ~pixel_dev


cp ~pixel_dev/bbtest/$moduleID/*.dat  ~pixel_dev/bbtest/$moduleID/*.root  ~pixel_dev/bbtest/$moduleID/*.log ~pixel_dev/pixel/digTest/$new_folderName
cd ~pixel_dev/MoReWeb/Analyse
python Controller.py -BT ~pixel_dev/pixel/digTest/$new_folderName

cd

cd ~pixel_dev/pixel/digTest/$new_folderName/BareFinalResults-R001/QualificationGroup/BareModule_p17_1/Summary1

BB="${moduleID}-BareModuleTestBumpBonding"
folderBB=${BB}$next_current_time
echo ${folderBB}
PA="${moduleID}-BareModuleTestPixelAlive"
folderPA=${PA}$next_current_time
echo ${folderPA}

TARBB="${folderBB}.tar.gz"
TARPA="${folderPA}.tar.gz"

echo ${TARBB}
echo ${TARPA}

echo "Compressing files for test Bump Bonding..."
tar -cvzf $TARBB ${folderBB}/*.*


echo "Compressing files for test Pixel Alive..."
tar -cvzf $TARPA ${folderPA}/*.*

cd
#echo "Uploading data to Database..."

#scp -P 23481 ${TARBB} cern@cmspixelprod.pi.infn.it:/home/cern/dropbox
#scp -P 23481 ${TARPA} cern@cmspixelprod.pi.infn.it:/home/cern/dropbox

echo "good bye."


