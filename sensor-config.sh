#/bin/bash

BATCH=$1
WAFER=$2
SENSOR=$3

sensorID=S${1}-${2}-${3}
moduleID=B${1}-${2}-${3}

if [ ${1} == '' ]; then
    echo "Please give me the sensorID like '32249X XX X'"
    exit 1  
fi

echo "Type temperature: "
read temperature

mkdir SensorTest_${sensorID}_BAM

echo "Filling... tab.txt for sensor" $sensorID

cp ~pixel_dev/bbtest/${moduleID}/ivCurve.log ~pixel_dev/Sensor_DATABASE/SensorTest_${sensorID}_BAM/ivCurve.log
cp ~pixel_dev/bbtest/${moduleID}/ivCurve.log ~pixel_dev/Sensor_DATABASE/SensorTest_${sensorID}_BAM/SensorTest_${sensorID}_BAM.tab.txt 

echo "Done."
echo "Filling... inf.txt for sensor" $sensorID

root -b -l  <<EOF 
.L BMT_sensor.C
sensorSummary("SensorTest_${sensorID}_BAM",${temperature})
.q
EOF

cd SensorTest_${sensorID}_BAM

I100=$(sed '3q;d' logfile.dat)
I150=$(sed '5q;d' logfile.dat)
slope=$(sed '7q;d' logfile.dat)
grade=$(sed '9q;d' logfile.dat)

cat > SensorTest_${sensorID}_BAM.inf.txt <<EOF
CENTRE CERN
STEP BAM
BATCH $BATCH
WAFER $WAFER
SENSOR $SENSOR
V1 100
V2 150
I1 $I100
I2 $I150
SLOPE $slope
TEMPERATURE $temperature
GRADE $grade
DATE $(date +%s)
COMMENT
EOF

echo "Done."

echo "Compressing files..."
tar -cvzf SensorTest_${sensorID}_BAM.tar.gz SensorTest_${sensorID}_BAM.inf.txt SensorTest_${sensorID}_BAM.tab.txt

echo "Grade: ${grade}"
echo "Do not forget to upload data to Database!!"
echo "good bye."

