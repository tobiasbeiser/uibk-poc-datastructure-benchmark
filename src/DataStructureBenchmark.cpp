﻿#include "DataStructureBenchmark.hpp"

using namespace std;

template <typename T>
void runBenchmarks(int collectionSize, int readPercentage, int insertPercentage, int benchmarkTime, bool printInfo)
{
	if (printInfo)
	{

		std::cout << "Running benchmarks for element size: " << sizeof(T) << " bytes" << std::endl;
		std::cout << "Collection size: " << collectionSize << std::endl;
		std::cout << "Read percentage: " << readPercentage << "%" << std::endl;
		std::cout << "Insert percentage: " << insertPercentage << "%" << std::endl;
		std::cout << "---------------------------------" << std::endl;
	}
	std::vector<unique_ptr<Benchmark>> benchmarks;
	benchmarks.push_back(std::make_unique<ArrayBenchmark<T>>(benchmarkTime, collectionSize, readPercentage, insertPercentage, "std::vector"));
	benchmarks.push_back(std::make_unique<ArrayRandomAccessBenchmark<T>>(benchmarkTime, collectionSize, readPercentage, insertPercentage, "std::vector_random"));
	benchmarks.push_back(std::make_unique<ListBenchmark<T>>(benchmarkTime, collectionSize, readPercentage, insertPercentage, "std::forward_list"));
	benchmarks.push_back(std::make_unique<ListRandomAccessBenchmark<T>>(benchmarkTime, collectionSize, readPercentage, insertPercentage, "std::forward_list_random"));
	for (int i = 2; i <= 16; i *= 2) {
		benchmarks.push_back(std::make_unique<TieredArrayBenchmark<T>>(benchmarkTime, collectionSize, i, readPercentage, insertPercentage, "TieredArrayChunkSize" + std::to_string(i)));
		benchmarks.push_back(std::make_unique<TieredArrayRandomBenchmark<T>>(benchmarkTime, collectionSize, i, readPercentage, insertPercentage, "TieredArrayRandomChunkSize" + std::to_string(i)));
	}
	for (int i = 8; i <= 32; i *= 2) {
		benchmarks.push_back(std::make_unique<UnrolledLinkedListBenchmark<T>>(benchmarkTime, collectionSize, i, readPercentage, insertPercentage, "UnrolledLinkedListChunkSize" + std::to_string(i)));
		benchmarks.push_back(std::make_unique<UnrolledLinkedListRandomBenchmark<T>>(benchmarkTime, collectionSize, i, readPercentage, insertPercentage, "UnrolledLinkedListRandomChunkSize" + std::to_string(i)));
	}

	// run all the benchmarks
	for (auto& benchmark : benchmarks) {
		benchmark->runBenchmark();
	}

}

int main(int argc, char* argv[])
{

	if (argc != 6)
	{
		std::cout << "Invalid parameters! Usage:\n"
			<< argv[0] << "<elementSize> <collectionSize> <insert/delete percentage> <read/write percentage> <benchmark time>";
		std::cout << "Element size:}\n 1: 8 bytes\n 2: 512 bytes\n 3: 8MB\n";
		std::cout << "Example: " << argv[0] << " 1 1000 10 90" << std::endl;
		return 1;
	}

	int elementSize = atoi(argv[1]);
	int collectionSize = atoi(argv[2]);
	int readPercentage = atoi(argv[3]);
	int insertPercentage = atoi(argv[4]);
	int benchmarkTime = atoi(argv[5]);
	bool printInfo = false;

	if (insertPercentage + readPercentage != 100)
	{
		std::cout << "Invalid parameters! Insert and read percentages must add up to 100" << std::endl;
		return 1;
	}

	if (insertPercentage > readPercentage)
	{
		std::cout << "Invalid parameters! Insert percentage must be smaller than read percentage" << std::endl;
		return 1;
	}

	if (elementSize < 1 || elementSize > 3)
	{
		std::cout << "Invalid parameters! Element size must be between 1 and 3" << std::endl;
		return 1;
	}

	switch (elementSize)
	{
	case 1:
		runBenchmarks<Element8Bytes>(collectionSize, readPercentage, insertPercentage, benchmarkTime, printInfo);
		break;
	case 2:
		runBenchmarks<Element512Bytes>(collectionSize, readPercentage, insertPercentage, benchmarkTime, printInfo);
		break;
	case 3:
		runBenchmarks<Element8MB>(collectionSize, readPercentage, insertPercentage, benchmarkTime, printInfo);
		break;
	default:
		break;
	}

	return 0;
}
