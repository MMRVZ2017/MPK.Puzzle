#ifndef DEFINES_H
#define DEFINES_H

// Just CHANGE these defines
#define NR_ROWS 20 //3 //20 //28
#define NR_COLS 20 //4 //20 //36
#define PRINT_MESSAGES false

// Do NOT CHANGE these defines
#define NR_PARTS (NR_ROWS * NR_COLS)
#define NR_POSITIONS NR_PARTS
#define NR_CORNERS 4
#define NR_EDGES ((NR_ROWS - 2) * 2 + (NR_COLS - 2) * 2)
#define NR_INNERS (NR_PARTS - NR_CORNERS - NR_EDGES)
#define NR_PART_TYPES 18

#endif //DEFINES_H
