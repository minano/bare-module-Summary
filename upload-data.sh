#/bin/bash

BATCH=$1
WAFER=$2
SENSOR=$3
sensorID=S${1}-${2}-${3}

if [ ${1} == '' ]; then
    echo "Please give me the sensorID like '322497 XX X'"
    exit 1   
fi

cd SensorTest_${sensorID}_BAM

TARFILE="SensorTest_${sensorID}_BAM.tar.gz"
scp -P 23481 ${TARFILE} cern@cmspixelprod.pi.infn.it:/home/cern/dropbox
