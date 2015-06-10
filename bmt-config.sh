#/bin/bash

BATCH=$1
WAFER=$2
SENSOR=$3

moduleID=B${1}-${2}-${3}
folder=${moduleID}*

if [ ${1} == '' ]; then
    echo "Please give me the sensorID like '32249X XX X'"
    exit 1  
fi

echo ${folder}
cd ${folder}/BareFinalResults-R001/QualificationGroup/BareModule_p17_1/Summary1

BB="${moduleID}-BareModuleTestBumpBonding"
folderBB=${BB}*
echo ${folderBB}
PA="${moduleID}-BareModuleTestPixelAlive"
folderPA=${PA}*
echo ${folderPA}

ext="tar.gz"

TARBB=$folderBB.tar.gz
TARPA=$folderPA.$ext

echo ${TARPA}

#echo "Compressing files for test Bump Bonding..."
#tar -cvzf ${folderBB}.tar.gz ${moduleID}-BareModuleTestBumpBonding*/*.*


#echo "Compressing files for test Pixel Alive..."
#tar -cvzf ${folderPA}.tar.gz ${moduleID}-BareModuleTestPixelAlive*/*.*


#echo "Uploading data to Database..."

#scp -P 23481 ${TARBB} cern@cmspixelprod.pi.infn.it:/home/cern/dropbox
#scp -P 23481 ${TARPA} cern@cmspixelprod.pi.infn.it:/home/cern/dropbox

echo "good bye."
