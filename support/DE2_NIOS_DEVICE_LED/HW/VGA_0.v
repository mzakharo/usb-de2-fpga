// VGA_0.v

// This file was auto-generated as part of a SOPC Builder generate operation.
// If you edit it your changes will probably be lost.

`timescale 1 ps / 1 ps
module VGA_0 (
		input  wire        iCLK,      //                   clk.clk
		output wire [9:0]  VGA_R,     // avalon_slave_0_export.export
		output wire [9:0]  VGA_G,     //                      .export
		output wire [9:0]  VGA_B,     //                      .export
		output wire        VGA_HS,    //                      .export
		output wire        VGA_VS,    //                      .export
		output wire        VGA_SYNC,  //                      .export
		output wire        VGA_BLANK, //                      .export
		output wire        VGA_CLK,   //                      .export
		input  wire        iCLK_25,   //                      .export
		output wire [15:0] oDATA,     //        avalon_slave_0.readdata
		input  wire [15:0] iDATA,     //                      .writedata
		input  wire [18:0] iADDR,     //                      .address
		input  wire        iWR,       //                      .write
		input  wire        iRD,       //                      .read
		input  wire        iCS,       //                      .chipselect
		input  wire        iRST_N     //               reset_n.reset_n
	);

	VGA_NIOS_CTRL #(
		.RAM_SIZE (307200)
	) vga_0 (
		.iCLK      (iCLK),      //                   clk.clk
		.VGA_R     (VGA_R),     // avalon_slave_0_export.export
		.VGA_G     (VGA_G),     //                      .export
		.VGA_B     (VGA_B),     //                      .export
		.VGA_HS    (VGA_HS),    //                      .export
		.VGA_VS    (VGA_VS),    //                      .export
		.VGA_SYNC  (VGA_SYNC),  //                      .export
		.VGA_BLANK (VGA_BLANK), //                      .export
		.VGA_CLK   (VGA_CLK),   //                      .export
		.iCLK_25   (iCLK_25),   //                      .export
		.oDATA     (oDATA),     //        avalon_slave_0.readdata
		.iDATA     (iDATA),     //                      .writedata
		.iADDR     (iADDR),     //                      .address
		.iWR       (iWR),       //                      .write
		.iRD       (iRD),       //                      .read
		.iCS       (iCS),       //                      .chipselect
		.iRST_N    (iRST_N)     //               reset_n.reset_n
	);

endmodule
