
#pragma once
#include <vector>
using namespace std;
class AbstractionLayer_PoempelPosition_Properties
{
public:
    AbstractionLayer_PoempelPosition_Properties():SideLength({0,0,0,0,0,0,0,0}){}
    float getSideLength(int i){if (i<8 && i>=0)return SideLength[i]; else return -1;};
    void shift(int i);
private:

    vector<double> SideLength;
    friend class AbstractionLayer_PoempelPosition;
    friend class AbstractionLayer_1;
};


