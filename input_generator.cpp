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
    unsigned long int size = 16; //число чисел в сортируемом файле
    for (unsigned long int i = 0; i < size; ++i)
    {
        if ((rand() % 2) == 1)
            mul = -1;
        for (int j = 0; j < 31; ++j)
        {
            buf += (rand() % 2) * pow (2,j);
        }
        cout << buf << " ";
        fout.write((char*)&buf, sizeof(buf));
    }
    cout << endl;
    fout.close();
    if(!fout.good()) {
        cout << "Error occurred at writing time!" << endl;
        return 1;
   }
    return 0;
}