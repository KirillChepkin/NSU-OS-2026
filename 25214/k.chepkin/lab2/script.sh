export TZ="PST8PDT"
gcc main.c -o main
./main
export TZ="GMT-7"
./main
rm main