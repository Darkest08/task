#include<iostream>
#include <math.h>
#include <fstream>
using namespace std;

int glenth = 32;

int main()
{
    string path;
    cin >> path;
    ifstream in(path);
    int eof = 0;
    int size = 0;
    do 
    {
        int val = 0;
        in.read((char*)&val, sizeof(val));
        if (!in.eof())
        {
            ++size;
            cout << val << ' ';
        }
        else
        {
            eof = 1;
        }
    }
    while(eof != 1);

    cout << endl << size << endl;
    return 0;
}
