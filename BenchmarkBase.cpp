#include "BenchmarkBase.h"

int BenchmarkBase::getPadding()
{
    int padding = 1;
    if (this->insertPercentage > 0)
    {
        double ratio = this->readPercentage / this->insertPercentage;
        ratio = ratio * 100;

        while ((int)ratio % 10 == 0)
        {
            ratio = ratio / 10;
        }
        padding = ratio;
    }
    return padding;
}
void BenchmarkBase::printResults()
{
    std::cout << "Total operations/s:" << (this->insertDeleteOperations + this->readWriteOperations)/this->runtime << std::endl;
}

void BenchmarkBase::resetCounters()
{
    this->readWriteOperations=0;
    this->insertDeleteOperations=0;
}