// Audio_0.v

// This file was auto-generated as part of a SOPC Builder generate operation.
// If you edit it your changes will probably be lost.

`timescale 1 ps / 1 ps
module Audio_0 (
		input  wire        iWR_CLK,   //                   clk.clk
		output wire        oAUD_DATA, // avalon_slave_0_export.export
		output wire        oAUD_LRCK, //                      .export
		output wire        oAUD_BCK,  //                      .export
		output wire        oAUD_XCK,  //                      .export
		input  wire        iCLK_18_4, //                      .export
		input  wire [15:0] iDATA,     //        avalon_slave_0.writedata
		input  wire        iWR,       //                      .write
		output wire [15:0] oDATA,     //                      .readdata
		input  wire        iRST_N     //               reset_n.reset_n
	);

	AUDIO_DAC_FIFO #(
		.REF_CLK     (18432000),
		.SAMPLE_RATE (48000),
		.DATA_WIDTH  (16),
		.CHANNEL_NUM (2)
	) audio_0 (
		.iWR_CLK   (iWR_CLK),   //                   clk.clk
		.oAUD_DATA (oAUD_DATA), // avalon_slave_0_export.export
		.oAUD_LRCK (oAUD_LRCK), //                      .export
		.oAUD_BCK  (oAUD_BCK),  //                      .export
		.oAUD_XCK  (oAUD_XCK),  //                      .export
		.iCLK_18_4 (iCLK_18_4), //                      .export
		.iDATA     (iDATA),     //        avalon_slave_0.writedata
		.iWR       (iWR),       //                      .write
		.oDATA     (oDATA),     //                      .readdata
		.iRST_N    (iRST_N)     //               reset_n.reset_n
	);

endmodule
