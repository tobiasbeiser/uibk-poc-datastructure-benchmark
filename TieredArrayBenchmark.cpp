#include "TieredArrayBenchmark.hpp"

template<typename T>
class TieredArray {
public:
    explicit TieredArray(size_t n, size_t chunkSize)
        : totalElements(n), chunkSize(chunkSize) {
//        Initialize Layers
        size_t size = n + 1;
        size_t numChunks = size / chunkSize;
        tiers = std::vector<std::vector<T>>(numChunks);
        for (int i = 0; i < numChunks - 1; i++) {
            tiers[i] = std::vector<T>(chunkSize);
        }
        tiers[numChunks - 1] = std::vector<T>(size % chunkSize);
    }

    void insert(size_t i, T& value) {
        size_t chunkIndex = i / chunkSize;
        size_t position = i % chunkSize;
        T previous = value;
        T current = tiers[chunkIndex][position];
//        chunk with element
        for (size_t k = position; k < chunkSize; k++) {
            current = tiers[chunkIndex][k];
            tiers[chunkIndex][k] = previous;
            previous = current;
        }
        for (size_t j = chunkIndex + 1; j < tiers.size(); j++) {
            for (int k = 0; k < tiers[j].size(); k++) {
                current = tiers[j][k];
                tiers[j][k] = previous;
                previous = current;
            }
        }
        totalElements++;
    }

    void del(size_t i) {
        size_t chunkIndex = i / chunkSize;
        size_t position = i % chunkSize;
        T current = tiers[0][0];
//        take some trash value to set for the last value (which is officially no part of the array anymore)
        T next = tiers[0][0];
        for (size_t j = tiers.size() - 1; j > chunkIndex; j--) {
            for (int k = tiers[j].size() - 1; k >= 0; k--) {
                current = tiers[j][k];
                tiers[j][k] = next;
                next = current;
            }
        }
//        chunk with element
        for (int k = tiers[chunkIndex].size() - 1; k >= position; k--) {
            current = tiers[chunkIndex][k];
            tiers[chunkIndex][k] = next;
            next = current;
        }
        totalElements--;
    }

    T& at(size_t i) {
        return tiers[i / chunkSize][i % chunkSize];
    }

    size_t size() const {
        return totalElements;
    }

private:
    size_t chunkSize;
    size_t totalElements = 0;
    std::vector<std::vector<T>> tiers;

};



template void TieredArrayBenchmark<Element8Bytes>::runBenchmark();
template void TieredArrayBenchmark<Element512Bytes>::runBenchmark();
template void TieredArrayBenchmark<Element8MB>::runBenchmark();

template <typename T>
void TieredArrayBenchmark<T>::runBenchmark()
{
    
	this->resetCounters();
//    TODO: chunkSize in TieredArrayBenchmark
	TieredArray<T> collection(this->collectionSize, 4);
    
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
			for (int k = 0; k < this->getPadding1() && i < collection.size(); k += 2)
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
				// insert
                // TODO: create an element for insert
				// collection.insert(i, element);
				this->insertDeleteOperations++;
				i++;
			}
			for (int k = 0; k < this->getPadding2() && i < collection.size(); k += 2)
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



			if (this->insertPercentage > 0 && i < this->collectionSize)
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