//Here is a modified source for Bit_Adder.h which emulates the delay between component/modules:


#ifndef BIT_ADDER_H_
#define BIT_ADDER_H_

#include <systemc>
#include <queue>

template<typename T = bool>
SC_MODULE(BIT_ADDER)
{
  public:
    sc_core::sc_in<T> a, b, cin;
    sc_core::sc_out<T> sum, cout;

    SC_CTOR(BIT_ADDER):
      a("a")
      , b("b")
      , ci("cin")
      , sum("sum")
      , cout("cout")
      , delay(sc_core::sc_time(2, sc_core::SC_NS))
      , eqSum("eqSum")
      , eqCarry("eqCarry")
    {
      SC_METHOD(do_add);
      sensitive << a << b << cin;
      dont_initialize();

      SC_METHOD(drive_sum);
      sensitive << eqSum;
      dont_initialize();

      SC_METHOD(drive_carry);
      sensitive << eqCarry;
      dont_initialize();
    }

    void do_add(void)
    {
      T valA = a.read();
      T valB = b.read();
      T valCi = cin.read();

      T tmpCo = (valA & valB) | (valB & valCi) | (valA & valCi);
      T tmpSum = valA ^ valB ^ valCi;

      coq.push(tmpCo);
      sumq.push(tmpSum);

      eqSum.notify(delay);
      eqCarry.notify(delay);
    }

    void drive_sum(void)
    {
      T valSum = sumq.front();
      sum.write(valSum);
      sumq.pop();
    }

    void drive_carry(void)
    {
      T valCarry = coq.front();
      cout.write(valCarry);
      coq.pop();
    }

  private:
    sc_core::sc_time delay;
    sc_core::sc_event_queue eqSum;
    sc_core::sc_event_queue eqCarry;
    std::queue<T> sumq;
    std::queue<T> coq;

}; // BIT_ADDER

#endif // BIT_ADDER_H_

This should be drop-in replacement for your module.

One thing you will have to change is the declaration where it would change something from:

BIT_ADDER bitAdder

to:

BIT_ADDER<sc_logic> bitAdder

Let us know if you need further clarification on how the module behavior is modeled.

Regards,

Ameya Vikram Singh
//https://forums.accellera.org/topic/5715-delaying-simulated-execution/

// See also:
// https://stackoverflow.com/questions/62956845/is-there-a-better-alternate-way-to-implement-delay-in-systemc-modules

