#/bin/bash

BATCH=$1
WAFER=$2
SENSOR=$3
moduleID=B${1}-${2}-${3}

if [ ${1} == '' ]; then
    echo "Please give me the sensorID like '322497 XX X'"
    exit 1   
fi

FOLDER=${moduleID}*
echo ${FOLDER}

cd ~pixel_dev/pixel/digTest/${FOLDER}/BareFinalResults-R001/QualificationGroup/BareModule_p17_1/Summary1

pwd
ls *.tar.gz 
#scp -P 23481 *.tar.gz cern@cmspixelprod.pi.infn.it:/home/cern/dropbox

