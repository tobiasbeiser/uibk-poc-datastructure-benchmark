# UIBK - Performance-oriented Computing - DataStructure Benchmark

This tool is used to benchmark different data structures for the course "VU-Performance Oriented Computing SS24" at UIBK.

## Building
```bash
mkdir build && cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Releaes && ninja
```
## Running Benchmarks
To run the benchmarks for all combinations of the following parameters you can run the provided python script

`python3 run_benchmark.py build/DataStructureBenchmark`

**Instruction Mix**:
| ins/del | read/write |
| -------:| ----------:|
|     0%  |      100%  |
|     1%  |       99%  |
|    10%  |       90%  |
|    50%  |       50%  |

**Element Size**: 8 Byte, 512 Byte, 8 MB

**Number of Elements**: 10, 1000, 100000, 10000000

Additionally the script will know if it is run on the LCC3 cluster and submit jobs with the benchmarks wia SLURM.

Note, if any data structure fails to allocate enough memory, the current run crashes, leaving other data structures in the program untested.




## Adding new Data Structures
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
#include "YourDataStructureBenchmark.hpp"

template void ArrayBenchmark<Element8Bytes>::runBenchmark();
template void ArrayBenchmark<Element512Bytes>::runBenchmark();
template void ArrayBenchmark<Element8MB>::runBenchmark();

template <typename T>
void YourDataStructureBenchmark<T>::runBenchmark()
{
    //TODO: Initialize your collection here
    std::vector<T> collection(this->collectionSize);

    // reset counters before every benchmark to ensure correct stats
	this->resetCounters();
	    
    const auto end = std::chrono::high_resolution_clock::now() + std::chrono::seconds(this->runtime);
    
	bool run = true;

	while (run)
	{
        //TODO: Depending on your collection type change the iterator
		for (int i = 0; i < this->collectionSize;)
		{
			if (std::chrono::high_resolution_clock::now() > end)
			{
				run = false;
				break;
			}

            //TODO: Add your implementation here
            //Read/Writes for range this->getPadding1() 
            //1xInsert
            //Read/Writes for range this->getPadding2()
            //1xDelete
            //make sure to check the outer loop variable before executing any of these actions
            //make sure to increment the outer variable and the corresponding counter after each action
            //pay attention to create the objects for insertions directly on the stack

			
		}
	}
    // always print results in the end to be read by the python script
    this->printResults();
}
```


Finally, create a new instance of your new class in the `DataStructureBenchmark.cpp:runBenchmarks` function.
```c++
std::unique_ptr<Benchmark<T>> yourBenchmark(new YourDataStructureBenchmark<T>(benchmarkTime,collectionSize, readPercentage, insertPercentage));
yourBenchmark->runBenchmark();
```