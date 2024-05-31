#pragma once
#include "Benchmark.h"
template<typename T>
class ArrayBenchmark : public Benchmark<T>
{
public:
	ArrayBenchmark(int runtime, int collectionSize, int readPercentage, int insertPercentage) : Benchmark<T>(runtime, collectionSize, readPercentage, insertPercentage) {}
	void runBenchmark() override;
};