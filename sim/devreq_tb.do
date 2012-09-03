SetActiveLib -work
comp -include "d:\mzakharo\Documents\Dropbox\projects\usb\hal.vhd" 
comp -include "d:\mzakharo\Documents\Dropbox\projects\usb\devreq.vhd"
comp -include "d:\mzakharo\Documents\Dropbox\projects\usb\devreq_tb.vhd" 
asim +access +r devreq_tb 
wave 
wave -noreg clk
wave -noreg reset	
wave -noreg hi
wave -noreg ho
wave -noreg dr
wave -noreg di
wave -noreg do 
wave -noreg otg_data	
#wave -noreg isp_emu_data	

