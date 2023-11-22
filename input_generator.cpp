#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;


int main()
{
    srand(time(nullptr));
    ofstream fout("input.bi");
    unsigned long int size = 50;
    for (unsigned long int i = 0; i < size; ++i)
    {
        for (int j = 0; j < 32; ++j)
        {
            fout << (bool)(rand() % 2);
        }
    }
    fout.close();
    return 0;
}