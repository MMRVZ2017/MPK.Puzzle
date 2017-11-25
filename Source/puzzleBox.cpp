#include "puzzleBox.h"

uint8_t Puzzlebox::baseTypes[NR_PART_TYPES] = {0b01010000, 0b10100000, 0b01100000, 0b10010000, \
                                               0b01010001, 0b10100010, 0b01100001, 0b01100010, 0b10010001, 0b10010010, 0b01010010, 0b10100001, \
                                               0b01010101, 0b10101010, 0b10101001, 0b10011001, 0b01101001, 0b01010110};
int8_t Puzzlebox::numType[NR_PART_TYPES] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void Puzzlebox::sortParts(vector <Part> * piece_array)
{
    Part* currentPiece = NULL;

    //je nachdem ob 0,1,2 werden die Teile in die sorted ecke, kante oder mid vector gespeichert
    for(int i = 0; i < (*piece_array).size(); i++)
    {
        currentPiece = NULL;

        // set ID number for current part
        (*piece_array)[i].setID(i);

        // ****************** CORNERS ********************
        // piece type 0
        if      ((*piece_array)[i].getConnections() == 0b01010000 || (*piece_array)[i].getConnections() == 0b00010100 || (*piece_array)[i].getConnections() == 0b00000101 || (*piece_array)[i].getConnections() == 0b01000001 )
        {
            (*piece_array)[i].setType(0);
            numType[0]++;
        }
            // piece type 1
        else if ((*piece_array)[i].getConnections() == 0b10100000 || (*piece_array)[i].getConnections() == 0b00101000 || (*piece_array)[i].getConnections() == 0b00001010 || (*piece_array)[i].getConnections() == 0b10000010 )
        {
            (*piece_array)[i].setType(1);
            numType[1]++;
        }
            // piecetype 2
        else if ((*piece_array)[i].getConnections() == 0b01100000 || (*piece_array)[i].getConnections() == 0b00011000 || (*piece_array)[i].getConnections() == 0b00000110 || (*piece_array)[i].getConnections() == 0b10000001 )
        {
            (*piece_array)[i].setType(2);
            numType[2]++;
        }
            // piecetype 3
        else if ((*piece_array)[i].getConnections() == 0b10010000 || (*piece_array)[i].getConnections() == 0b00100100 || (*piece_array)[i].getConnections() == 0b00001001 || (*piece_array)[i].getConnections() == 0b01000010 )
        {
            (*piece_array)[i].setType(3);
            numType[3]++;
        }
            // ****************** EDGES ********************
            // piecetype 4
        else if ((*piece_array)[i].getConnections() == 0b01010001 || (*piece_array)[i].getConnections() == 0b01010100 || (*piece_array)[i].getConnections() == 0b00010101 || (*piece_array)[i].getConnections() == 0b01000101 )
        {
            (*piece_array)[i].setType(4);
            numType[4]++;
        }
            // piecetype 5
        else if ((*piece_array)[i].getConnections() == 0b10100010 || (*piece_array)[i].getConnections() == 0b10101000 || (*piece_array)[i].getConnections() == 0b00101010 || (*piece_array)[i].getConnections() == 0b10001010 )
        {
            (*piece_array)[i].setType(5);
            numType[5]++;
        }
            // piecetype 6
        else if ((*piece_array)[i].getConnections() == 0b01100001 || (*piece_array)[i].getConnections() == 0b01011000 || (*piece_array)[i].getConnections() == 0b00010110 || (*piece_array)[i].getConnections() == 0b10000101 )
        {
            (*piece_array)[i].setType(6);
            numType[6]++;
        }
            // piecetype 7
        else if ((*piece_array)[i].getConnections() == 0b01100010 || (*piece_array)[i].getConnections() == 0b10011000 || (*piece_array)[i].getConnections() == 0b00100110 || (*piece_array)[i].getConnections() == 0b10001001 )
        {
            (*piece_array)[i].setType(7);
            numType[7]++;
        }
            // piecetype 8
        else if ((*piece_array)[i].getConnections() == 0b10010001 || (*piece_array)[i].getConnections() == 0b01100100 || (*piece_array)[i].getConnections() == 0b00011001 || (*piece_array)[i].getConnections() == 0b01000110 )
        {
            (*piece_array)[i].setType(8);
            numType[8]++;
        }
            // piecetype 9
        else if ((*piece_array)[i].getConnections() == 0b10010010 || (*piece_array)[i].getConnections() == 0b10100100 || (*piece_array)[i].getConnections() == 0b00101001 || (*piece_array)[i].getConnections() == 0b01001010 )
        {
            (*piece_array)[i].setType(9);
            numType[9]++;
        }
            // piecetype 10
        else if ((*piece_array)[i].getConnections() == 0b01010010 || (*piece_array)[i].getConnections() == 0b10010100 || (*piece_array)[i].getConnections() == 0b00100101 || (*piece_array)[i].getConnections() == 0b01001001 )
        {
            (*piece_array)[i].setType(10);
            numType[10]++;
        }
            // piecetype 11
        else if ((*piece_array)[i].getConnections() == 0b10100001 || (*piece_array)[i].getConnections() == 0b01101000 || (*piece_array)[i].getConnections() == 0b00011010 || (*piece_array)[i].getConnections() == 0b10000110 )
        {
            (*piece_array)[i].setType(11);
            numType[11]++;
        }
            // ****************** MIDS ********************
            // piecetype 12
        else if ((*piece_array)[i].getConnections() == 0b01010101 )
        {
            (*piece_array)[i].setType(12);
            numType[12]++;
        }
            // piecetype 13
        else if ((*piece_array)[i].getConnections() == 0b10101010 )
        {
            (*piece_array)[i].setType(13);
            numType[13]++;
        }
            // piecetype 14
        else if ((*piece_array)[i].getConnections() == 0b10101001 || (*piece_array)[i].getConnections() == 0b01101010 || (*piece_array)[i].getConnections() == 0b10011010 || (*piece_array)[i].getConnections() == 0b10100110 )
        {
            (*piece_array)[i].setType(14);
            numType[14]++;
        }
            // piecetype 15
        else if ((*piece_array)[i].getConnections() == 0b10011001 || (*piece_array)[i].getConnections() == 0b01100110 )
        {
            (*piece_array)[i].setType(15);
            numType[15]++;
        }
            // piecetype 16
        else if ((*piece_array)[i].getConnections() == 0b01101001 || (*piece_array)[i].getConnections() == 0b01011010 || (*piece_array)[i].getConnections() == 0b10010110 || (*piece_array)[i].getConnections() == 0b10100101 )
        {
            (*piece_array)[i].setType(16);
            numType[16]++;
        }
            // piecetype 17
        else if ((*piece_array)[i].getConnections() == 0b01010110 || (*piece_array)[i].getConnections() == 0b10010101 || (*piece_array)[i].getConnections() == 0b01100101 || (*piece_array)[i].getConnections() == 0b01011001 )
        {
            (*piece_array)[i].setType(17);
            numType[17]++;
        }
        else
        {
            cout << "WRONG PART DETECTED! ID: " << (*piece_array)[i].getID() << " CONNECTIONS: " << bitset<8>((*piece_array)[i].getConnections()) << endl << "ABORT" << endl;
            abort();
        }

        currentPiece = &(*piece_array)[i];
        type[(*piece_array)[i].getType()].push_back(currentPiece);
    }

}

