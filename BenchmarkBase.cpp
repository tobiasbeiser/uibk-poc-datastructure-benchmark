#include "BenchmarkBase.hpp"

int BenchmarkBase::getPadding()
{
    double rwFraction = this->readPercentage / 100.0;
    double idFraction = this->insertPercentage / 100.0;

    int totalReads = this->collectionSize*rwFraction;
    int totalInserts = this->collectionSize*idFraction;

    int padding;

    if (totalInserts==0)
    {
        padding=totalReads;
    }else if(totalInserts==1)
    {
        padding = totalReads/(totalInserts+1);
    }else
    {
        padding = totalReads/totalInserts;
    }
    return padding;
}
void BenchmarkBase::printResults()
{
    int total = this->insertDeleteOperations + this->readWriteOperations;
    std::cout << this->dataStructureName << ";" << (total)/this->runtime << std::endl;
    // std::cout << "Padding: " << this->padding << std::endl;
    // std::cout << "Padding 1:" << this->getPadding1() << std::endl;
    // std::cout << "Padding 2: " << this->getPadding2() << std::endl;
    // std::cout << "Reads: " << this->readWriteOperations << ";Inserts: " << this->insertDeleteOperations << "; Ratio: " << 100.0/total*readWriteOperations << "/" << 100.0/total*insertDeleteOperations  <<std::endl;
}

void BenchmarkBase::resetCounters()
{
    this->readWriteOperations=0;
    this->insertDeleteOperations=0;
}

int BenchmarkBase::getPadding1()
{
    if (this->padding%2!=0)
    {
        return padding-1;
    }
    else return padding;
    
}

int BenchmarkBase::getPadding2()
{
    if (this->padding%2!=0)
    {
        return padding+1;
    }
    else return padding;
}