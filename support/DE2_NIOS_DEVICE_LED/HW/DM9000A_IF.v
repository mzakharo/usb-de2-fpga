module DM9000A_IF(	//	HOST Side
					iDATA,
					oDATA,
					iCMD,
					iRD_N,
					iWR_N,
					iCS_N,
					iRST_N,
					iCLK,
					iOSC_50,
					oINT,
					//	DM9000A Side
					ENET_DATA,
					ENET_CMD,
					ENET_RD_N,
					ENET_WR_N,
					ENET_CS_N,
					ENET_RST_N,
					ENET_INT,
					ENET_CLK	);
//	HOST Side
input	[15:0]	iDATA;
input			iCMD;
input			iRD_N;
input			iWR_N;
input			iCS_N;
input			iRST_N;
input			iCLK;
input			iOSC_50;
output	[15:0]	oDATA;
output			oINT;
//	DM9000A Side
inout	[15:0]	ENET_DATA;
output			ENET_CMD;
output			ENET_RD_N;
output			ENET_WR_N;
output			ENET_CS_N;
output			ENET_RST_N;
output			ENET_CLK;
input			ENET_INT;

reg		[15:0]	TMP_DATA;
reg				ENET_CMD;
reg				ENET_RD_N;
reg				ENET_WR_N;
reg				ENET_CS_N;
reg				ENET_CLK;
reg		[15:0]	oDATA;
reg				oINT;

assign	ENET_DATA	=	ENET_WR_N	?	16'hzzzz	:	TMP_DATA;

always@(posedge iCLK or negedge iRST_N)
begin
	if(!iRST_N)
	begin
		TMP_DATA 	<=	0;
		ENET_CMD 	<=	0;
		ENET_RD_N 	<=	1;
		ENET_WR_N 	<=	1;
		ENET_CS_N 	<=	1;
		oDATA		<=	0;
		oINT		<=	0;
	end
	else
	begin
		oDATA		<=	ENET_DATA;
		oINT		<=	ENET_INT;
		TMP_DATA 	<=	iDATA;
		ENET_CMD 	<=	iCMD;
		ENET_CS_N 	<=	iCS_N;
		ENET_RD_N 	<=	iRD_N;
		ENET_WR_N 	<=	iWR_N;
	end
end

always@(posedge iOSC_50)
ENET_CLK	<=	~ENET_CLK;

assign	ENET_RST_N	=	iRST_N;

endmodule



