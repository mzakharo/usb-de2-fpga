

	
VHDSRC:= $(shell find ../src/ -name "*.vhd")

all: time load  


load: usb.asm.rpt
	quartus_pgm -c USB-Blaster usb.cdf

db/usb.map.cdb: $(VHDSRC)
	quartus_map --read_settings_files=on --write_settings_files=off usb -c usb	
usb.fit.rpt:  db/usb.map.cdb
	quartus_fit --read_settings_files=off --write_settings_files=off usb -c usb
usb.asm.rpt: usb.fit.rpt
	quartus_asm --read_settings_files=off --write_settings_files=off usb -c usb
usb.sta.rpt: usb.fit.rpt
	quartus_sta usb -c usb
time : usb.sta.rpt
	cat $<

clean:
	rm -rf db incremental_db *.rpt *.smsg *.summary *.sof *.pof *.pin *.done *.qdf

