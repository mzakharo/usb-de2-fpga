// SEG7_Display.v

// This file was auto-generated as part of a SOPC Builder generate operation.
// If you edit it your changes will probably be lost.

`timescale 1 ps / 1 ps
module SEG7_Display (
		input  wire        iCLK,   //          clk.clk
		input  wire        iWR,    // avalon_slave.write
		input  wire [31:0] iDIG,   //             .writedata
		output wire [6:0]  oSEG0,  //  conduit_end.export
		output wire [6:0]  oSEG1,  //             .export
		output wire [6:0]  oSEG2,  //             .export
		output wire [6:0]  oSEG3,  //             .export
		output wire [6:0]  oSEG4,  //             .export
		output wire [6:0]  oSEG5,  //             .export
		output wire [6:0]  oSEG6,  //             .export
		output wire [6:0]  oSEG7,  //             .export
		input  wire        iRST_N  //      reset_n.reset_n
	);

	SEG7_LUT_8 seg7_display (
		.iCLK   (iCLK),   //          clk.clk
		.iWR    (iWR),    // avalon_slave.write
		.iDIG   (iDIG),   //             .writedata
		.oSEG0  (oSEG0),  //  conduit_end.export
		.oSEG1  (oSEG1),  //             .export
		.oSEG2  (oSEG2),  //             .export
		.oSEG3  (oSEG3),  //             .export
		.oSEG4  (oSEG4),  //             .export
		.oSEG5  (oSEG5),  //             .export
		.oSEG6  (oSEG6),  //             .export
		.oSEG7  (oSEG7),  //             .export
		.iRST_N (iRST_N)  //      reset_n.reset_n
	);

endmodule
