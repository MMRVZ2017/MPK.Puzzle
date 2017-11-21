#ifndef PART_H
#define PART_H

#include <iostream>

#define NR_PARTS 1008
#define NR_CORNERS 4
#define NR_EDGES 120
#define NR_INNERS 884

using namespace std;

class Part
{
public:
	Part() : connections(0) {}
	~Part() {}
	uint8_t getConnections() const
	{
		return connections;
	}
	void setConnections(uint8_t newconnections)
	{
		connections = newconnections;
	}

private:
	uint8_t connections;
};

/*int main()
{

	Vector <Part> part_array(NR_PARTS);
	Vector <Part *> corners_array(NR_CORNERS);
	Vector <Part *> edges_array(NR_EDGES);
	Vector <Part *> inners_array(NR_INNERS);

	return 0;
}*/


/*#include <iostream>
#include <vector>
#include <bitset>

#define NR_OF_PARTS 1008

using namespace std;

class Part
{
public:
	Part()
	{
		index = 0;
		type = 0;
		connections = 0;
	}
	~Part() {}

	uint16_t index;			//position on the solved board: 0-1007
	uint8_t type;			//inner:0, edge:1, corner:2
	bitset<8> connections;	//mating connections (4 sides): female:0, male:1, none:2
};
*/

#endif