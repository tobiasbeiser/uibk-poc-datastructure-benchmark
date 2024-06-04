#include "ArrayRandomAccessBenchmark.hpp"
#include <algorithm>
#include <random>

template void ArrayRandomAccessBenchmark<Element8Bytes>::runBenchmark();
template void ArrayRandomAccessBenchmark<Element512Bytes>::runBenchmark();
template void ArrayRandomAccessBenchmark<Element8MB>::runBenchmark();

template <typename T>
void ArrayRandomAccessBenchmark<T>::runBenchmark()
{
	this->resetCounters();
	auto collection = std::make_unique<std::vector<std::unique_ptr<T>>>();
	collection->resize(this->collectionSize + 1);
	for (int i = 0; i < this->collectionSize + 1; i++)
	{
		collection->at(i) = std::make_unique<T>();
	}
	std::vector<int> randomIndices(collectionSize + 1);
	for (int i = 0; i < collectionSize + 1; i++)
	{
		randomIndices.at(i) = i;
	}
	auto rng = std::default_random_engine{};
	std::shuffle(randomIndices.begin(), randomIndices.end(), rng);


	const auto end = std::chrono::high_resolution_clock::now() + std::chrono::seconds(this->runtime);
	bool run = true;
	while (run)
	{
		for (auto it = randomIndices.begin(); it != randomIndices.end();)
		{
			if (std::chrono::high_resolution_clock::now() > end)
			{
				run = false;
				break;
			}

			for (int k = 0; k < this->getPadding1() && it != randomIndices.end(); k += 2)
			{
				// read
				char data = collection->at(*it)->data[0];
				data++;
				++it;
				this->readWriteOperations++;
				if (it == randomIndices.end())
				{
					break;
				}

				// write
				collection->at(*it)->data[0] = 0;
				++it;
				this->readWriteOperations++;
			}



			if (this->insertPercentage > 0 && it != randomIndices.end())
			{
				// insert
				collection->emplace(collection->begin() + *it, std::make_unique<T>());
				this->insertDeleteOperations++;
				++it;
			}

			for (int k = 0; k < this->getPadding2() && it != randomIndices.end(); k += 2)
			{
				// read
				char data = collection->at(*it)->data[0];
				data++;
				++it;
				this->readWriteOperations++;
				if (it == randomIndices.end())
				{
					break;
				}
				// write
				collection->at(*it)->data[0] = 0;
				++it;
				this->readWriteOperations++;
			}



			if (this->insertPercentage > 0 && it != randomIndices.end())
			{
				// delete
				collection->erase(collection->begin() + *it);
				++it;
				this->insertDeleteOperations++;
			}

		}
	}
	this->printResults();
}