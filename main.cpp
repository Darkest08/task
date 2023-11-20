#define _WIN32_WINNT  0x0501
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

const unsigned int glenth = 32;

int FileSize(string & filename)
{
    ifstream in(filename, ifstream::binary);
    int size = 0; 

    while (!in.eof()) { 
        in.get();       
        ++size;              
    }

    in.close();

    --size; 

    return size;
}

int ReadValue(ifstream & in)
{
    int val = 0;
    char buf = 0;
    int mul = 1;
    buf = in.get();
    if (buf == '1') 
        mul = -1;
    //cout << buf;
    for (int i = 1; i< glenth; ++i)
    {
        buf = in.get();
        //cout << buf;
        if (buf == '1')
            val+=pow(2,glenth - i);
    }
    return val * mul;
}

int main()
{

    string inpath = "input.bi";

    unsigned int fsize = FileSize(inpath); //размер читаемого файла
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof (statex);
        GlobalMemoryStatusEx (&statex);
    unsigned int global_msize = (int)statex.ullTotalPhys; //размер оперативной памяти
    unsigned int msize = ceil(global_msize / 4); //размер доступной для кучи оперативной памяти

    unsigned int iterations = ceil(fsize / msize); //вычисляю число итераций работы программы

    cout << fsize << " " << msize << endl;

    ifstream in(inpath, ifstream::binary);
    for (int i = 0; i < trunc(fsize/glenth); ++i)
    {
        cout << ReadValue(in) << endl;
    }
    in.close();

    return 0;
}