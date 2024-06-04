#pragma once
#include "Benchmark.hpp"
#include <vector>
template <typename T>
class ArrayBenchmark : public Benchmark
{
public:
	ArrayBenchmark(int runtime, int collectionSize, int readPercentage, int insertPercentage, const std::string& dataStructureName)
		: Benchmark(runtime, collectionSize, readPercentage, insertPercentage, dataStructureName) {}
	void runBenchmark() override;
};