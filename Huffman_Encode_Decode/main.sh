#bin/bash

PKG_OK=$(dpkg-query -W --showformat='${Status}\n' gcc|grep "install ok installed")
if [ "" == "$PKG_OK" ]; then
    echo "=========================="
    echo "Installing gcc compiler"
    echo "=========================="

    sudo apt-get install gcc
fi

printf "Enter cache size: \n"
read m

printf "Enter fileName: \n"
read inputFile

SIZE_IN=$(stat -c%s $inputFile )
printf "******************************************PART I********************************************\n"
printf "                              Compression using Huffman algorithm!!\n"
if [ $SIZE_IN == "0" ]; then
	echo "Input file is empty please try agin after inserting data into the file"
	exit 0
fi

echo "Input file is "
echo $inputFile
echo "Size of input file $SIZE_IN"

cp  $inputFile dup_input.txt


g++ Encode.cpp -o e.out
printf $inputFile | ./e.out

echo "The compressed file is input.txt.huf"
SIZE_HUF=$(stat -c%s "input.txt.huf")


cp input.txt.huf compressed_input.huf



g++ Decode.cpp -o d.out
printf "input.txt.huf" | ./d.out

ER_VAL=$(diff -U 0 dup_input.txt $inputFile | grep -v ^@ | wc -l)
SIZE_IN=$(stat -c%s $inputFile)
C_RATIO=$(echo "scale =3; ($SIZE_IN)/($SIZE_HUF)" | bc) 
printf "Compression Ratio = $C_RATIO\n"
printf "Total Differece between file before and after Huffman compression = $ER_VAL\n"

# g++ entropy.cpp - o h.out
# ./h.out

printf "\n"


printf "******************************************PART II********************************************\n"
printf "                              Compression using LZ algorithm!!\n"

g++ LZ_encode-decode.cpp -o encode-decode.out

SIZE_IN=$(stat -c%s $inputFile )
printf "Size of input file $SIZE_IN \n"


./encode-decode.out $m $inputFile encodings.txt decodings.txt

SIZE_LZ=$(stat -c%s "encodings.txt")
C_RATIO=$(echo "scale =3; ($SIZE_LZ/($SIZE_IN*8))" | bc) 

ER_VAL=$(diff -b -B -q -U 0 decodings.txt $inputFile | grep -v ^@ | wc -l)

printf "Total Differece between file before and after LZ compression = = $ER_VAL \n"
diff $inputFile decodings.txt -b -q -B

printf "\n"

printf "Entropy value is "
g++ entropy.cpp -o h.out
./h.out


printf "Terminating program.....\nPress Y/n to delete the ecodings or not: "
read ch

if [[ $ch == "Y" ||  $ch == "y" ]];then
	rm compressed_input.huf
	rm dup_input.txt
	rm *.out
	rm encodings.txt
	rm decodings.txt
fi


printf "Program Terminated!!\n"
