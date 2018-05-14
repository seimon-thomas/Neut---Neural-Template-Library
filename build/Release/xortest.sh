#! /bin/sh
FAILURE=0
echo '********************************************************************'
echo '********************************************************************'
echo Cleaning files...
rm ../Data/Networks/XORNetwork_2_5_1/*.xml
rm ../Data/Outputs/XORNetwork_2_5_1Output.csv
echo ----------------------------------------
echo Creatng Network...
./NeuralNetworkApplication --create --template="../Data/Templates/XORNetwork_2_5_1.xml" --network="../Data/Networks/XORNetwork_2_5_1/XORNetwork_2_5_1.xml"
FAILURE=$?
if [ $FAILURE -ne 0 ]; then
echo "*** Create failed with code: $FAILURE"
fi
if [ $FAILURE -eq 0 ]; then
echo ----------------------------------------
echo Training Network...
./NeuralNetworkApplication --train --input="../Data/Inputs/XORInput.csv" --network="../Data/Networks/XORNetwork_2_5_1/XORNetwork_2_5_1.xml" --target="../Data/Targets/XORTargetsScaled.csv" --error=0.001 --saveevery=10000 --epochsize=4
FAILURE=$?
if [ $FAILURE -ne 0 ]; then
echo "*** Train failed with code: $FAILURE"
fi
fi
if [ $FAILURE -eq 0 ]; then
echo ----------------------------------------
echo Recalling Network...
./NeuralNetworkApplication --recall --input="../Data/Inputs/XORInput.csv" --network="../Data/Networks/XORNetwork_2_5_1/XORNetwork_2_5_1.xml" --output="../Data/Outputs/XORNetwork_2_5_1Output.csv" --target="../Data/Targets/XORTargetsScaled.csv" --epochsize=4
FAILURE=$?
if [ $FAILURE -ne 0 ]; then
echo "*** Recall failed with code: $FAILURE"
fi
fi
if [ $FAILURE -eq 0 ]; then
echo ----------------------------------------
echo Comparing Network...
diff ../Data/Networks/XORNetwork_2_5_1/XORNetwork_2_5_1.16468.xml ../Data/Networks/XORNetwork_2_5_1/Backup/XORNetwork_2_5_1.16468.xml
FAILURE=$?
if [ $FAILURE -ne 0 ]; then
echo "*** Produced Network is differen"
else
echo "Success - Networks match."
fi
fi
if [ $FAILURE -eq 0 ]; then
echo ----------------------------------------
echo Comparing Outputs...
diff ../Data/Outputs/XORNetwork_2_5_1Output.csv ../Data/Outputs/Backup/XORNetwork_2_5_1Output.csv
FAILURE=$?
if [ $FAILURE -ne 0 ]; then
echo "*** Produced Output is differen"
else
echo "Success - Outputs match."
fi
fi
echo ----------------------------------------
if [ $FAILURE -eq 0 ]; then
echo Finished - Success!.
else
echo Finished - ***Failure***
fi
