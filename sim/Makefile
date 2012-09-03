
#top 

TOP=hal_tb

# work library name (must be the same as in modelsim.ini)
WORK=work

#Compiler
VC=vcom
#VCOPT=-2008
#Library Generator
VL=vlib
#Simulator
VS=vsim

SRC= \
	hal.vhd \
	hal_tb.vhd \
	isp_inc.vhd \
	usb_inc.vhd \
	devreq.vhd \
	drv.vhd \
	drv_tb.vhd \
	sevenseg.vhd

	
VHDSRC:= $(shell find . -name "*.vhd")

all: $(VC) 


echo:
	echo $(FOO)


OBJS=$(SRC:.vhd=)
OBJS:=$(addprefix $(WORK)/,$(OBJS))

$(VC): $(WORK) $(OBJS) 
	touch $(VC)

$(WORK)/% : %.vhd 
	$(VC) $(VCOPT) $<

$(WORK):
	$(VL) $@

vsim: $(VC)
	$(VS) -gui $(TOP) -do $(TOP).sim &


clean:
	rm -rf vcom  *.wlf transcript $(WORK)

