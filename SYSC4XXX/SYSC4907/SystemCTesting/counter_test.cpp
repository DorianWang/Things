//-----------------------------------------------------
// Using file from http://www.asic-world.com/systemc/first1.html#Hello_World_Program
// This is my second Systemc Example
// Design Name : first_counter
// File Name : first_counter.cpp
// Function : This is a 4 bit up-counter with
// Synchronous active high reset and
// with active high enable signal
//-----------------------------------------------------
#include "systemc.h"

class first_counter : public sc_module {
   public:
      sc_in_clk     clock ;      // Clock input of the design
      sc_in<bool>   reset ;      // active high, synchronous Reset input
      sc_in<bool>   enable;      // Active high enable signal for counter
      sc_out<sc_uint<4> > counter_out; // 4 bit vector output of the counter

      //------------Local Variables Here---------------------
      sc_uint<4>	count;

      //------------Code Starts Here-------------------------
      // Below function implements actual counter logic
      void incr_count () {
       // At every rising edge of clock we check if reset is active
       // If active, we load the counter output with 4'b0000
       if (reset.read() == 1) {
         count =  0;
         counter_out.write(count);
       // If enable is active, then we increment the counter
       } else if (enable.read() == 1) {
         count = count + 1;
         counter_out.write(count);
         cout<<"@" << sc_time_stamp() <<" :: Incremented Counter "
           <<counter_out.read()<<endl;
       }
      } // End of function incr_count

      // Constructor for the counter
      // Since this counter is a positive edge trigged one,
      // We trigger the below block with respect to positive
      // edge of the clock and also when ever reset changes state
      SC_CTOR(first_counter) {
         cout<<"Executing new"<<endl;
         SC_METHOD(incr_count); // Run incr_counter() as SystemC function.
         // https://www.doulos.com/knowhow/systemc/systemc-tutorial/modules-and-processes/
         sensitive << reset;
         sensitive << clock.pos();
      } // End of Constructor

}; // End of Module counter








// Ideas to make them more like C++
// https://stackoverflow.com/questions/45308468/systemc-multiple-module-implementations-in-single-cpp-file
// https://stackoverflow.com/questions/18692827/how-to-use-header-file-in-systemc-hello-world-program
// https://www.doulos.com/knowhow/systemc/systemc-faq/#q2
// http://cfs-vision.com/2017/09/11/learning-systemc-002-module-sc_module/#template_for_coding_in_systemc






