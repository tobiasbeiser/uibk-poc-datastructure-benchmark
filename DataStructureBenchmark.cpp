#include "DataStructureBenchmark.h"

using namespace std;

template <typename T>
void runBenchmarks(int collectionSize, int readPercentage, int insertPercentage, int benchmarkTime)
{
	std::cout << "Running benchmarks for element size: " << sizeof(T) << " bytes" << std::endl;
	std::cout << "Collection size: " << collectionSize << std::endl;
	::cout << "Read percentage: " << readPercentage << "%" << std::endl;
	std::cout << "Insert percentage: " << insertPercentage << "%" << std::endl;
	std::cout << "---------------------------------" << std::endl;
	std::unique_ptr<Benchmark<T>> arrayBenchmark(new ArrayBenchmark<T>(benchmarkTime,collectionSize, readPercentage, insertPercentage));
	std::unique_ptr<Benchmark<T>> listBenchmark(new ListBenchmark<T>(benchmarkTime,collectionSize, readPercentage, insertPercentage));
	arrayBenchmark->runBenchmark();
	std::cout << "---------------------------------" << std::endl;
	listBenchmark->runBenchmark();
}

int main(int argc, char *argv[])
{
	if (argc != 6)
	{
		std::cout << "Invalid parameters! Usage:\n"
				  << argv[0] << "<elementSize> <collectionSize> <insert/delete percentage> <read/write percentage> <benchmark time>";
		std::cout << "Element size:}\n 1: 8 bytes\n 2: 512 bytes\n 3: 8MB\n";
		std::cout << "Example: " << argv[0] << "1 1000 10 90" << std::endl;
		return 1;
	}

	int elementSize = atoi(argv[1]);
	int collectionSize = atoi(argv[2]);
	int readPercentage = atoi(argv[3]);
	int insertPercentage = atoi(argv[4]);
	int benchmarkTime = atoi(argv[5]);

	if (insertPercentage + readPercentage != 100)
	{
		std::cout << "Invalid parameters! Insert and read percentages must add up to 100" << std::endl;
		return 1;
	}

	if(insertPercentage > readPercentage)
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
		runBenchmarks<Element8Bytes>(collectionSize, readPercentage, insertPercentage, benchmarkTime);
		break;
	case 2:
		runBenchmarks<Element512Bytes>(collectionSize, readPercentage, insertPercentage, benchmarkTime);
		break;
	case 3:
		runBenchmarks<Element8MB>(collectionSize, readPercentage, insertPercentage, benchmarkTime);
		break;
	default:
		break;
	}

	return 0;
}
