if [ "$(id -u)" != "0" ]; then
	echo -e "\e[41m installing requires root privileges\e[0m"
	echo "exiting..."
	exit
fi

FILE=/usr/local/lib/libgmp.a
if [ -f "$FILE" ]; then
	echo "libgmp installed... skipping"
else
	echo "installing libgmp... wait"
	sleep 3
	cd lib/gmp/
	./configure
	apt get install m4
	make
	#echo "make check"
	#make check
	make install
	cd - 
	echo -e "\e[42mlibgmp installed succesfully\e[0m"
fi

gcc main.c -o main -lgmp
