gcc main.c -o main
echo "------------------------"
./main -i
echo "------------------------"
./main -i -p
echo "------------------------"
./main -c -u
echo "------------------------"
./main -Vvalue=name -Venter=shift
echo "------------------------"
./main -c -C 1024 -c
echo "------------------------"
./main -u -U 1024 -u
echo "------------------------"
./main -p -s -p
echo "------------------------"
./main -d
echo "------------------------"
./main -V enter=shift -v
echo "------------------------"
./main
echo "------------------------"
rm main