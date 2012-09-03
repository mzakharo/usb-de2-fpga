// DM9000A.v

// This file was auto-generated as part of a SOPC Builder generate operation.
// If you edit it your changes will probably be lost.

`timescale 1 ps / 1 ps
module DM9000A (
		input  wire        iCLK,       //                   clk.clk
		input  wire        iOSC_50,    // avalon_slave_0_export.export
		inout  wire [15:0] ENET_DATA,  //                      .export
		output wire        ENET_CMD,   //                      .export
		output wire        ENET_RD_N,  //                      .export
		output wire        ENET_WR_N,  //                      .export
		output wire        ENET_CS_N,  //                      .export
		output wire        ENET_RST_N, //                      .export
		output wire        ENET_CLK,   //                      .export
		input  wire        ENET_INT,   //                      .export
		input  wire [15:0] iDATA,      //        avalon_slave_0.writedata
		input  wire        iCMD,       //                      .address
		input  wire        iRD_N,      //                      .read_n
		input  wire        iWR_N,      //                      .write_n
		input  wire        iCS_N,      //                      .chipselect_n
		output wire [15:0] oDATA,      //                      .readdata
		output wire        oINT,       //    avalon_slave_0_irq.irq
		input  wire        iRST_N      //               reset_n.reset_n
	);

	DM9000A_IF dm9000a (
		.iCLK       (iCLK),       //                   clk.clk
		.iOSC_50    (iOSC_50),    // avalon_slave_0_export.export
		.ENET_DATA  (ENET_DATA),  //                      .export
		.ENET_CMD   (ENET_CMD),   //                      .export
		.ENET_RD_N  (ENET_RD_N),  //                      .export
		.ENET_WR_N  (ENET_WR_N),  //                      .export
		.ENET_CS_N  (ENET_CS_N),  //                      .export
		.ENET_RST_N (ENET_RST_N), //                      .export
		.ENET_CLK   (ENET_CLK),   //                      .export
		.ENET_INT   (ENET_INT),   //                      .export
		.iDATA      (iDATA),      //        avalon_slave_0.writedata
		.iCMD       (iCMD),       //                      .address
		.iRD_N      (iRD_N),      //                      .read_n
		.iWR_N      (iWR_N),      //                      .write_n
		.iCS_N      (iCS_N),      //                      .chipselect_n
		.oDATA      (oDATA),      //                      .readdata
		.oINT       (oINT),       //    avalon_slave_0_irq.irq
		.iRST_N     (iRST_N)      //               reset_n.reset_n
	);

endmodule
