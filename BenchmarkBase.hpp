#pragma once
#include "Elements.hpp"
#include <iostream>
#include <chrono>
class BenchmarkBase
{
public:
    BenchmarkBase(int runtime, int collectionSize, int readPercentage, int insertPercentage, const std::string& dataStructureName)
        : runtime(runtime), collectionSize(collectionSize), readPercentage(readPercentage), insertPercentage(insertPercentage), dataStructureName(dataStructureName), padding(getPadding()) {}
    virtual void runBenchmark() = 0;
    virtual ~BenchmarkBase() = default;

protected:
    const int runtime;
    int readWriteOperations;
    int insertDeleteOperations;

    int collectionSize;
    const int readPercentage;
    const int insertPercentage;
    
    int getPadding1();
    int getPadding2();

    void printResults();
    void resetCounters();

private:
    int getPadding();
    const int padding;
    std::string dataStructureName;
};
