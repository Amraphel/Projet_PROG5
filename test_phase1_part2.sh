#!/bin/bash
FILES=$(find ./Examples_loader -executable | grep example*)

#Color
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
GREEN='\033[0;32m'
PURPLE='\033[0;35m'
NC='\033[0m' # No Color

for file in $FILES
do

readelf -S $file > output_readelf1
./read_elfSection $file > output_readelf2

echo -e "${BLUE}START TEST ON ${YELLOW} $file ${NC}:"


res=1
i=1

head -n -5 output_readelf1 > temp ; mv temp output_readelf1
head -n -5 output_readelf2 > temp ; mv temp output_readelf2

nbLigne1=$(wc -l < output_readelf1)
nbLigne2=$(wc -l < output_readelf2)

if [ $nbLigne1 -ne $nbLigne2 ]
then
    res=0
    echo "Nombre de ligne différent"
fi

while read lineFile1 <&3 && read lineFile2 <&4  
do  
    if [ $i -gt 4 ] ; then

    line1=$(echo $lineFile1 | tr -d " \t\n\r")
    line2=$(echo $lineFile2 | tr -d " \t\n\r")
    if [ "$line1" != "$line2" ] ; then
        lineEcho1=$(echo $lineFile1 | tr -d "\t\n\r")
        lineEcho2=$(echo $lineFile2 | tr -d "\t\n\r")
        echo -e "\"$lineEcho1\" ${PURPLE}DIFFERENT THAN${NC} \"$lineEcho2\""
        res=0
    fi 
    
    fi 
    ((i=i+1))
done 3<output_readelf1 4<output_readelf2

if [ $res == 0 ] ; then
    echo -e "${RED}ERROR (see above)${NC}\n"
else
    echo -e "${GREEN}SUCCESS${NC}\n"
fi


done


rm output_readelf1
rm output_readelf2
