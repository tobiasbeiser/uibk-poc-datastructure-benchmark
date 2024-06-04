#include "ArrayBenchmark.hpp"
//#include <memory>

template void ArrayBenchmark<Element8Bytes>::runBenchmark();
template void ArrayBenchmark<Element512Bytes>::runBenchmark();
template void ArrayBenchmark<Element8MB>::runBenchmark();

template <typename T>
void ArrayBenchmark<T>::runBenchmark()
{
	this->resetCounters();
	auto collection = std::make_unique<std::vector<std::unique_ptr<T>>>();
	collection->resize(this->collectionSize + 1);
	for (int i = 0; i < this->collectionSize+1; i++)
	{
		collection->at(i) = std::make_unique<T>();
	}


	const auto end = std::chrono::high_resolution_clock::now() + std::chrono::seconds(this->runtime);
	bool run = true;
	while (run)
	{
		for (int i = 0; i < collection->size();)
		{
			if (std::chrono::high_resolution_clock::now() > end)
			{
				run = false;
				break;
			}

			for (int k = 0; k < this->getPadding1() && i < collection->size(); k += 2)
			{
				// read
				char data = collection->at(i)->data[0];
				data++;
				i++;
				this->readWriteOperations++;
				if (i >= collection->size())
				{
					break;
				}

				// write
				collection->at(i)->data[0] = 0;
				i++;
				this->readWriteOperations++;
			}



			if (this->insertPercentage > 0 && i < collection->size())
			{
				// insert
				// for some reason using emplace will cause a stack overflow when T is Element8MB
				// even the use of collection->emplace(collection-end()) will cause a stack overflow while the the following line will not
				collection->emplace_back(std::make_unique<T>());
				this->insertDeleteOperations++;
				i++;
			}

			for (int k = 0; k < this->getPadding2() && i < collection->size(); k += 2)
			{
				// read
				char data = collection->at(i)->data[0];
				data++;
				i++;
				this->readWriteOperations++;
				if (i >= collection->size())
				{
					break;
				}
				// write
				collection->at(i)->data[0] = 0;
				i++;
				this->readWriteOperations++;
			}



			if (this->insertPercentage > 0 && i < collection->size())
			{
				// delete
				collection->erase(collection->begin() + i);
				i++;
				this->insertDeleteOperations++;
			}

		}
	}
	this->printResults();
}