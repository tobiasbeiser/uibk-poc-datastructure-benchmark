#include "ArrayBenchmark.h"
#include <vector>
#include <memory>

template void ArrayBenchmark<Element8Bytes>::runBenchmark();
template void ArrayBenchmark<Element512Bytes>::runBenchmark();
template void ArrayBenchmark<Element8MB>::runBenchmark();

template <typename T>
void ArrayBenchmark<T>::runBenchmark()
{
	this->resetCounters();
	std::vector<T> collection(this->collectionSize);
	auto end = std::chrono::high_resolution_clock::now() + std::chrono::seconds(this->runtime);
	bool run = true;
	while (run)
	{
		for (int i = 0; i < this->collectionSize;)
		{
			if (std::chrono::high_resolution_clock::now() > end)
			{
				run = false;
				break;
			}
			if (this->insertPercentage > 0)
			{
				// insert
				T newElement = T();
				collection.push_back(newElement);
				this->insertDeleteOperations++;
				i++;
			}

			for (int k = 0; k < this->padding; k++)
			{
				// read
				char data = collection[i].data[0];
				data++;
				i++;
				this->readWriteOperations++;
				// write
				collection[i].data[0] = 0;
				i++;
				this->readWriteOperations++;
			}

			// delete
			if (this->insertPercentage > 0)
			{
				collection.pop_back();
				i++;
				this->insertDeleteOperations++;
			}
		}
	}
	this->printResults();
	// std::cout << "Padding: " << padding << std::endl;
	// std::cout << "Padding: " << padding << std::endl;
	// std::cout << readWriteOperations << " read operations" << std::endl;
	// std::cout << insertDeleteOperations << " insert operations" << std::endl;
	// std::cout << "Array Benchmark:" << std::endl;
	// std::cout << readWriteOperations + insertDeleteOperations << " / " << operations << std::endl;
	// std::cout << "Read/Write Operations: " << readWriteOperations << std::endl;
	// std::cout << "Insert/Delete Operations: " << insertDeleteOperations << std::endl;
}