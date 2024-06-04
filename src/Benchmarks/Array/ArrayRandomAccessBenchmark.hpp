#pragma once
#include "Benchmark.hpp"
#include <vector>
template <typename T>
class ArrayRandomAccessBenchmark : public Benchmark
{
public:
	ArrayRandomAccessBenchmark(int runtime, int collectionSize, int readPercentage, int insertPercentage, const std::string& dataStructureName)
		: Benchmark(runtime, collectionSize, readPercentage, insertPercentage, dataStructureName) {}
	void runBenchmark() override;
};