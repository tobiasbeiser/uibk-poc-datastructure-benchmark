#pragma once
#include <vector>
#include <any>
#include <cmath>
template<typename T>
class TieredArray {
public:
    TieredArray(size_t n, size_t chunkSize)
        : totalElements(n), chunkSize(chunkSize) {
        //        Initialize Layers
        size_t size = n + 1;
        size_t numChunks = ceil(size / (double)chunkSize);
        tiers = std::vector<std::vector<T>>(numChunks);
        for (int i = 0; i < numChunks; i++) {
            tiers[i] = std::vector<T>(chunkSize);
        }
    }
    virtual ~TieredArray() {}

    void insert(size_t i) {
        T* value = new T();
        size_t chunkIndex = i / chunkSize;
        size_t position = i % chunkSize;
        T* previous = value;
        T* current = &(tiers[chunkIndex][position]);
        //        chunk with element
        for (size_t k = position; k < tiers[chunkIndex].size(); k++) {
            current = &(tiers[chunkIndex][k]);
            tiers[chunkIndex][k] = *previous;
            previous = current;
        }
        for (size_t j = chunkIndex + 1; j < tiers.size(); j++) {
            for (int k = 0; k < tiers[j].size(); k++) {
                current = &(tiers[j][k]);
                tiers[j][k] = *previous;
                previous = current;
            }
        }
        delete value;
        totalElements++;
    }

    void del(size_t i) {
        size_t chunkIndex = i / chunkSize;
        size_t position = i % chunkSize;
        T* current = &(tiers.back().back());
        //        take some trash value to set for the last value (which is officially no part of the array anymore)
        T* next = &(tiers.back().back());
        for (size_t j = tiers.size() - 1; j > chunkIndex; j--) {
            for (int k = tiers[j].size() - 1; k >= 0; k--) {
                current = &(tiers[j][k]);
                tiers[j][k] = *next;
                next = current;
            }
        }
        //        chunk with element
        for (int k = tiers[chunkIndex].size() - 1; k >= position && k>=0; k--) {
            current = &(tiers[chunkIndex][k]);
            tiers[chunkIndex][k] = *next;
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