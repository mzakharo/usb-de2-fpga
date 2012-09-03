LIBRARY ieee;
USE ieee.std_logic_1164.ALL;

--
-- 7segment driver hex display of a number from A to J
--   This is created as an ENTITY so that it can be reused many times easily
--
ENTITY sevenseg IS PORT(
		dataIn : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
		segmentsOut : OUT STD_LOGIC_VECTOR(6 DOWNTO 0) );
END sevenseg;

architecture rtl of sevenseg IS
BEGIN
-- The following SELECT statement converts a 4 bit number, called dataIn to a pattern of 7 bits, or wires, 
--   to connect to a 7-segment display called segmentsOut
--
        with dataIn SELECT             -- 7-segment bits are A to G but the LSB is A
               segmentsOut(6 DOWNTO 0) <=     "1000000" WHEN "0000",         -- 0
                                              "1111001" WHEN "0001",         -- 1
                                              "0100100" WHEN "0010",         -- 2
                                              "0110000" WHEN "0011",         -- 3
                                              "0011001" WHEN "0100",         -- 4
                                              "0010010" WHEN "0101",         -- 5
                                              "0000010" WHEN "0110",         -- 6
                                              "1111000" WHEN "0111",         -- 7
                                              "0000000" WHEN "1000",         -- 8
                                              "0010000" WHEN "1001",         -- 9
                                              "0001000" WHEN "1010",         -- A
                                              "0000011" WHEN "1011",         -- b
                                              "0100111" WHEN "1100",         -- c
                                              "0100001" WHEN "1101",         -- d
                                              "0000110" WHEN "1110",         -- E
                                              "0001110" WHEN others;         -- F and for simulation
END rtl;
