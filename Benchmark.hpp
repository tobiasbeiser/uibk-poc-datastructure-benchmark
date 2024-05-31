#pragma once
#include "Elements.hpp"
#include <iostream>
#include <chrono>
class Benchmark
{
public:
	Benchmark(int runtime, int collectionSize, int readPercentage, int insertPercentage, const std::string &dataStructureName)
		: runtime(runtime), collectionSize(collectionSize), readPercentage(readPercentage), insertPercentage(insertPercentage), dataStructureName(dataStructureName), padding(getPadding()) {}
	virtual ~Benchmark() = default;
	
    virtual void runBenchmark() = 0;

protected:
    const int runtime;
    int readWriteOperations;
    int insertDeleteOperations;

    const int collectionSize;
    const int readPercentage;
    const int insertPercentage;

    int getPadding1();
    int getPadding2();

    void printResults(double duration);
    void resetCounters();

private:
    int getPadding();
    const int padding;
    const std::string dataStructureName;

};
