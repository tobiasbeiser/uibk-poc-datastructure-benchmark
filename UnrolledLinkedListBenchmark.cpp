#include "UnrolledLinkedListBenchmark.hpp"
#include <vector>
#include <memory>
#include <iostream>

constexpr size_t CHUNK_SIZE = 16;

template <typename T>
struct UnrolledLinkedListNode {
    std::vector<T> elements;
    std::shared_ptr<UnrolledLinkedListNode<T>> next;

    UnrolledLinkedListNode() : elements(), next(nullptr) {
        elements.reserve(CHUNK_SIZE);
    }
};

template <typename T>
class UnrolledLinkedList {
public:
    explicit UnrolledLinkedList(size_t initialSize) {
        size_t numChunks = (initialSize + CHUNK_SIZE - 1) / CHUNK_SIZE;
        head = std::make_shared<UnrolledLinkedListNode<T>>();
        auto current = head;
        for (size_t i = 0; i < numChunks; ++i) {
            for (int j = 0; j < CHUNK_SIZE; ++j) {
                current->elements.emplace_back(T());
            }
            if (i < numChunks - 1) {
                current->next = std::make_shared<UnrolledLinkedListNode<T>>();
                current = current->next;
            }
        }
        tail = current;
    }

    void read(std::shared_ptr<UnrolledLinkedListNode<T>>& node, size_t& chunkIndex) {
        if (chunkIndex < node->elements.size()) {
            char data = node->elements[chunkIndex].data[0];
            data++;
        }
    }

    void write(std::shared_ptr<UnrolledLinkedListNode<T>>& node, size_t& chunkIndex) {
        if (chunkIndex < node->elements.size()) {
            node->elements[chunkIndex].data[0] = 0;
        }
    }

    void insert(std::shared_ptr<UnrolledLinkedListNode<T>>& node, size_t& chunkIndex, const T& newElement) {
        if (!head) {
            head = std::make_shared<UnrolledLinkedListNode<T>>();
            head->elements.push_back(newElement);
            chunkIndex = 0;
            tail = head;
        } else if (tail->elements.size() + 1 < CHUNK_SIZE) {
            tail->elements.push_back(newElement);
        } else {
            auto newNode = std::make_shared<UnrolledLinkedListNode<T>>();
            size_t halfSize = tail->elements.size() / 2;
            newNode->elements.insert(newNode->elements.end(), std::make_move_iterator(tail->elements.begin() + halfSize), std::make_move_iterator(tail->elements.end()));
            tail->elements.erase(tail->elements.begin() + halfSize, tail->elements.end());
            newNode->elements.push_back(newElement);
            tail->next = newNode;
            tail = newNode;
        }
    }

    void remove(std::shared_ptr<UnrolledLinkedListNode<T>>& node, size_t& chunkIndex) {
        if (!node->elements.empty()) {
            node->elements.pop_back();
        }
        if (chunkIndex >= node->elements.size()) {
            node = node->next;
            chunkIndex = 0;
        }
    }

    std::shared_ptr<UnrolledLinkedListNode<T>> head;
    std::shared_ptr<UnrolledLinkedListNode<T>> tail;
};

template void UnrolledLinkedListBenchmark<Element8Bytes>::runBenchmark();
template void UnrolledLinkedListBenchmark<Element512Bytes>::runBenchmark();
template void UnrolledLinkedListBenchmark<Element8MB>::runBenchmark();

template <typename T>
void UnrolledLinkedListBenchmark<T>::runBenchmark()
{
    // Initialize the unrolled linked list with the specified collection size.
    UnrolledLinkedList<T> collection(this->collectionSize);

    // Reset counters before every benchmark to ensure correct stats.
    this->resetCounters();

    const auto end = std::chrono::high_resolution_clock::now() + std::chrono::seconds(this->runtime);

    bool run = true;

    while (run)
    {
        auto node = collection.head;
        size_t chunkIndex = 0;

        while (node && run)
        {
            // Stop if time is over
            if (std::chrono::high_resolution_clock::now() > end)
            {
                run = false;
                break;
            }

            for (int i = 0; i < this->getPadding1() && node; i += 2)
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

            if (this->insertPercentage > 0 && node)
            {
                collection.insert(node, chunkIndex, T());
                this->insertDeleteOperations++;

                // Reset chunkIndex to 0 as we might have moved to a new chunk
                chunkIndex = 0;
            }

            for (int i = 0; i < this->getPadding2() && node; i += 2)
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

            if (this->insertPercentage > 0 && node && !node->elements.empty())
            {
                collection.remove(node, chunkIndex);
                this->insertDeleteOperations++;

                // Reset chunkIndex to 0 as we might have moved to a new chunk
                chunkIndex = 0;
            }

            // Ensure chunkIndex is updated correctly
            if (chunkIndex >= node->elements.size() && node->next)
            {
                node = node->next;
                chunkIndex = 0;
            }
        }
    }

    // Always print results at the end to be read by the python script.
    this->printResults();
}
