#include <iostream>
#include <fstream>
#include <ctime>
#include <math.h>
using namespace std;


int main()
{
    srand(time(nullptr));
    ofstream fout("input.bin", ios::binary);
    int buf = 0;
    int mul = -1;
    unsigned long int size = 50;
    for (unsigned long int i = 0; i < size; ++i)
    {
        if ((rand() % 2) == 1)
            mul = -1;
        for (int j = 0; j < 31; ++j)
        {
            buf += (rand() % 2) * pow (2,j);
        }
        fout.write((char*)&buf, sizeof(buf));
    }
    fout.close();
    return 0;
}