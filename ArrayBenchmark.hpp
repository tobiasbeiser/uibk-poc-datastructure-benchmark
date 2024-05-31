#pragma once
#include "Benchmark.hpp"
template <typename T>
class ArrayBenchmark : public Benchmark<T>
{
public:
	ArrayBenchmark(int runtime, int collectionSize, int readPercentage, int insertPercentage, const std::string& dataStructureName)
		: Benchmark<T>(runtime, collectionSize, readPercentage, insertPercentage, dataStructureName) {}
	void runBenchmark() override;
};