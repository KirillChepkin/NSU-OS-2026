touch file.txt
gcc -Wall -Wextra main.c -o prog
chmod u+r,u+w,u+x,g-r,g-w,g+x,o-r,o-w,o-x prog
echo "------------------"
echo "Launched the rogram with execution rights only set for owner and their group:"
ls -l | grep prog
./prog
echo "------------------"
chmod u+s prog
echo "Launched the program with setuid bit:"
ls -l | grep prog
./prog