#include "ListBenchmark.h"
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
	auto end = std::chrono::high_resolution_clock::now() + std::chrono::seconds(this->runtime);
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
			if (this->insertPercentage > 0)
			{
				T newElement = T();
				collection.push_front(newElement);
				this->insertDeleteOperations++;
				it++;
			}

			for (int k = 0; k < this->padding; k++)
			{
				// read
				char data = (*it).data[0];
				data++;
				it++;
				// write
				(*it).data[0] = 0;
				this->readWriteOperations++;
				it++;
			}

			if (this->insertPercentage > 0)
			{
				// delete
				collection.pop_front();
				this->insertDeleteOperations++;
				it++;
			}
		}
	}
	this->printResults();
}