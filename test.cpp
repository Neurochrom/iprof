// Copyright (c) 2015-2019 Paweł Cichocki
// License: https://opensource.org/licenses/MIT

#include <math.h>
#include <future>
#include <iostream>

#include "iprof.hpp"

using namespace std;

// Senseless calculations func 1
double bigWave()
{
   IPROF_FUNC;

   double ret = 0;
   for (int i = 0; i < 10000; ++i)
      ret += sin(i/1000) - ret * 0.9;
   return ret;
}

// Senseless calculations func 2
double hugePower()
{
   IPROF_FUNC;

   double ret = 2;
   {
      IPROF("FirstPowerLoop");
      for (int i = 0; i < 5000; ++i)
      {
         double exp = (i % 10 + 1) / 7.8;
         ret = pow(ret * 1.4, exp);
      }
   }
   {
      IPROF("SecondPowerLoop");
      for (int i = 0; i < 5000; ++i)
      {
         double exp = double(((i & 8) + 1) >> 3);
         ret = pow(ret * 1.4, exp);
      }
   }
   {
      IPROF("BigWavePowerLoop");
      for (int i = 0; i < 3; ++i)
         ret -= bigWave();
   }

   return ret;
}

// Senseless calculations func 3
double heavyCalc()
{
   IPROF_FUNC;

   double ret = 0;
   for (int i = 0; i < 1000; ++i)
   {
      ret += bigWave();
      ret -= hugePower();
   }
   return ret;
}


int main()
{
   cout << "Hi ;)\n" << endl;

   cout << "sizeof(InternalProfiler::Tree): " << sizeof(InternalProfiler::Tree) << endl;

   cout << "\nAnd the lucky double is: " << heavyCalc() << endl;

   InternalProfiler::aggregateEntries();
   cout << "\nThe profiler stats so far:\n"
           "WHAT: AVG_TIME (TOTAL_TIME / TIMES_EXECUTED)"
           "\nAll times in micro seconds\n"
        << InternalProfiler::stats << endl;

   cout << "Second lucky double is " << heavyCalc() << endl;

   InternalProfiler::aggregateEntries();
   cout << "\nThe profiler stats after the second run:\n"
        << InternalProfiler::stats << endl;

#ifndef DISABLE_IPROF_MULTITHREAD
   cout << "Let's try a multithread environment" << endl;

   InternalProfiler::addEntriesToAllThreadStats();

   auto load = []
   {
      heavyCalc();
      InternalProfiler::aggregateEntries();
      InternalProfiler::addEntriesToAllThreadStats();
   };

   auto futureLucky = std::async(load);
   auto futureLucky2 = std::async(load);

   futureLucky.get();
   futureLucky2.get();

   cout << "\nThe profiler all thread stats:\n"
        << InternalProfiler::allThreadStats << endl;
#else
   cout << "iprof multithread disabled\n" << endl;
#endif

   cout << "Goodbye" << endl;
   return 0;
}
