#include "Benchmark.hpp"
int Benchmark::getPadding()
{
    double rwFraction = this->readPercentage / 100.0;
    double idFraction = this->insertPercentage / 100.0;

    int totalReads = this->collectionSize * rwFraction;
    int totalInserts = this->collectionSize * idFraction;

    int padding;

    if (totalInserts == 0)
    {
        padding = totalReads;
    }
    else if (totalInserts == 1)
    {
        padding = totalReads / (totalInserts + 1);
    }
    else
    {
        padding = totalReads / totalInserts;
    }
    return padding;
}
void Benchmark::printResults(const double duration )
{
    int total = this->insertDeleteOperations + this->readWriteOperations;
    std::cout << this->dataStructureName << ";" << (total) / duration << std::endl;
    // std::cout << "Padding: " << this->padding << std::endl;
    // std::cout << "Padding 1:" << this->getPadding1() << std::endl;
    // std::cout << "Padding 2: " << this->getPadding2() << std::endl;
    // std::cout << "Reads: " << this->readWriteOperations << ";Inserts: " << this->insertDeleteOperations << "; Ratio: " << 100.0/total*readWriteOperations << "/" << 100.0/total*insertDeleteOperations  <<std::endl;
}

void Benchmark::resetCounters()
{
    this->readWriteOperations = 0;
    this->insertDeleteOperations = 0;
}

int Benchmark::getPadding1()
{
    if (this->padding % 2 != 0)
    {
        return padding - 1;
    }
    else return padding;

}

int Benchmark::getPadding2()
{
    if (this->padding % 2 != 0)
    {
        return padding + 1;
    }
    else return padding;
}