#file main_postmap.tcl
#Include the flow package to create a post-map netlist
#package require ::quartus::flow
#open the project in TimeQuest
#project_open usb
#create a post-map database
#execute_module -tool map
#create the timing netlist based on the post-map results
#create_timing_netlist -post_fit -model slow
#read in the constraints from the golden SDC file
#read_sdc usb.sdc
#update the timing netlist with the new constraints
#update_timing_netlist
#generated a clock report
#report_clocks
#generated a clock-to-clock report
#report_clock_transfers

report_clock_fmax_summary -panel_name {FMax}
report_timing -setup -pairs_only -panel_name {Slow Model Worst-Case Timing Paths||Slow Model Setup: 'clock'} -to_clock [get_clocks {clock}] -npaths 20 -detail summary

report_timing -setup -pairs_only -to_clock [get_clocks {clock}] -npaths 1
report_clock_fmax_summary 
#delete our post-map timing netlist
#delete_timing_netlist
#close the TimeQuest project
#project_close