int8_t Puzzlebox::countType(int type)
{
    if (type >= 0 && type < NR_PART_TYPES)
        return numType[type];
    else
        return -1;
}

// Print all vectors
void Puzzlebox::printAllVectors()
{
    for(int i=0; i < NR_PART_TYPES; i++)
    {
        printVector(i);
    }
}

void Puzzlebox::printVector(int vectorNumber) // print all elements of a vector
{
    cout << "Vector with Piece Type: " << vectorNumber << endl;

    for(int i=0; i < type[vectorNumber].size() ; i++)
    {
        printPart(type[vectorNumber][i]);
        printID(type[vectorNumber][i]);
        cout << endl;
    }
    cout << endl;
}

// Funktion überladen für selbe Version nur mit Part als Input Parameter
void Puzzlebox::printPart(Part * piece)
{
    cout << bitset<8>((*piece).getConnections());
}

void Puzzlebox::printID(Part * piece)
{
    cout << "   ID: " <<(*piece).getID();
}

void Puzzlebox::printTypeArr() {
    cout << "Types Available: ";
    for(int i = 0; i < NR_PART_TYPES; i++){
        cout << signed(numType[i]) << " ";
    }
    cout << endl;
}




/*int main()
{
    // INTERFACE TO GROUP REITER, BAUMGARTNER
    vector <Part> part_array(NR_PARTS);
    vector <Part *> corners_array(NR_CORNERS);
    vector <Part *> edges_array(NR_EDGES);
    vector <Part *> inners_array(NR_INNERS);
    // --------------------------------------

    // EXAMPLE PUZZLE
    part_array[0].setConnections(0b00010100); // Piece 1
    part_array[1].setConnections(0b00011010); // Piece 2
    part_array[2].setConnections(0b00100110); // Piece 3
    part_array[3].setConnections(0b00001001); // Piece 4
    part_array[4].setConnections(0b10101000); // Piece 5
    part_array[5].setConnections(0b01011001); // Piece 6
    part_array[6].setConnections(0b10101010); // Piece 7
    part_array[7].setConnections(0b01000101); // Piece 8
    part_array[8].setConnections(0b01010000); // Piece 9
    part_array[9].setConnections(0b01010010); // Piece 10
    part_array[10].setConnections(0b01010010); // Piece 11
    part_array[11].setConnections(0b10000010); // Piece 12
    // END OF PUZZLE DECLARATION

    vector <Part> * p_part_array; // create Pointer of vector part_array
    p_part_array = & part_array;

    Puzzlebox myPuzzlebox;
    myPuzzlebox.sortParts(p_part_array);
    myPuzzlebox.printAllVectors();

    return 5;
}*/
