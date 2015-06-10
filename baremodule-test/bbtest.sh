#/bin/bash
source /home/pixel_dev/local/root/bin/thisroot.sh
source /home/pixel_dev/.bashrc

cd pxar/pxar
cat main/testBM | bin/pXar -d data/DigitalModuleTest
cd 

cp /home/pixel_dev/pxar/pxar/data/DigitalModuleTest/pxar.root /home/pixel_dev/bbtest/B322015-32-1/roc$1.root
cp /home/pixel_dev/pxar/pxar/data/DigitalModuleTest/pxar.log /home/pixel_dev/bbtest/B322015-32-1/roc$1.log
cp /home/pixel_dev/pxar/pxar/data/DigitalModuleTest/dacParameters_C0.dat  /home/pixel_dev/bbtest/B322015-32-1/dacParameters_C$1.dat

