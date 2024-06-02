#pragma once
#include "Benchmark.hpp"
#include <vector>
template <typename T>
class UnrolledLinkedListBenchmark : public Benchmark
{
public:
    UnrolledLinkedListBenchmark(int runtime, int collectionSize, int readPercentage, int insertPercentage, const std::string& dataStructureName)
            : Benchmark(runtime, collectionSize, readPercentage, insertPercentage, dataStructureName) {}
    void runBenchmark() override;
};