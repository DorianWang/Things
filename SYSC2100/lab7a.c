#include <hc12dp256.h>


void initHW();
void RTIISR();

char lightsOn = 0x00;


void main(void)
{
   initHW();
   while (1) { };
}

void initHW( void )
{
  // Init Hardware
  DDRK = 0x2F;
  DDRT = 0x60;
  DDRP = 0x20;

  CRGFLG = CRGFLG;



  RTICTL = 0x3F;
}

#pragma interrupt_handler RTIISR
void RTIISR(void)
{
  // TBD
}

























