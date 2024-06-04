#pragma once
#include "Benchmark.hpp"
#include "Types/TieredArray.hpp"
#include <vector>
#include <any>
#include <cmath>


template <typename T>
class TieredArrayRandomBenchmark : public Benchmark
{
    int chunkSize;
public:
	TieredArrayRandomBenchmark(int runtime, int collectionSize, int chunkSize, int readPercentage, int insertPercentage, const std::string& dataStructureName)
    : Benchmark(runtime, collectionSize, readPercentage, insertPercentage, dataStructureName) { this->chunkSize = chunkSize;}
	void runBenchmark() override;
};