import re
vreg_s = '#define\s+([\w]+)\s+(0x[0-9A-Fa-f]+)\s*$'
vreg = re.compile(vreg_s)

f = open('hal.h', 'r')
o = open('out.vhd', 'w')

#define\s+[\w,]+\s+(0[x,X])?\d+
#http://gskinner.com/RegExr/

#cmm prepare

for line in f:
	
	reg = vreg.match(line)
	if (reg != None ):
		i = int( reg.group(2),16)
		str = 'constant %s : std_logic_vector(15 downto 0) := x"%04X";\n' % (reg.group(1), i)
		print str
		o.write(str)
o.close()
f.close()
	
