#include "ArrayBenchmark.hpp"
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

			for (int k = 0; k < this->getPadding1() && i < this->collectionSize; k+=2)
			{
				// read
				char data = collection.at(i).data[0];
				data++;
				i++;
				this->readWriteOperations++;

				// write
				collection[i].data[0] = 0;
				i++;
				this->readWriteOperations++;
			}

			
			
			if (this->insertPercentage > 0 && i < this->collectionSize)
			{
				// insert
				T newElement = T();
				collection.push_back(newElement);
				this->insertDeleteOperations++;
				i++;
			}

			for (int k = 0; k < this->getPadding2() && i < this->collectionSize; k+=2)
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
			if (this->insertPercentage > 0 && i < this->collectionSize)
			{
				collection.pop_back();
				i++;
				this->insertDeleteOperations++;
			}
		}
	}
	this->printResults();
}