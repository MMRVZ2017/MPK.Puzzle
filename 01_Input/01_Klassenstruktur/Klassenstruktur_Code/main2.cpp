#include <iostream>
#include "vector.h"

#define NR_OF_PARTS 1008

using namespace std;

class Part
{
public:
    Part()
    {
        type = 0;
		for(int i=0;i<8;i++)
        {
            connections[i] = 0;
        }
        position = -1;	//to indicate, that no solution has been found yet
        orientation = -1;
    }
    ~Part() {}
protected:
    uint8_t type;	//inner:0, edge:1, corner:2
	uint8_t connections[8];	//mating connections (4 sides): female:0, male:1, none:2
	uint16_t position;	//position on the solved board: 0-1007
	uint8_t orientation;	//CW rotation = orientation*90 (deg)
};

int main(){
	Vector <Part> part_array(NR_OF_PARTS);
	
	return 0;
}
