#pragma once
#include "Benchmark.hpp"
#include <vector>
template <typename T>
class UnrolledLinkedListBenchmark : public Benchmark
{
    int chunkSize;
public:
    UnrolledLinkedListBenchmark(int runtime, int collectionSize, int chunkSize, int readPercentage, int insertPercentage, const std::string& dataStructureName)
            : Benchmark(runtime, collectionSize, readPercentage, insertPercentage, dataStructureName) { this->chunkSize = chunkSize;}
    void runBenchmark() override;
};