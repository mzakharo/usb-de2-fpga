// sram_0.v

// This file was auto-generated as part of a SOPC Builder generate operation.
// If you edit it your changes will probably be lost.

`timescale 1 ps / 1 ps
module sram_0 (
		input  wire        iCLK,      //                   clk.clk
		inout  wire [15:0] SRAM_DQ,   // avalon_slave_0_export.export
		output wire [17:0] SRAM_ADDR, //                      .export
		output wire        SRAM_UB_N, //                      .export
		output wire        SRAM_LB_N, //                      .export
		output wire        SRAM_WE_N, //                      .export
		output wire        SRAM_CE_N, //                      .export
		output wire        SRAM_OE_N, //                      .export
		input  wire [15:0] iDATA,     //        avalon_slave_0.writedata
		output wire [15:0] oDATA,     //                      .readdata
		input  wire [17:0] iADDR,     //                      .address
		input  wire        iWE_N,     //                      .write_n
		input  wire        iOE_N,     //                      .read_n
		input  wire        iCE_N,     //                      .chipselect_n
		input  wire [1:0]  iBE_N,     //                      .byteenable_n
		input  wire        iRST_N     //               reset_n.reset_n
	);

	SRAM_16Bit_512K sram_0 (
		.iCLK      (iCLK),      //                   clk.clk
		.SRAM_DQ   (SRAM_DQ),   // avalon_slave_0_export.export
		.SRAM_ADDR (SRAM_ADDR), //                      .export
		.SRAM_UB_N (SRAM_UB_N), //                      .export
		.SRAM_LB_N (SRAM_LB_N), //                      .export
		.SRAM_WE_N (SRAM_WE_N), //                      .export
		.SRAM_CE_N (SRAM_CE_N), //                      .export
		.SRAM_OE_N (SRAM_OE_N), //                      .export
		.iDATA     (iDATA),     //        avalon_slave_0.writedata
		.oDATA     (oDATA),     //                      .readdata
		.iADDR     (iADDR),     //                      .address
		.iWE_N     (iWE_N),     //                      .write_n
		.iOE_N     (iOE_N),     //                      .read_n
		.iCE_N     (iCE_N),     //                      .chipselect_n
		.iBE_N     (iBE_N),     //                      .byteenable_n
		.iRST_N    (iRST_N)     //               reset_n.reset_n
	);

endmodule
