#include <iostream>
#include "vector.h"
#define Number_Parts 1008
#define Number_Corners 4
#define Number_Edges 120
#define Number_Inners 884

using namespace std;

class Part
{
public:
    Part()
    {
        for(int i=0;i<8;i++)
        {
            Orientation[i] = 0;
        }
    }
    ~Part() {}
protected:
    Vector<Part> Part_Array(Number_Parts);
    uint8_t Orientation[8];
};

class Corner : public Part
{
public:
    Corner() {}
    ~Corner() {}
private:
    Vector<Part *> Corner_Array(Number_Corners);
};

class Edge : public Part
{
public:
    Edge() {}
    ~Edge() {}
private:
    Vector<Part *> Edge_Array(Number_Edges);
};

class Inner : public Part
{
public:
    Inner() {}
    ~Inner() {}
private:
    Vector<Part *> Inner_Array(Number_Inners);
};
