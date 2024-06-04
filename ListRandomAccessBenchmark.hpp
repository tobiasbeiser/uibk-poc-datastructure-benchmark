#pragma once
#include "Benchmark.hpp"
template <typename T>
class ListRandomAccessBenchmark : public Benchmark
{
public:
	ListRandomAccessBenchmark(int runtime, int collectionSize, int readPercentage, int insertPercentage, const std::string& dataStructureName)
		: Benchmark(runtime, collectionSize, readPercentage, insertPercentage, dataStructureName) {}
	void runBenchmark() override;
};