#pragma once
#include "BenchmarkBase.h"
template <typename T>
class Benchmark : public BenchmarkBase
{
public:
	Benchmark(int runtime, int collectionSize, int readPercentage, int insertPercentage)
		: BenchmarkBase(runtime, collectionSize, readPercentage, insertPercentage) {}
	virtual ~Benchmark() = default;
	virtual void runBenchmark() override= 0;

};
