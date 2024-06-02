#pragma once
#include "Benchmark.hpp"
#include <vector>
#include <any>
#include <cmath>
template <typename T>
class TieredArrayBenchmark : public Benchmark
{
public:
	TieredArrayBenchmark(int runtime, int collectionSize, int readPercentage, int insertPercentage, const std::string& dataStructureName) 
    : Benchmark(runtime, collectionSize, readPercentage, insertPercentage, dataStructureName) {}
	void runBenchmark() override;
};