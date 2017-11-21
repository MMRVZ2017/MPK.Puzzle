#include <stdint.h>

class SolutionElement
{
public:
	SolutionElement();
	~SolutionElement();

public:
	uint16_t index;  //x.jpg
	uint8_t orientation;    //CW rotation = orientation*90 (deg): 0,1,2,3
};

