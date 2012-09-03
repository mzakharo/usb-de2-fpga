module ISP1362_IF(	//	HOST Side
					iDATA,
					oDATA,
					iADDR,
					iRD_N,
					iWR_N,
					iCS_N,
					iRST_N,
					iCLK,
					oINT0_N,
					oINT1_N,
					//	ISP1362 Side
					OTG_DATA,
					OTG_ADDR,
					OTG_RD_N,
					OTG_WR_N,
					OTG_CS_N,
					OTG_RST_N,
					OTG_INT0,
					OTG_INT1	);
//	HOST Side
input	[15:0]	iDATA;
input	[1:0]	iADDR;
input			iRD_N;
input			iWR_N;
input			iCS_N;
input			iRST_N;
input			iCLK;
output	[15:0]	oDATA;
output			oINT0_N;
output			oINT1_N;
//	ISP1362 Side
inout	[15:0]	OTG_DATA;
output	[1:0]	OTG_ADDR;
output			OTG_RD_N;
output			OTG_WR_N;
output			OTG_CS_N;
output			OTG_RST_N;
input			OTG_INT0;
input			OTG_INT1;

reg		[1:0]	OTG_ADDR;
reg				OTG_RD_N;
reg				OTG_WR_N;
reg				OTG_CS_N;
reg		[15:0]	TMP_DATA;
reg		[15:0]	oDATA;
reg				oINT0_N;
reg				oINT1_N;

assign	OTG_DATA	=	OTG_WR_N	?	16'hzzzz	:	TMP_DATA	;

always@(posedge iCLK or negedge iRST_N)
begin
	if(!iRST_N)
	begin
		TMP_DATA	<=	0;
		OTG_ADDR	<=	0;
		OTG_RD_N	<=	1;
		OTG_WR_N	<=	1;
		OTG_CS_N	<=	1;
		TMP_DATA	<=	0;
		oDATA		<=	0;
		oINT0_N		<=	1;
		oINT1_N		<=	1;
	end
	else
	begin
		oDATA		<=	OTG_DATA;
		oINT0_N		<=	OTG_INT0;
		oINT1_N		<=	OTG_INT1;
		TMP_DATA	<=	iDATA;
		OTG_ADDR 	<=	iADDR;
		OTG_RD_N 	<=	iRD_N;
		OTG_WR_N	<=	iWR_N;
		OTG_CS_N	<=	iCS_N;
	end
end

assign	OTG_RST_N	=	iRST_N;

endmodule