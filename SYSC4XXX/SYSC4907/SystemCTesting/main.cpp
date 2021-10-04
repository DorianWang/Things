#include <iostream>

#include "systemc.h"

// First SystemC stuff use: http://www.asic-world.com/systemc/first1.html#Hello_World_Program

using std::cout;
using std::endl;

SC_MODULE (hello_world){
   SC_CTOR (hello_world){
      // Empty for now.
   }
   void say_hello(){
      cout << "Hello World!" << endl;
   }
};


int main()
{
   hello_world hello("HELLO");
   hello.say_hello();
   // Should return 201402L, library doesn't work with different versions.
   cout << __cplusplus << endl;
   return 0;
}






