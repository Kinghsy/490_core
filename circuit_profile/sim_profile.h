//
// Created by tripack on 17-4-2.
//

#ifndef VE490_SIM_PROFILE_H
#define VE490_SIM_PROFILE_H

#include <common.h>
#include <vector>
#include <string>
#include <bitset>
#include <ostream>

class BlifBooleanNet;

struct FocusedSimulationResult;

struct SimulationResult {

    size_t nSamples;

    std::vector<std::vector<int> > inputResult;
    std::vector<std::vector<int> > outputResult;
    std::vector<std::vector<int> > internalResult;

    std::vector<std::string> inputName;
    std::vector<std::string> outputName;
    std::vector<std::string> internalName;

    SimulationResult(const BlifBooleanNet& net,
                     size_t nSamples);

    FocusedSimulationResult focus(const std::vector<std::string>& node) const;
};

class FocusedSimulationResult {

    std::vector<size_t> data;
    size_t nSamples;

public:
    const size_t& cnSamples = nSamples;
    const std::vector<size_t>& cdata;
    const std::vector<std::string> nodeOrder;

    /* Notice this function is ordered!
     * The order of the query must be maintained
     * in the subsequent operations (count etc.)
     */
    FocusedSimulationResult(const SimulationResult& result,
                            const std::vector<std::string>& node);

    size_t count(size_t term) const {return data.at(term);}
    size_t count(const DBitset& term) const;
    size_t count(const std::vector<DBitset> &termSet) const;

    friend std::ostream &operator<<(std::ostream &os,
                                    const FocusedSimulationResult &result);

};

struct CompareResult {

    size_t nSamples;
    size_t nErrors;

};

size_t countMatchError(const TTable &orignal, const TTable &next,
                       const FocusedSimulationResult &sim);

void printMatchError(const TTable &orignal, const TTable &next,
                     const FocusedSimulationResult &sim);

#endif //VE490_SIM_PROFILE_H
