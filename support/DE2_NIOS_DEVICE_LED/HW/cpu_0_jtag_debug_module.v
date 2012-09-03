//Legal Notice: (C)2006 Altera Corporation. All rights reserved.  Your
//use of Altera Corporation's design tools, logic functions and other
//software and tools, and its AMPP partner logic functions, and any
//output files any of the foregoing (including device programming or
//simulation files), and any associated documentation or information are
//expressly subject to the terms and conditions of the Altera Program
//License Subscription Agreement or other applicable license agreement,
//including, without limitation, that your use is for the sole purpose
//of programming logic devices manufactured by Altera and sold by Altera
//or its authorized distributors.  Please refer to the applicable
//agreement for further details.


// turn off superfluous verilog processor warnings 
// altera message_level Level1 
// altera message_off 10034 10035 10036 10037 10230 10240 10030 

module cpu_0_jtag_debug_module (
                                 // inputs:
                                  MonDReg,
                                  break_readreg,
                                  clk,
                                  clrn,
                                  dbrk_hit0_latch,
                                  dbrk_hit1_latch,
                                  dbrk_hit2_latch,
                                  dbrk_hit3_latch,
                                  debugack,
                                  ena,
                                  ir_in,
                                  jtag_state_udr,
                                  monitor_error,
                                  monitor_ready,
                                  raw_tck,
                                  reset_n,
                                  resetlatch,
                                  rti,
                                  shift,
                                  tdi,
                                  tracemem_on,
                                  tracemem_trcdata,
                                  tracemem_tw,
                                  trc_im_addr,
                                  trc_on,
                                  trc_wrap,
                                  trigbrktype,
                                  trigger_state_1,
                                  update,
                                  usr1,

                                 // outputs:
                                  ir_out,
                                  irq,
                                  jdo,
                                  jrst_n,
                                  st_ready_test_idle,
                                  take_action_break_a,
                                  take_action_break_b,
                                  take_action_break_c,
                                  take_action_ocimem_a,
                                  take_action_ocimem_b,
                                  take_action_tracectrl,
                                  take_action_tracemem_a,
                                  take_action_tracemem_b,
                                  take_no_action_break_a,
                                  take_no_action_break_b,
                                  take_no_action_break_c,
                                  take_no_action_ocimem_a,
                                  take_no_action_tracemem_a,
                                  tdo
                               )
