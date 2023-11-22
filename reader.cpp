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
        char buf = 0;
        int mul = 1;
        buf = in.get();
        if (!in.eof())
        {
            if (buf == '1') 
            {
                mul = -1;
            }
            for (int i = 1; i < glenth; ++i)
            {
                buf = in.get();
                if (in.eof())
                    {
                        eof = 1;
                        break;
                    }
                if (buf == '1')
                    val+=pow(2,glenth - i - 1);
            }
            ++size;
        cout << val * mul << ' ';
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
