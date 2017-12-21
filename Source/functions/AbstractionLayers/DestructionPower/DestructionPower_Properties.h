//
// Created by mpapa on 05.12.2017.
//
#pragma once

#define DESTRUCTION_INIT 0.5
#include <stdint.h>
#include <map>
#include <vector>

using namespace std;

class DestructionPower_Properties
{
public:
    explicit DestructionPower_Properties();
    map<int,float> getSpeedTable() {return SpeedTable;}
    void setSpeedTable();

private:

    static map<int,float> SpeedTable;
    vector<float> DestructionArray;
    friend class DestructionPower;

};