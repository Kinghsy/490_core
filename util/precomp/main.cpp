//
// Created by tripack on 17-3-7.
//

/*
 * Precomputation Program
 * Pre-computes Decomposition results for 2-5 input patterns
 * Also used for observing the techs.
 */

#include <cinttypes>
#include <iostream>
#include <initializer_list>
#include <array>
#include <numeric>
#include <bitset>

#include "common.h"
#include "do_lut.h"

uint8_t lut3[256] = {0};
uint8_t lut3_2[256] = {0};

using std::cout;
using std::endl;
using std::bitset;

int main() {
    cout << "bitset of 0101 1100B:" << endl;
    cout << bitset<8>("01011100") << endl;
    cout << bitset<8>(0x5Cul) << endl;
    auto v = bitset<8>(0x5Cul);
    cout << v[0] << v[1] << v[2] << v[3] << endl;

    cout << fillWith<32>(4, std::bitset<32>("0010")) << endl;

    cout << upCast<4, 32>(std::bitset<4>("0101")) << endl;

    cout << ones<16>(3) << endl;
    cout << ones<16>() << endl;
    cout << ones<16>(0) << endl;
    cout << ones<64>(0) << endl;
    cout << ones<64>() << endl;

    cout << expansionMask<5>(4ul, 2ul) << endl;

    cout << "Select:" << endl;
    auto s = selectWithin<3>(2);
    for (auto e : s) cout << e << endl;
    cout << s.size() << endl;

    cout << expand<5, 3>(FUN(3)("00101101"), std::bitset<5>("10110")) << endl;

    cout << transfer<10, 4>(std::bitset<4>("1001"), std::bitset<10>("0100101100")) << endl;

    for (const auto& e : lut1Rst) {
        cout << "=== === === ===" << endl;
        cout << "Function: " << e.first << endl;
        cout << "LeftFun: " << e.second.leftFun << endl;
        cout << "RightFun: " << e.second.rightFun << endl;
        cout << "CombineFun: " << e.second.combine << endl;
        cout << "MaskLeft: " << e.second.maskLeft << endl;
        cout << "MaskRight: " << e.second.maskRight << endl;
        cout << "MaskDiscard:" << e.second.maskDiscard << endl;
    }

    cout << endl << "Count: " << lut2Rst.size() << endl;
    for (const auto& e : lut2Rst) {
        cout << "=== === === ===" << endl;
        cout << "Function: " << e.first << endl;
        cout << "LeftFun: " << e.second.leftFun << endl;
        cout << "RightFun: " << e.second.rightFun << endl;
        cout << "CombineFun: " << e.second.combine << endl;
        cout << "MaskLeft: " << e.second.maskLeft << endl;
        cout << "MaskRight: " << e.second.maskRight << endl;
        cout << "MaskDiscard:" << e.second.maskDiscard << endl;
    }

    PRECOM_RESULT(3) lut3 = do3input();

    std::cout << lut3.size() << endl;

    PRECOM_RESULT(4) lut4 = do4input(lut3);

    std::cout << lut4.size() << endl;

    PRECOM_RESULT(5) lut5 = do5input(lut3, lut4);

    std::cout << lut5.size() << endl;
    return 0;
}


