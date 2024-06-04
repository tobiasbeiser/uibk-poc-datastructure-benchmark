#include "TieredArrayBenchmark.hpp"


template void TieredArrayBenchmark<Element8Bytes>::runBenchmark();
template void TieredArrayBenchmark<Element512Bytes>::runBenchmark();
template void TieredArrayBenchmark<Element8MB>::runBenchmark();

template <typename T>
void TieredArrayBenchmark<T>::runBenchmark()
{
    
	this->resetCounters();
	TieredArray<T> collection(this->collectionSize, this->chunkSize);
    
	const auto end = std::chrono::high_resolution_clock::now() + std::chrono::seconds(this->runtime);
	bool run = true;
	while (run)
	{
		for (int i = 0; i < collection.size();)
		{
			if (std::chrono::high_resolution_clock::now() > end)
			{
				run = false;
				break;
			}
			for (int k = 0; k < this->getPadding1() && i < collection.size() - 1; k += 2)
			{
                
				// read
                char charData = collection.at(i).data[0];
				charData++;
				i++;
				this->readWriteOperations++;

				// write
				collection.at(i).data[0] = 0;
				i++;
				this->readWriteOperations++;
			}

			if (this->insertPercentage > 0 && i < collection.size())
			{
				// insert
				collection.insert(i);
				this->insertDeleteOperations++;
				i++;
			}
			for (int k = 0; k < this->getPadding2() && i < collection.size() - 1; k += 2)
			{
				// read
				char data = collection.at(i).data[0];
				data++;
				i++;
				this->readWriteOperations++;

				// write
				collection.at(i).data[0] = 0;
				i++;
				this->readWriteOperations++;
			}



			if (this->insertPercentage > 0 && i < collection.size())
			{
				// delete
				collection.del(i);
				i++;
				this->insertDeleteOperations++;
			}

		}
	}
	this->printResults();
}