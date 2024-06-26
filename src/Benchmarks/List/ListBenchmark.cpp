#include "ListBenchmark.hpp"
#include <iostream>
#include <forward_list>

template void ListBenchmark<Element8Bytes>::runBenchmark();
template void ListBenchmark<Element512Bytes>::runBenchmark();
template void ListBenchmark<Element8MB>::runBenchmark();

template <typename T>
void ListBenchmark<T>::runBenchmark()
{
	this->resetCounters();
	std::forward_list<T> collection(this->collectionSize);

	const auto end = std::chrono::high_resolution_clock::now() + std::chrono::seconds(this->runtime);
	
	bool run = true;
	while (run)
	{
		for (typename std::forward_list<T>::iterator it = collection.begin(); it != collection.end();)
		{
			if (std::chrono::high_resolution_clock::now() > end)
			{
				run = false;
				break;
			}

			for (int k = 0; k < this->getPadding1() && it != collection.end(); k += 2)
			{
				// read
				char data = (*it).data[0];
				this->readWriteOperations++;
				data++;
				it++;
				if (it == collection.end())
				{
					break;
				}

				// write
				(*it).data[0] = 0;
				this->readWriteOperations++;
				it++;
			}


			if (this->insertPercentage > 0)
			{
				// insert
				collection.emplace_front();
				this->insertDeleteOperations++;
				if (it != collection.end())
				{
					it++;
				}
			}

			for (int k = 0; k < this->getPadding2() && it != collection.end(); k += 2)
			{
				// read
				char data = (*it).data[0];
				this->readWriteOperations++;
				data++;
				it++;

				if (it == collection.end())
				{
					break;
				}

				// write
				(*it).data[0] = 0;
				this->readWriteOperations++;
				it++;
			}

			if (this->insertPercentage > 0 && it != collection.end())
			{
				// delete
				collection.pop_front();
				this->insertDeleteOperations++;
				if (it != collection.end())
				{
					it++;
				}
			}
		}
	}
	this->printResults();
}