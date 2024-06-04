#include "UnrolledLinkedListRandomBenchmark.hpp"
#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

template void UnrolledLinkedListRandomBenchmark<Element8Bytes>::runBenchmark();
template void UnrolledLinkedListRandomBenchmark<Element512Bytes>::runBenchmark();
template void UnrolledLinkedListRandomBenchmark<Element8MB>::runBenchmark();

template <typename T>
void UnrolledLinkedListRandomBenchmark<T>::runBenchmark()
{
    // Initialize the unrolled linked list with the specified collection size.
    UnrolledLinkedList<T> collection(this->collectionSize, this->chunkSize);

    // Reset counters before every benchmark to ensure correct stats.
    this->resetCounters();

    // to simulate random access, we store the nodes in a vector and shuffle them
    // we then iterate over the vector and get a different starting node for each iteration
    // this way the overhead for generating the random access pattern is not included in the benchmark
    std::vector<std::shared_ptr<UnrolledLinkedListNode<T>>> nodes;
    nodes.reserve(this->collectionSize);

    auto node = collection.head;
    while (node != nullptr)
    {
        nodes.push_back(node);
        node = node->next;
    }

    auto rng = std::default_random_engine{};
    std::shuffle(nodes.begin(), nodes.end(), rng);

    const auto end = std::chrono::high_resolution_clock::now() + std::chrono::seconds(this->runtime);

    bool run = true;
    while (run)
    {
        for (int i = 0; i < nodes.size();)
        {
            auto node = nodes[i];
            size_t chunkIndex = 0;

            if (std::chrono::high_resolution_clock::now() > end)
            {
                run = false;
                break;
            }

            for (int k = 0; k < this->getPadding1() && node != nullptr; k += 2)
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

                if (tmp != node || ++chunkIndex >= node->elements.size())
                {
                    chunkIndex = 0;
                }
            }

            for (int k = 0; k < this->getPadding2() && node != nullptr; k += 2)
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

            if (this->insertPercentage > 0 && node != nullptr && !node->elements.empty())
            {
                collection.remove(node, chunkIndex);
                this->insertDeleteOperations++;

                // Reset chunkIndex to 0 as we might have moved to a new chunk
                chunkIndex = 0;
            }

            // Ensure chunkIndex is updated correctly
            if (node != nullptr && chunkIndex >= node->elements.size())
            {
                node = node->next;
                chunkIndex = 0;
            }

            ++i;
        }
    }

    // Always print results at the end to be read by the python script.
    this->printResults();
}
