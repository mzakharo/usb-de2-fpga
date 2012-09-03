// ISP1362.v

// This file was auto-generated as part of a SOPC Builder generate operation.
// If you edit it your changes will probably be lost.

`timescale 1 ps / 1 ps
module ISP1362 (
		input  wire        avs_hc_clk_iCLK,           //    hc_clock.clk
		input  wire        avs_hc_reset_n_iRST_N,     //  hc_reset_n.reset_n
		input  wire [15:0] avs_hc_writedata_iDATA,    //          hc.writedata
		output wire [15:0] avs_hc_readdata_oDATA,     //            .readdata
		input  wire        avs_hc_address_iADDR,      //            .address
		input  wire        avs_hc_read_n_iRD_N,       //            .read_n
		input  wire        avs_hc_write_n_iWR_N,      //            .write_n
		input  wire        avs_hc_chipselect_n_iCS_N, //            .chipselect_n
		output wire        avs_hc_irq_n_oINT0_N,      //      hc_irq.irq_n
		input  wire        avs_dc_clk_iCLK,           //    dc_clock.clk
		input  wire        avs_dc_reset_n_iRST_N,     //  dc_reset_n.reset_n
		input  wire [15:0] avs_dc_writedata_iDATA,    //          dc.writedata
		output wire [15:0] avs_dc_readdata_oDATA,     //            .readdata
		input  wire        avs_dc_address_iADDR,      //            .address
		input  wire        avs_dc_read_n_iRD_N,       //            .read_n
		input  wire        avs_dc_write_n_iWR_N,      //            .write_n
		input  wire        avs_dc_chipselect_n_iCS_N, //            .chipselect_n
		output wire        avs_dc_irq_n_oINT0_N,      //      dc_irq.irq_n
		inout  wire [15:0] USB_DATA,                  // conduit_end.export
		output wire [1:0]  USB_ADDR,                  //            .export
		output wire        USB_RD_N,                  //            .export
		output wire        USB_WR_N,                  //            .export
		output wire        USB_CS_N,                  //            .export
		output wire        USB_RST_N,                 //            .export
		input  wire        USB_INT0,                  //            .export
		input  wire        USB_INT1                   //            .export
	);

	ISP1362_IF isp1362 (
		.avs_hc_clk_iCLK           (avs_hc_clk_iCLK),           //    hc_clock.clk
		.avs_hc_reset_n_iRST_N     (avs_hc_reset_n_iRST_N),     //  hc_reset_n.reset_n
		.avs_hc_writedata_iDATA    (avs_hc_writedata_iDATA),    //          hc.writedata
		.avs_hc_readdata_oDATA     (avs_hc_readdata_oDATA),     //            .readdata
		.avs_hc_address_iADDR      (avs_hc_address_iADDR),      //            .address
		.avs_hc_read_n_iRD_N       (avs_hc_read_n_iRD_N),       //            .read_n
		.avs_hc_write_n_iWR_N      (avs_hc_write_n_iWR_N),      //            .write_n
		.avs_hc_chipselect_n_iCS_N (avs_hc_chipselect_n_iCS_N), //            .chipselect_n
		.avs_hc_irq_n_oINT0_N      (avs_hc_irq_n_oINT0_N),      //      hc_irq.irq_n
		.avs_dc_clk_iCLK           (avs_dc_clk_iCLK),           //    dc_clock.clk
		.avs_dc_reset_n_iRST_N     (avs_dc_reset_n_iRST_N),     //  dc_reset_n.reset_n
		.avs_dc_writedata_iDATA    (avs_dc_writedata_iDATA),    //          dc.writedata
		.avs_dc_readdata_oDATA     (avs_dc_readdata_oDATA),     //            .readdata
		.avs_dc_address_iADDR      (avs_dc_address_iADDR),      //            .address
		.avs_dc_read_n_iRD_N       (avs_dc_read_n_iRD_N),       //            .read_n
		.avs_dc_write_n_iWR_N      (avs_dc_write_n_iWR_N),      //            .write_n
		.avs_dc_chipselect_n_iCS_N (avs_dc_chipselect_n_iCS_N), //            .chipselect_n
		.avs_dc_irq_n_oINT0_N      (avs_dc_irq_n_oINT0_N),      //      dc_irq.irq_n
		.USB_DATA                  (USB_DATA),                  // conduit_end.export
		.USB_ADDR                  (USB_ADDR),                  //            .export
		.USB_RD_N                  (USB_RD_N),                  //            .export
		.USB_WR_N                  (USB_WR_N),                  //            .export
		.USB_CS_N                  (USB_CS_N),                  //            .export
		.USB_RST_N                 (USB_RST_N),                 //            .export
		.USB_INT0                  (USB_INT0),                  //            .export
		.USB_INT1                  (USB_INT1)                   //            .export
	);

endmodule
