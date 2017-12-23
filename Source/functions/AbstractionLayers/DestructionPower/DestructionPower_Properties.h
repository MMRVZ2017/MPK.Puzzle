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
    void setSpeedTable();
    static map<int,float> SpeedTable;


    vector<float> DestructionArray;

private:
    friend class DestructionPower;

};