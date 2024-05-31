# uibk-poc-datastructure-benchmark
Benchmarking tool for different data structures for the course "VU-Performance Oriented Computing SS24" at UIBK

## Extending
To add a new data structure to the benchmark you have to create a new class. Classes in C++ consist of a `.h` and a `.cpp` file. The header file tells us whats in the class and the `.cpp` file implements the required functions.

### Example

`YourDataStructureBenchmark.h`
```c++
#pragma once
#include "Benchmark.h"
template<typename T>
class YourDataStructureBenchmark.h : public Benchmark<T>
{
public:
	YourDataStructureBenchmark(int runtime, int collectionSize, int readPercentage, int insertPercentage) 
    : Benchmark<T>(runtime, collectionSize, readPercentage, insertPercentage) {}
	void runBenchmark() override;
};
```


`YourDataStructureBenchmark.cpp`
```c++

```
