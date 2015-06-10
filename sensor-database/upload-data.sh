#/bin/bash

BATCH=$1
WAFER=$2
SENSOR=$3
STEP=$4
sensorID=S${1}-${2}-${3}

if [ ${1} == '' ]; then
    echo "Please give me the sensorID like '322497 XX X STEP'"
    exit 1   
fi

cd SensorTest_${sensorID}_${STEP}

TARFILE="SensorTest_${sensorID}_${STEP}.tar.gz"
scp -P 23481 ${TARFILE} cern@cmspixelprod.pi.infn.it:/home/cern/dropbox
