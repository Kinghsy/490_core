//
// Created by tripack on 16-12-16.
//
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <set>
#include <regex>
#include <boost/tokenizer.hpp>

#include "blif_replace.h"
#include "../circuit/interface.h"

using std::string;
using std::endl;
using std::cerr;
using std::cout;

const string statementModel("model");
const string statementInputs("inputs");
const string statementOutputs("outputs");
const string statementNames("names");
const string statementEnd("end");
const std::regex comment("^#.*");

struct BlifMetaData {
    std::set<string> inputs;
    std::set<string> outputs;
};

std::vector<string> tokenizeLine(std::string& line);
void removeCommon(std::ifstream& ifile,
                  std::ofstream& ofile,
                  const std::set<string>& nodeList);
void dumpNodes(std::ifstream &fromFile,
               std::ofstream &toFile);


void replacePartialBlif(const string& source,
                        const string& replaced,
                        const string& withFile,
                        const string& outFile) {
    // Open the required files
    std::ifstream fsource(source);
    if (!fsource) assert(0);
    std::ofstream fofile(outFile);
    if (!fofile) assert(0);

    BlifBooleanNet replacedNet(replaced);
    BlifBooleanNet withNet(withFile);

    // Verify the replacement is actually valid
    std::vector<string> replacedIn = replacedNet.inputNodeList();
    std::vector<string> replacedOut = replacedNet.outputNodeList();
    std::vector<string> withIn = withNet.inputNodeList();
    std::vector<string> withOut = withNet.outputNodeList();
    if (replacedIn.size() != withIn.size()) assert(0);
    if (!std::is_permutation(
            replacedIn.begin(),
            replacedIn.end(),
            withIn.begin()))
        assert(0); // Replaced and with file must has same inputs

    std::set<string> removedNode = replacedNet.totalNodeSet();
    for(const auto& node : replacedIn)
        removedNode.erase(node);
    // The input nodes must not be removed
    // Output nodes can be removed though
    removeCommon(fsource, fofile, removedNode);



    // Merge the new version:
    std::ifstream fwith(withFile);
    if (!fwith) assert(0);
    dumpNodes(fwith, fofile);

    // The terminating ".end"
    fofile << ".end" << endl;
}

void dumpNodes(std::ifstream &fromFile,
               std::ofstream &toFile) {
    string line;
    bool preserveCurrentline = false;
    while (fromFile) {
        std::getline(fromFile, line);
        if (!fromFile) return;
        if (line != "") {
            std::vector<string> tokenList = tokenizeLine(line);
            if (tokenList[0] == statementNames) {
                preserveCurrentline = true;
            } else if (tokenList[0] == statementEnd) {
                // DO NOT COPY THE ".end" line
                return;
            }
        }
        if (preserveCurrentline)
            toFile << line << endl;
    }
}

void removeCommon(std::ifstream &ifile,
                  std::ofstream &ofile,
                  const std::set<string>& nodeList) {
    string line;
    bool preserveCurrentline = true;
    while (ifile) {
        std::getline(ifile, line);
        if (!ifile) return;
        if (line != "") {
            std::vector<string> tokenList = tokenizeLine(line);
            if (tokenList[0] == statementNames) {
                string& last = tokenList[tokenList.size() - 1];
                if (nodeList.count(last)) { // if outnode is deleted
                    preserveCurrentline = false;
                } else {
                    preserveCurrentline = true;
                }
            } else if (tokenList[0] == statementEnd) {
                // DO NOT COPY THE ".end" line
                return;
            }
        }
        if (preserveCurrentline)
            ofile << line << endl;
    }
}

std::vector<string> tokenizeLine(std::string &line) {
    boost::tokenizer<> tok(line);
    return std::vector<string>(tok.begin(), tok.end());
}
