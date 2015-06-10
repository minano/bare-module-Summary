
#! /bin/sh-f
source /home/pixel_dev/local/root/bin/thisroot.sh
source /home/pixel_dev/.bashrc

cp /home/pixel_dev/pxar/pxar/data/DigitalModuleTest/pxar.root /home/pixel_dev/bbtest/B322497-15-3/roc$1.root
cp /home/pixel_dev/pxar/pxar/data/DigitalModuleTest/pxar.log /home/pixel_dev/bbtest/B322497-15-3/roc$1.log
cp /home/pixel_dev/pxar/pxar/data/DigitalModuleTest/dacParameters_C0.dat  /home/pixel_dev/bbtest/B322497-15-3/dacParameters_C$1.dat

