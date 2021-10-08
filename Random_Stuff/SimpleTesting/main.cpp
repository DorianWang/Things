#include <iostream>
#include <map>
#include <unordered_map>
#include <string>

#include <chrono> // Timing
#include <cstdint> // Integer types
#include <random> // mersenne twister

// For getting memory usage
#include "windows.h"
#include "psapi.h"


// https://stackoverflow.com/questions/27975715/does-size-of-array-in-c-affect-time-to-access-or-change-a-piece-of-data-in-the-a

using namespace std;
 using std::chrono::high_resolution_clock;
 using std::chrono::duration_cast;
 using std::chrono::duration;
 using std::chrono::microseconds;

int main()
{
   PROCESS_MEMORY_COUNTERS_EX pmc;
   GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
   SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
   SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
   size_t total = virtualMemUsedByMe + physMemUsedByMe;
   size_t previousMem = total;

   std::map<uint_fast64_t, std::string> test1;
   std::unordered_map<uint_fast64_t, std::string> test2;
   uint_fast64_t simple_set[400]; // The amount of folders we will likely be working with.
   uint_fast64_t* fir_set = new uint_fast64_t[2000]; // The maximum number of folders to work with.
   cout << sizeof(uint_fast64_t) << " " << sizeof(uint_fast32_t) << endl;
   cout << sizeof(test1) << " " << sizeof(test2) << endl;
   mersenne_twister_engine<uint_fast64_t, 64, 312, 156, 31,
                             0xb5026f5aa96619e9, 29,
                             0x5555555555555555, 17,
                             0x71d67fffeda60000, 37,
                             0xfff7eee000000000, 43, 6364136223846793005> RNG;
   RNG.seed(0);

   auto t1 = high_resolution_clock::now();
   // Filling the arrays
   for (size_t i = 0; i < 400; i++){
      simple_set[i] = RNG();
   }
   cout << simple_set[0] << " " << simple_set[399] << endl;
   for (size_t i = 0; i < 2000; i++){
      fir_set[i] = RNG();
   }
   auto t2 = high_resolution_clock::now();
   auto us_int = duration_cast<microseconds>(t2 - t1);
   cout << "Time to generate random numbers: " << us_int.count() << "us" << endl;


   GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
   virtualMemUsedByMe = pmc.PrivateUsage;
   physMemUsedByMe = pmc.WorkingSetSize;
   total = virtualMemUsedByMe + physMemUsedByMe;
   cout << "Memory use increased by: " << (total - previousMem) << endl;
   previousMem = total;
   cout << total << " | " << virtualMemUsedByMe << " + " << physMemUsedByMe << endl;

   // Testing map population
   t1 = high_resolution_clock::now();
   for (size_t i = 0; i < 400; i++){
      test1.emplace(simple_set[i], "");
   }
   for (size_t i = 0; i < 2000; i++){
      test1.emplace(fir_set[i], "");
   }
   t2 = high_resolution_clock::now();
   us_int = duration_cast<microseconds>(t2 - t1);
   cout << "Time to fill map with 2400: " << us_int.count() << "us" << endl;

   // Get memory usage here;
   GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
   virtualMemUsedByMe = pmc.PrivateUsage;
   physMemUsedByMe = pmc.WorkingSetSize;
   total = virtualMemUsedByMe + physMemUsedByMe;
   cout << "Memory use increased by: " << (total - previousMem) << endl;
   previousMem = total;
   cout << total << " | " << virtualMemUsedByMe << " + " << physMemUsedByMe << endl;

   // Testing unordered map population
   t1 = high_resolution_clock::now();
   for (size_t i = 0; i < 400; i++){
      test2.emplace(simple_set[i], "");
   }
   for (size_t i = 0; i < 2000; i++){
      test2.emplace(fir_set[i], "");
   }
   t2 = high_resolution_clock::now();
   us_int = duration_cast<microseconds>(t2 - t1);
   cout << "Time to fill umap with 2400: " << us_int.count() << "us" << endl;

   // Get memory usage here;
   GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
   virtualMemUsedByMe = pmc.PrivateUsage;
   physMemUsedByMe = pmc.WorkingSetSize;
   total = virtualMemUsedByMe + physMemUsedByMe;
   cout << "Memory use increased by: " << (total - previousMem) << endl;
   previousMem = total;
   cout << total << " | " << virtualMemUsedByMe << " + " << physMemUsedByMe << endl;

   // "Random" access
   t1 = high_resolution_clock::now();
   for (size_t i = 0; i < 400; i++){
      test1[fir_set[simple_set[i] % 2000]] = std::string("Changed but also long enough to avoid SSO!");
   }
   t2 = high_resolution_clock::now();
   us_int = duration_cast<microseconds>(t2 - t1);
   cout << "Time to access and modify 400 map entries: " << us_int.count() << "us" << endl;

   // Get memory usage here;
   GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
   virtualMemUsedByMe = pmc.PrivateUsage;
   physMemUsedByMe = pmc.WorkingSetSize;
   total = virtualMemUsedByMe + physMemUsedByMe;
   cout << "Memory use increased by: " << (total - previousMem) << endl;
   previousMem = total;
   cout << total << " | " << virtualMemUsedByMe << " + " << physMemUsedByMe << endl;

   t1 = high_resolution_clock::now();
   for (size_t i = 0; i < 400; i++){
      test2[fir_set[simple_set[i] % 2000]] = std::string("Changed but also long enough to avoid SSO!");
   }
   t2 = high_resolution_clock::now();
   us_int = duration_cast<microseconds>(t2 - t1);
   cout << "Time to access and modify 400 umap entries: " << us_int.count() << "us" << endl;

   GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
   virtualMemUsedByMe = pmc.PrivateUsage;
   physMemUsedByMe = pmc.WorkingSetSize;
   total = virtualMemUsedByMe + physMemUsedByMe;
   cout << "Memory use increased by: " << (total - previousMem) << endl;
   previousMem = total;
   cout << total << " | " << virtualMemUsedByMe << " + " << physMemUsedByMe << endl;


   delete [] fir_set;
   cout << "Hello world!" << endl;
   return 0;
}
