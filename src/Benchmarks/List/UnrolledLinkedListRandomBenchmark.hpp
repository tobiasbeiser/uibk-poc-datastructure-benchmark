#pragma once
#include "Benchmark.hpp"
#include "Types/UnrolledLinkedList.hpp"
template <typename T>
class UnrolledLinkedListRandomBenchmark : public Benchmark
{
    int chunkSize;
public:
    UnrolledLinkedListRandomBenchmark(int runtime, int collectionSize, int chunkSize, int readPercentage, int insertPercentage, const std::string& dataStructureName)
            : Benchmark(runtime, collectionSize, readPercentage, insertPercentage, dataStructureName) { this->chunkSize = chunkSize;}
    void runBenchmark() override;
};