;

  parameter SLD_NODE_INFO = 286279168;


  output  [  1: 0] ir_out;
  output           irq;
  output  [ 37: 0] jdo;
  output           jrst_n;
  output           st_ready_test_idle;
  output           take_action_break_a;
  output           take_action_break_b;
  output           take_action_break_c;
  output           take_action_ocimem_a;
  output           take_action_ocimem_b;
  output           take_action_tracectrl;
  output           take_action_tracemem_a;
  output           take_action_tracemem_b;
  output           take_no_action_break_a;
  output           take_no_action_break_b;
  output           take_no_action_break_c;
  output           take_no_action_ocimem_a;
  output           take_no_action_tracemem_a;
  output           tdo;
  input   [ 31: 0] MonDReg;
  input   [ 31: 0] break_readreg;
  input            clk;
  input            clrn;
  input            dbrk_hit0_latch;
  input            dbrk_hit1_latch;
  input            dbrk_hit2_latch;
  input            dbrk_hit3_latch;
  input            debugack;
  input            ena;
  input   [  1: 0] ir_in;
  input            jtag_state_udr;
  input            monitor_error;
  input            monitor_ready;
  input            raw_tck;
  input            reset_n;
  input            resetlatch;
  input            rti;
  input            shift;
  input            tdi;
  input            tracemem_on;
  input   [ 35: 0] tracemem_trcdata;
  input            tracemem_tw;
  input   [  6: 0] trc_im_addr;
  input            trc_on;
  input            trc_wrap;
  input            trigbrktype;
  input            trigger_state_1;
  input            update;
  input            usr1;

  reg     [  2: 0] DRsize;
  reg              dr_update1;
  reg              dr_update2;
  reg              in_between_shiftdr_and_updatedr;
  reg     [  1: 0] ir;
  reg     [  1: 0] ir_out;
  wire             irq;
  reg     [ 37: 0] jdo;
  wire             jrst_n;
  reg              jxdr;
  reg     [ 37: 0] sr;
  wire             st_ready_test_idle;
  reg              st_shiftdr;
  reg              st_updatedr;
  reg              st_updateir;
  wire             take_action_break_a;
  wire             take_action_break_b;
  wire             take_action_break_c;
  wire             take_action_ocimem_a;
  wire             take_action_ocimem_b;
  wire             take_action_tracectrl;
  wire             take_action_tracemem_a;
  wire             take_action_tracemem_b;
  wire             take_no_action_break_a;
  wire             take_no_action_break_b;
  wire             take_no_action_break_c;
  wire             take_no_action_ocimem_a;
  wire             take_no_action_tracemem_a;
  wire             tdo;
  always @(posedge clk)
    begin
      dr_update1 <= st_updatedr;
      dr_update2 <= dr_update1;
      jxdr <= ~dr_update1 & dr_update2;
    end


  assign take_action_ocimem_a = jxdr && (ir == 2'b00) && 
    ~jdo[35] && jdo[34];

  assign take_no_action_ocimem_a = jxdr && (ir == 2'b00) && 
    ~jdo[35] && ~jdo[34];

  assign take_action_ocimem_b = jxdr && (ir == 2'b00) && 
    jdo[35];

  assign take_action_tracemem_a = jxdr && (ir == 2'b01) &&
    ~jdo[37] && 
    jdo[36];

  assign take_no_action_tracemem_a = jxdr && (ir == 2'b01) &&
    ~jdo[37] && 
    ~jdo[36];

  assign take_action_tracemem_b = jxdr && (ir == 2'b01) &&
    jdo[37];

  assign take_action_break_a = jxdr && (ir == 2'b10) && 
    ~jdo[36] && 
    jdo[37];

  assign take_no_action_break_a = jxdr && (ir == 2'b10) && 
    ~jdo[36] && 
    ~jdo[37];

  assign take_action_break_b = jxdr && (ir == 2'b10) && 
    jdo[36] && ~jdo[35] &&
    jdo[37];

  assign take_no_action_break_b = jxdr && (ir == 2'b10) && 
    jdo[36] && ~jdo[35] &&
    ~jdo[37];

  assign take_action_break_c = jxdr && (ir == 2'b10) && 
    jdo[36] &&  jdo[35] &&
    jdo[37];

  assign take_no_action_break_c = jxdr && (ir == 2'b10) && 
    jdo[36] &&  jdo[35] &&
    ~jdo[37];

  assign take_action_tracectrl = jxdr && (ir == 2'b11) &&  
    jdo[15];

  always @(posedge raw_tck or negedge jrst_n)
    begin
      if (jrst_n == 0)
          ir_out <= 2'b0;
      else 
        ir_out <= {debugack, monitor_ready};
    end


  always @(posedge raw_tck or negedge jrst_n)
    begin
      if (jrst_n == 0)
        begin
          sr <= 0;
          DRsize <= 3'b000;
        end
      else if (st_updateir)
        begin
          case (ir_in)
          
              2'b00: begin
                  DRsize <= 3'b100;
              end // 2'b00 
          
              2'b01: begin
                  DRsize <= 3'b101;
              end // 2'b01 
          
              2'b10: begin
                  DRsize <= 3'b101;
              end // 2'b10 
          
              2'b11: begin
                  DRsize <= 3'b010;
              end // 2'b11 
          
          endcase // ir_in
          ir <= ir_in;
        end
      else if (~shift & ~usr1 & ena & ~in_between_shiftdr_and_updatedr)
          case (ir)
          
              2'b00: begin
                  sr[35] <= debugack;
                  sr[34] <= monitor_error;
                  sr[33] <= resetlatch;
                  sr[32 : 1] <= MonDReg;
                  sr[0] <= monitor_ready;
              end // 2'b00 
          
              2'b01: begin
                  sr[35 : 0] <= tracemem_trcdata;
                  sr[37] <= tracemem_tw;
                  sr[36] <= tracemem_on;
              end // 2'b01 
          
              2'b10: begin
                  sr[37] <= trigger_state_1;
                  sr[36] <= dbrk_hit3_latch;
                  sr[35] <= dbrk_hit2_latch;
                  sr[34] <= dbrk_hit1_latch;
                  sr[33] <= dbrk_hit0_latch;
                  sr[32 : 1] <= break_readreg;
                  sr[0] <= trigbrktype;
              end // 2'b10 
          
              2'b11: begin
                  sr[15 : 12] <= 1'b0;
                  sr[11 : 2] <= trc_im_addr;
                  sr[1] <= trc_wrap;
                  sr[0] <= trc_on;
              end // 2'b11 
          
          endcase // ir
      else if (shift & ~usr1 & ena)
          case (DRsize)
          
              3'b000: begin
                  sr <= {tdi, sr[37 : 2], tdi};
              end // 3'b000 
          
              3'b001: begin
                  sr <= {tdi, sr[37 : 9], tdi, sr[7 : 1]};
              end // 3'b001 
          
              3'b010: begin
                  sr <= {tdi, sr[37 : 17], tdi, sr[15 : 1]};
              end // 3'b010 
          
              3'b011: begin
                  sr <= {tdi, sr[37 : 33], tdi, sr[31 : 1]};
              end // 3'b011 
          
              3'b100: begin
                  sr <= {tdi, sr[37],         tdi, sr[35 : 1]};
              end // 3'b100 
          
              3'b101: begin
                  sr <= {tdi, sr[37 : 1]};
              end // 3'b101 
          
              default: begin
                  sr <= {tdi, sr[37 : 2], tdi};
              end // default
          
          endcase // DRsize
    end


  assign tdo = sr[0];
  assign st_ready_test_idle = rti;
  always @(posedge raw_tck)
    begin
      if (~usr1 & ena & jtag_state_udr)
          jdo <= sr;
    end


  always @(posedge raw_tck or posedge update)
    begin
      if (update == 1)
        begin
          st_shiftdr <= 1'b0;
          if (usr1 & ena)
            begin
              st_updateir <= 1'b1;
              st_updatedr <= 1'b0;
            end
          else if (~usr1 & ena)
            begin
              st_updateir <= 1'b0;
              st_updatedr <= 1'b1;
            end
          else 
            begin
              st_updateir <= 1'b0;
              st_updatedr <= 1'b0;
            end
        end
      else if (shift & ~usr1 & ena)
        begin
          st_updateir <= 1'b0;
          st_updatedr <= 1'b0;
          st_shiftdr <= 1'b1;
        end
      else 
        begin
          st_updateir <= 1'b0;
          st_updatedr <= 1'b0;
          st_shiftdr <= 1'b0;
        end
    end


  always @(posedge raw_tck or negedge jrst_n)
    begin
      if (jrst_n == 0)
          in_between_shiftdr_and_updatedr <= 1'b0;
      else if (st_shiftdr)
          in_between_shiftdr_and_updatedr <= 1'b1;
      else if (st_updatedr)
          in_between_shiftdr_and_updatedr <= 1'b0;
    end



//synthesis translate_off
//////////////// SIMULATION-ONLY CONTENTS
  assign jrst_n = reset_n;

//////////////// END SIMULATION-ONLY CONTENTS

//synthesis translate_on
//synthesis read_comments_as_HDL on
//  assign jrst_n = clrn;
//synthesis read_comments_as_HDL off

endmodule

