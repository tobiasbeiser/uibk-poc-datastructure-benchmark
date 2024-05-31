#pragma once
#include "BenchmarkBase.hpp"
template <typename T>
class Benchmark : public BenchmarkBase
{
public:
	Benchmark(int runtime, int collectionSize, int readPercentage, int insertPercentage, const std::string &dataStructureName)
		: BenchmarkBase(runtime, collectionSize, readPercentage, insertPercentage, dataStructureName) {}
	virtual ~Benchmark() = default;
	virtual void runBenchmark() override= 0;

};
