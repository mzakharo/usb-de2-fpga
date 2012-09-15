
clean:
	cd quartus && make clean
	cd support/winusb/usb2/ && make clean
	rm -rf src/*.bak
