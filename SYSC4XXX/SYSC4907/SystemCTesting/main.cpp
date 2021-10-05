#include <iostream>

#include "systemc.h"
#include "counter_test.cpp"

// First SystemC stuff use: http://www.asic-world.com/systemc/first1.html#Hello_World_Program
using std::cout;
using std::endl;

// If no main in program but "systemc.h" is included
// sc_main will be run instead like a main function.
int sc_main (int argc, char* argv[])
{
   sc_signal<bool> clock;
   sc_signal<bool> reset;
   sc_signal<bool> enable;
   sc_signal<sc_uint<4>> counter_out;
   size_t i = 0;

   first_counter counter("COUNTER");
   counter.clock(clock);
   counter.reset(reset);
   counter.enable(enable);
   counter.counter_out(counter_out);

   // Starts scheduler, then runs 1 delta cycle, still don't quite understand.
   // See section "4.3.4.2 Function sc_start" in IEEE Std 1666-2011.
   // https://ieeexplore.ieee.org/document/6134619, appears to be latest standard.
   sc_start(0, SC_NS); // SC_NS is from enum sc_time_unit in sc_time.h. It has a value of 2.

   sc_trace_file* wf = sc_create_vcd_trace_file("counter");
   sc_trace(wf, clock, "clock");
   sc_trace(wf, reset, "reset");
   sc_trace(wf, enable, "enable");
   sc_trace(wf, counter_out, "count");

   reset = 0;
   enable = 0;
   for (i = 0; i < 5; i++){
      clock = 0;
      sc_start(1, SC_NS);
      clock = 1;
      sc_start(1, SC_NS);
   }

   reset = 1;
   cout << "@" << sc_time_stamp() << " || Activate reset" << endl;
   for (i = 0; i < 10; i++){
      clock = 0;
      sc_start(1, SC_NS);
      clock = 1;
      sc_start(1, SC_NS);
   }

   reset = 0;
   cout << "@" << sc_time_stamp() << " || Deactivate reset" << endl;
   for (i = 0; i < 5; i++){
      clock = 0;
      sc_start(1, SC_NS);
      clock = 1;
      sc_start(1, SC_NS);
   }

   enable = 1;
   cout << "@" << sc_time_stamp() << " || Activate enable" << endl;
   for (i = 0; i < 20; i++){
      clock = 0;
      sc_start(1, SC_NS);
      clock = 1;
      sc_start(1, SC_NS);
   }

   enable = 0;
   cout << "@" << sc_time_stamp() << " || Deactivate enable" << endl;
   cout << "@" << sc_time_stamp() << " || Ending sumulation" << endl;
   sc_close_vcd_trace_file(wf);
   return 0;
}






