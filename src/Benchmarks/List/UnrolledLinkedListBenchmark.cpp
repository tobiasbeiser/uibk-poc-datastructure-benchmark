#include "UnrolledLinkedListBenchmark.hpp"
#include <iostream>


template void UnrolledLinkedListBenchmark<Element8Bytes>::runBenchmark();
template void UnrolledLinkedListBenchmark<Element512Bytes>::runBenchmark();
template void UnrolledLinkedListBenchmark<Element8MB>::runBenchmark();

template <typename T>
void UnrolledLinkedListBenchmark<T>::runBenchmark()
{
    // Initialize the unrolled linked list with the specified collection size.
    UnrolledLinkedList<T> collection(this->collectionSize, this->chunkSize);

    // Reset counters before every benchmark to ensure correct stats.
    this->resetCounters();

    const auto end = std::chrono::high_resolution_clock::now() + std::chrono::seconds(this->runtime);

    bool run = true;
    auto node = collection.head;
    size_t chunkIndex = 0;
    std::shared_ptr<T> toInsert = std::make_shared<T>();
    while (run)
    {
        if (!node) {
            node = collection.head;
            chunkIndex = 0;
        }

        // Stop if time is over
        if (std::chrono::high_resolution_clock::now() > end)
        {
            run = false;
            break;
        }

        for (int i = 0; i < this->getPadding1() && node != nullptr; i += 2)
        {
            // Stop if time is over
            if (std::chrono::high_resolution_clock::now() > end)
            {
                run = false;
                break;
            }

            // Read
            collection.read(node, chunkIndex);
            this->readWriteOperations++;

            // Write
            collection.write(node, chunkIndex);
            this->readWriteOperations++;

            chunkIndex++;

            // If new index points to no element, go to the next chunk
            if (chunkIndex >= node->elements.size())
            {
                node = node->next;
                chunkIndex = 0;
            }
        }

        if (this->insertPercentage > 0 && node != nullptr)
        {
            std::shared_ptr<UnrolledLinkedListNode<T>> tmp = collection.insert(node, chunkIndex);
            this->insertDeleteOperations++;

            if(tmp != node || ++chunkIndex >= node->elements.size()){
                chunkIndex = 0;
            }
        }

        for (int i = 0; i < this->getPadding2() && node != nullptr; i += 2)
        {
            // Stop if time is over
            if (std::chrono::high_resolution_clock::now() > end)
            {
                run = false;
                break;
            }

            // Read
            collection.read(node, chunkIndex);
            this->readWriteOperations++;

            // Write
            collection.write(node, chunkIndex);
            this->readWriteOperations++;

            chunkIndex++;
            if (chunkIndex >= node->elements.size())
            {
                node = node->next;
                chunkIndex = 0;
            }
        }

        if (!node) {
            node = collection.head;
            chunkIndex = 0;
        }

        if (this->insertPercentage > 0 && node != nullptr && !node->elements.empty())
        {
            collection.remove(node, chunkIndex);
            this->insertDeleteOperations++;

            // Reset chunkIndex to 0 as we might have moved to a new chunk
            chunkIndex = 0;
        }

        // Ensure chunkIndex is updated correctly
        if (chunkIndex >= node->elements.size())
        {
            node = node->next;
            chunkIndex = 0;
        }
    }

    // Always print results at the end to be read by the python script.
    this->printResults();
}
