#include <iostream>
#include <bitset>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <algorithm>
#include <random>

#define MAX_ABSTRAX 1
#define structdebug

using namespace std;


#include "header/input.h"

/**
 * IMPORTANT: Each new layer must be included there, to avoid compile errors
 */
#include "functions/AbstractionLayers/Layer1/AbstractionLayer_1.h"
#include "functions/AbstractionLayers/DestructionPower/DestructionPower.h"
#include "header/solve.h"
