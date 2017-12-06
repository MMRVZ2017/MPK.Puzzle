#include <stdint.h>

class Part
{
public:
    double r;
    double g;
    double b;

    Part(): connections(0){}
    ~Part() {}
    uint8_t getConnections() const
    {return connections;}

    void setConnections(uint8_t newconnections)
    {connections = newconnections;}

private:
    uint8_t connections;


};