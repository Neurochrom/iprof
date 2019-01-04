# iprof - a pretty simple and performant C++ profiling library

The iprof library let's you measure the performance of your C++ code in real time with little overhead, both in developer- and run-time.
The name is derived from InternalProfiler.

## Usage

In any function you want to measure add the IPROF_FUNC macro:
```C++
#include "iprof/iprof.hpp"

// ...

void supectedPerformanceCulprit()
{
   IPROF_FUNC;
   // ...
}
```
You can also gather stats for any scope easily:
```C++
/// ...
{
   IPROF("HeavyStuff");
   // ...
}
// ...
```
The statistics are gathered in the ```InternalProfiler::stats```.
To make iprof performant (you don't want your profiler to perturb the measurements by being slow)
the stats are not aggregated automatically. In an application that has a well defined main loop
such as a game it is recommended to gather them once every main loop iteration.
The stats can be aggregated using ```InternalProfiler::agregateEntries()```.

The stats can also be easily streamed out to text thanks to the provided << operator
```C++
InternalProfiler::agregateEntries();
std::cout << "The latest internal profiler stats:\n"
          << InternalProfiler::stats << std::endl;
```
On modern compilers that support thread_local (for objects with a constructor without crashing)
iprof also handles gathering stats across many threads:
```C++
InternalProfiler::agregateEntries();
InternalProfiler::addEntriesToAllThreadStats();
std::cout << "The latest internal profiler stats from across all threads:\n"
          << InternalProfiler::allThreadStats << std::endl;
```
The << operator formats the text in the following manner:
```
WHAT: AVG_TIME (TOTAL_TIME / TIMES_EXECUTED)
```
All times are given in micro seconds.
Sample output:
```
heavyCalc: 904026 (904026 / 1)
heavyCalc/bigWave: 148.25 (148250 / 1000)
heavyCalc/hugePower: 755.53 (755530 / 1000)
heavyCalc/hugePower/SecondPowerLoop: 32.733 (32733 / 1000)
heavyCalc/hugePower/BigWavePowerLoop: 445.48 (445480 / 1000)
heavyCalc/hugePower/FirstPowerLoop: 276.753 (276753 / 1000)
heavyCalc/hugePower/BigWavePowerLoop/bigWave: 148.288 (444863 / 3000)
```

## Building

Just add hitime.hpp, ipprof.hpp and iprof.cpp to your project.
A sample is provided in test.cpp.

To build the sample on unixish systems:
```bash
clang++ -std=c++1z -O3 test.cpp iprof.cpp -Wall -lpthread -o iprof_test.out
```
You can use g++ instead of clang++ if you so choose.

A MSVS project to build the sample is provided under the ```winbuild``` directory.

## Constributing

Just submit a pull request on gilab or github

https://gitlab.com/Neurochrom/iprof

https://github.com/Neurochrom/iprof

## License

This library is **licensed under the permissive [MIT](https://opensource.org/licenses/MIT) license**.

Copyright (c) 2015-2019 Paweł Cichocki

Enjoy ;)
