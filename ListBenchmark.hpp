#pragma once
#include "Benchmark.hpp"
template <typename T>
class ListBenchmark : public Benchmark<T>
{
public:
	ListBenchmark(int runtime, int collectionSize, int readPercentage, int insertPercentage) 
	: Benchmark<T>(runtime, collectionSize, readPercentage, insertPercentage) {}
	void runBenchmark() override;
};