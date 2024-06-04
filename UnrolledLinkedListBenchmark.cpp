#include "UnrolledLinkedListBenchmark.hpp"
#include <vector>
#include <memory>
#include <iostream>

template <typename T>
struct UnrolledLinkedListNode {
    std::vector<std::shared_ptr<T>> elements;
    std::shared_ptr<UnrolledLinkedListNode<T>> next;

    explicit UnrolledLinkedListNode(size_t chunkSize) : elements(), next(nullptr) {
        elements.reserve(chunkSize);
    }
};

template <typename T>
class UnrolledLinkedList {
public:
    UnrolledLinkedList(size_t initialSize, size_t chunkSize)
    : chunkSize(chunkSize){
        size_t numChunks = (initialSize + chunkSize - 1) / chunkSize;
        head = std::make_shared<UnrolledLinkedListNode<T>>(chunkSize);
        auto current = head;
        for (size_t i = 0; i < numChunks - 1; ++i) {
            for (int j = 0; j < chunkSize; ++j) {
                current->elements.emplace_back(std::make_shared<T>());
            }
            current->next = std::make_shared<UnrolledLinkedListNode<T>>(chunkSize);
            current = current->next;
        }
        for (int i = 0; i < initialSize % chunkSize; ++i) {
            current->elements.emplace_back(std::make_shared<T>());
        }
        current->next = nullptr;
    }

    void read(std::shared_ptr<UnrolledLinkedListNode<T>>& node, size_t& chunkIndex) {
        if (chunkIndex < node->elements.size()) {
            char data = node->elements[chunkIndex]->data[0];
            data++;
        }
    }

    void write(std::shared_ptr<UnrolledLinkedListNode<T>>& node, size_t& chunkIndex) {
        if (chunkIndex < node->elements.size()) {
            node->elements[chunkIndex]->data[0] = 0;
        }
    }

    std::shared_ptr<UnrolledLinkedListNode<T>> insert(std::shared_ptr<UnrolledLinkedListNode<T>>& node, size_t& chunkIndex) {
        if (node->elements.size() < chunkSize) {
            node->elements.insert(node->elements.begin() + chunkIndex, std::make_shared<T>());
            chunkIndex++;
            return node;
        } else {
            auto newNode = std::make_shared<UnrolledLinkedListNode<T>>(chunkSize);
            size_t halfSize = node->elements.size() / 2;
            newNode->elements.insert(newNode->elements.end(),
                                     std::make_move_iterator(node->elements.begin() + halfSize),
                                     std::make_move_iterator(node->elements.end()));
            node->elements.erase(node->elements.begin() + halfSize, node->elements.end());
            newNode->elements.insert(newNode->elements.end(), std::make_shared<T>());
            newNode->next = node->next;
            node->next = newNode;
            chunkIndex = newNode->elements.size() - 1; // adjust chunkIndex to point to the newly inserted element
            return newNode;
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
private:
    size_t chunkSize;

};

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
