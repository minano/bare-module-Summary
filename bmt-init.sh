#/bin/bash

BATCH=$1
WAFER=$2
SENSOR=$3
moduleID=B$1-$2-$3

if [ ${1} == '' ]; then
    echo "Please give me the moduleID like '32249X XX X'"
    exit 1   
fi

cd pxar/pxar
main/mkConfig -d data/DigitalModuleTest -r digv21respin -b
cd ~pixel_dev

if [ ! -e bbtest/$moduleID ]; then
    mkdir /home/pixel_dev/bbtest/${moduleID}
    echo "Folder" $moduleID "created!"
else
    echo "Folder" $moduleID "for bare module exists."
fi

cat > bbtest.sh << EOF
#/bin/bash
source /home/pixel_dev/local/root/bin/thisroot.sh
source /home/pixel_dev/.bashrc

cd pxar/pxar
cat main/testBM | bin/pXar -d data/DigitalModuleTest
cd 

cp /home/pixel_dev/pxar/pxar/data/DigitalModuleTest/pxar.root /home/pixel_dev/bbtest/$moduleID/roc\$1.root
cp /home/pixel_dev/pxar/pxar/data/DigitalModuleTest/pxar.log /home/pixel_dev/bbtest/$moduleID/roc\$1.log
cp /home/pixel_dev/pxar/pxar/data/DigitalModuleTest/dacParameters_C0.dat  /home/pixel_dev/bbtest/$moduleID/dacParameters_C\$1.dat

EOF
chmod 755 bbtest.sh
echo "bbtest.sh created!"
