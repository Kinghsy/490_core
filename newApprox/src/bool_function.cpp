//
// Created by king on 17-4-13.
//

#include <cstdlib>
#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <map>

#include "ttable.h"
#include "const.h"
#include "kmap.h"


using std::string;
using std::vector;

BooleanFunction::BooleanFunction
        (size_t inSize, const TTable& tTab,
         const vector<NodeName >& pName,
         const NodeName& oPName): truthTab(tTab)
{
    assert(inSize == tTab.nInputs());
    inputSize =inSize;
    truthTab = tTab;
    portName = pName;
    outPortName = oPName;
}



void BooleanFunction::operator=(const BooleanFunction &initBF) {
    this->inputSize = initBF.inputSize;
    this->truthTab = initBF.truthTab;
    this->portName = initBF.portName;
    this->outPortName = initBF.outPortName;
}

bool BooleanFunction::operator==(const BooleanFunction &initBF) {
    if (this->inputSize != initBF.inputSize)
        return false;

    if (this->outPortName != initBF.outPortName)
        return false;

    for (int i = 0; i < this->inputSize; ++i) {
        if (this->getPortNum( initBF.getPortName(i) ) == -1)
            return false;
    }

    for (int i = 0; i < (1 << (this->inputSize)); ++i) {
        size_t base = 0;
        size_t temp = i;
        for (int k = 0; k < inputSize; ++k) {
            base += (temp % 2) * ( 1 << (this->getPortNum( initBF.getPortName(k))));
            temp = temp / 2;
        }
        if (this->truthTab[base] != initBF.truthTab[i])
            return false;
    }

    return true;
}

int BooleanFunction::operator^(const BooleanFunction &initBF) {
    if (this->inputSize != initBF.inputSize)
        //assert(0);
        return -1;

    if (this->outPortName != initBF.outPortName)
        //assert(0);
        return -1;

    for (size_t i = 0; i < this->inputSize; ++i) {
        if (this->getPortNum( initBF.getPortName(i) ) == -1)
            //assert(0);
            return -1;
    }

    int count = 0;
    for (int i = 0; i < (1 << (this->inputSize)); ++i) {
        size_t base = 0;
        size_t temp = i;
        for (int k = 0; k < inputSize; ++k) {
            base += (temp % 2) * ( 1 << (this->getPortNum( initBF.getPortName(k))));
            temp = temp / 2;
        }
        if (this->truthTab[base] != initBF.truthTab[i])
            count ++;
    }

    return count;
}

BooleanFunction combineBooleanFunction(
        const BooleanFunction& bf1,
        const BooleanFunction& bf2,
        const TTable& method,
        const NodeName& outName )
{
    // build inputs' set
    vector<NodeName> nodeSet = bf1.portName;
    for (size_t i = 0; i < bf2.inputSize; ++i)
        nodeSet.push_back(bf2.getPortName(i));

    // inputSize;
    size_t newInputSize = bf1.inputSize + bf2.inputSize;

    // outPortName;
    NodeName outPortName = outName;

    // TTable;
    DBitset leftMask(newInputSize, 0);
    DBitset rightMask(newInputSize, 0);
    for (int i = 0; i < bf1.inputSize; ++i) {
        leftMask[i] = 1;
    }
    for (size_t i = bf1.inputSize; i < newInputSize; ++i) {
        rightMask[i] = 1;
    }
    TTable resTTable = combineTruthTable(
            bf1.truthTab, bf2.truthTab, leftMask, rightMask, method);

    return BooleanFunction(
            newInputSize, resTTable, nodeSet, outPortName
    );

}

int BooleanFunction::getPortNum(const NodeName &name) const  {
    for (int i = 0; i < inputSize; ++i) {
        if (portName[i] == name) return i;
    }
    return -1;
}

bool BooleanFunction::isAll0s() const {
    for (size_t i = 0; i < (1 << inputSize); ++i) {
        if (truthTab[i] == 1) return false;
    }
    return true;
}

bool BooleanFunction::isAll1s() const {
    for (size_t i = 0; i < (1 << inputSize); ++i) {
        if (truthTab[i] == 0) return false;
    }
    return true;
}