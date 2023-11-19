#define _WIN32_WINNT  0x0501
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

ifstream::pos_type filesize(string & filename)
{
    ifstream in(filename, ifstream::binary);
    return in.tellg(); 
}

int main()
{
    string inpath = "input.bi";

    unsigned int fsize = filesize(inpath); //размер читаемого файла
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof (statex);
        GlobalMemoryStatusEx (&statex);
    unsigned int global_msize = (int)statex.ullTotalPhys; //размер оперативной памяти
    unsigned int msize = ceil(global_msize / 4); //размер доступной для кучи оперативной памяти

    return 0;
}