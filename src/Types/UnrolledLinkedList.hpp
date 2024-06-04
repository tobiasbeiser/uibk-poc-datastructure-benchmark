#pragma once
#include <vector>
#include <memory>
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
        : chunkSize(chunkSize) {
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
        }
        else {
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
