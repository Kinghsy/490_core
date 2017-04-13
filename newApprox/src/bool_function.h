//
// Created by king on 17-4-11.
//

#ifndef VE490_NEW_BOOLEAN_FUNCTION_H
#define VE490_NEW_BOOLEAN_FUNCTION_H

#include <cstdlib>
#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <map>

#include "const.h"
#include "kmap.h"
#include "ttable.h"

class BooleanFunction {
private:
    int inputSize;
    TTable truthTab;
    std::vector<NodeName> portName;
    NodeName outPortName;
public:
    BooleanFunction(): truthTab(std::string("00")) {};
    BooleanFunction(const int &inputSize,
                    TTable& truthTab,
                    const std::vector<NodeName >& portName,
                    const NodeName& outPortName);
    ~BooleanFunction();

    void operator= (const BooleanFunction &initBF);
    bool operator== (const BooleanFunction &initBF);
    int operator^(const BooleanFunction &initBF);

    friend BooleanFunction combineBooleanFunction(
            const BooleanFunction& bf1,
            const BooleanFunction& bf2,
            const TTable& method,
            const NodeName& outName);

    int getInputSize() const {return inputSize;}
    int getVal(const DBitset& term) const {return truthTab[term];}
    int getVal(const size_t term) const {return truthTab[term];}
    bool isAll0s() const;
    bool isAll1s() const;

    TTable& getTTable() {return truthTab;}

    //friend TTable findMethod(const BooleanFunction& bf);

    int getProtNum(const NodeName& name) const;
    NodeName getPortName(const int& i) const;
    NodeName getOutPortName() const;
};

BooleanFunction combineBooleanFunction(
        const BooleanFunction& bf1,
        const BooleanFunction& bf2,
        const TTable& method,
        const NodeName& outName
);

#endif //VE490_BOOLEAN_FUNCTION_H
