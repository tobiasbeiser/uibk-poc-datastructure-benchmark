#pragma once
#include "Elements.h"
#include <iostream>
#include <chrono>
class BenchmarkBase
{
public:
    BenchmarkBase(int runtime, int collectionSize, int readPercentage, int insertPercentage)
        : runtime(runtime), collectionSize(collectionSize), readPercentage(readPercentage), insertPercentage(insertPercentage), padding(getPadding()) {}
    virtual void runBenchmark() = 0;
    virtual ~BenchmarkBase() = default;

protected:
    const int runtime;
    int readWriteOperations;
    int insertDeleteOperations;

    const int collectionSize;
    const int readPercentage;
    const int insertPercentage;
    const int padding;

    void printResults();
    void resetCounters();

private:
    int getPadding();
};
