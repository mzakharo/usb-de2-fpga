SetActiveLib -work
#comp -include "d:\mzakharo\Documents\Dropbox\projects\usb\otg.vhd" 
comp -include "D:\mzakharo\Documents\Dropbox\projects\usb\drv_tb.vhd" 
asim +access +r drv_tb 
wave 
wave -noreg clk
wave -noreg reset	
wave -noreg otg_i
wave -noreg otg_o
wave -noreg drv_i
wave -noreg drv_o 
wave -noreg otg_data	
wave -noreg dbg
# The following lines can be used for timing simulation
# acom <backannotated_vhdl_file_name>
# comp -include "D:\mzakharo\Documents\Dropbox\projects\usb\isp1368_TB_tim_cfg.vhd" 
# asim +access +r TIMING_FOR_isp1368 
