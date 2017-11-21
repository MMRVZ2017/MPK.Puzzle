//
// Created by mpapa on 19.11.2017.
//

#ifndef MPK_PUZZLELOESER_GRUPPE3_HELPERFUNCTIONS_H
#define MPK_PUZZLELOESER_GRUPPE3_HELPERFUNCTIONS_H

#include <stdint.h>

class HelperFunctions {
public:
    // Thinks, that an uint8_t is a char
    //template <typename T>
    //static T ContinousShift(T bitStream, int shiftLength);

    /*
     * This is a function to shift a 8 bit continuous.
     *
     * The given bitstream will be shifted the given shiftLength to the left
     *      example: HelperFunctions::ContinousShift(test, 4);
     *
     * If a negative number will be given as shiftLength, the bitstream will
     * be shifted the positive shiftLength to the right
     *      example: HelperFunctions::ContinousShift(test, -4);
     */
    static uint8_t ContinuousShift(uint8_t bitStream, int shiftLength);
};


#endif //MPK_PUZZLELOESER_GRUPPE3_HELPERFUNCTIONS_H
