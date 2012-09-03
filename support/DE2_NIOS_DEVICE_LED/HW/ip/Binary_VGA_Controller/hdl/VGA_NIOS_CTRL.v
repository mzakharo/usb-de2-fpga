module	VGA_NIOS_CTRL (	//	Host Side
						iDATA,
						oDATA,
						iADDR,
						iWR,
						iRD,
						iCS,
						iRST_N,
						iCLK,		//	Host Clock
						//	Export Side
						VGA_R,
						VGA_G,
						VGA_B,
						VGA_HS,
						VGA_VS,
						VGA_SYNC,
						VGA_BLANK,
						VGA_CLK,
						iCLK_25	);

parameter	RAM_SIZE	=	19'h4B000;

//	Host Side
output	[15:0]	oDATA;
input	[15:0]	iDATA;	
input	[18:0]	iADDR;
input			iWR,iRD,iCS;
input			iCLK,iRST_N;
reg		[15:0]	oDATA;
//	Export Side
output	[9:0]	VGA_R;
output	[9:0]	VGA_G;
output	[9:0]	VGA_B;
output			VGA_HS;
output			VGA_VS;
output			VGA_SYNC;
output			VGA_BLANK;
output			VGA_CLK;
input			iCLK_25;

reg		[3:0]	mCursor_RGB_N;
reg		[9:0]	mCursor_X;
reg		[9:0]	mCursor_Y;
reg		[9:0]	mCursor_R;
reg		[9:0]	mCursor_G;
reg		[9:0]	mCursor_B;
reg		[9:0]	mON_R;
reg		[9:0]	mON_G;
reg		[9:0]	mON_B;
reg		[9:0]	mOFF_R;
reg		[9:0]	mOFF_G;
reg		[9:0]	mOFF_B;
wire	[18:0]	mVGA_ADDR;
wire	[9:0]	mVGA_R;
wire	[9:0]	mVGA_G;
wire	[9:0]	mVGA_B;

always@(posedge iCLK or negedge iRST_N)
begin
	if(!iRST_N)
	begin
		mCursor_RGB_N	<=	0;
		mCursor_X		<=	0;
		mCursor_Y		<=	0;
		mCursor_R		<=	0;
		mCursor_G		<=	0;
		mCursor_B		<=	0;
		mON_R			<=	0;
		mON_G			<=	0;
		mON_B			<=	0;
		mOFF_R			<=	0;
		mOFF_G			<=	0;
		mOFF_B			<=	0;
		oDATA			<=	0;
	end
	else
	begin
		if(iCS)
		begin
			if(iWR)
			begin
				case(iADDR)
				RAM_SIZE+0 :	mCursor_RGB_N	<=	iDATA;
				RAM_SIZE+1 :	mCursor_X		<=	iDATA;
				RAM_SIZE+2 :	mCursor_Y		<=	iDATA;
				RAM_SIZE+3 :	mCursor_R		<=	iDATA;
				RAM_SIZE+4 :	mCursor_G		<=	iDATA;
				RAM_SIZE+5 :	mCursor_B		<=	iDATA;
				RAM_SIZE+6 :	mON_R			<=	iDATA;
				RAM_SIZE+7 :	mON_G			<=	iDATA;
				RAM_SIZE+8 :	mON_B			<=	iDATA;
				RAM_SIZE+9 :	mOFF_R			<=	iDATA;
				RAM_SIZE+10:	mOFF_G			<=	iDATA;
				RAM_SIZE+11:	mOFF_B			<=	iDATA;
				endcase
			end
			else if(iRD)
			begin
				case(iADDR)
				RAM_SIZE+0 :	oDATA	<=	mCursor_RGB_N	;
				RAM_SIZE+1 :	oDATA	<=	mCursor_X		;
				RAM_SIZE+2 :	oDATA	<=	mCursor_Y		;
				RAM_SIZE+3 :	oDATA	<=	mCursor_R		;
				RAM_SIZE+4 :	oDATA	<=	mCursor_G		;
				RAM_SIZE+5 :	oDATA	<=	mCursor_B		;
				RAM_SIZE+6 :	oDATA	<=	mON_R			;
				RAM_SIZE+7 :	oDATA	<=	mON_G			;
				RAM_SIZE+8 :	oDATA	<=	mON_B			;
				RAM_SIZE+9 :	oDATA	<=	mOFF_R			;
				RAM_SIZE+10:	oDATA	<=	mOFF_G			;
				RAM_SIZE+11:	oDATA	<=	mOFF_B			;
				endcase
			end
		end
	end
end

VGA_Controller		u0	(	//	Host Side
								.iCursor_RGB_EN(mCursor_RGB_N),
								.iCursor_X(mCursor_X),
								.iCursor_Y(mCursor_Y),
								.iCursor_R(mCursor_R),
								.iCursor_G(mCursor_G),
								.iCursor_B(mCursor_B),							
								.oAddress(mVGA_ADDR),
								.iRed	(mVGA_R),
								.iGreen	(mVGA_G),
								.iBlue	(mVGA_B),
								//	VGA Side
								.oVGA_R(VGA_R),
								.oVGA_G(VGA_G),
								.oVGA_B(VGA_B),
								.oVGA_H_SYNC(VGA_HS),
								.oVGA_V_SYNC(VGA_VS),
								.oVGA_SYNC(VGA_SYNC),
								.oVGA_BLANK(VGA_BLANK),
								.oVGA_CLOCK(VGA_CLK),
								//	Control Signal
								.iCLK_25(iCLK_25),
								.iRST_N(iRST_N)	);

VGA_OSD_RAM			u1	(	//	Read Out Side
							.oRed(mVGA_R),
							.oGreen(mVGA_G),
							.oBlue(mVGA_B),
							.iVGA_ADDR(mVGA_ADDR),
							.iVGA_CLK(VGA_CLK),
							//	Write In Side
							.iWR_DATA(iDATA),
							.iWR_ADDR(iADDR),
							.iWR_EN(iWR && (iADDR < RAM_SIZE) && iCS),
							.iWR_CLK(iCLK),
							//	CLUT
							.iON_R(mON_R),
							.iON_G(mON_G),
							.iON_B(mON_B),
							.iOFF_R(mOFF_R),
							.iOFF_G(mOFF_G),
							.iOFF_B(mOFF_B),
							//	Control Signals
							.iRST_N(iRST_N)	);
								
endmodule