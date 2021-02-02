#ifndef SHM_COM_H_INCLUDED
#define SHM_COM_H_INCLUDED

// Only check GCC, since that is the only one I will be working with.
// Won't work with pre-ANSI but ANSI C is the de-facto standard.
#if __GNUC__
   #if defined(__LP64__) || defined(_LP64)
      #define ENV_64_BIT
   #else
      #define ENV_32_BIT
   #endif
   // Not always true, but I don't care about other environments
#else
   #define ENV_32_BIT
#endif // __GNUC__

#define TEXT_SZ 2048

struct shared_use_st {
   int written_by_you;
   char some_text[TEXT_SZ];
};


#endif // SHM_COM_H_INCLUDED
