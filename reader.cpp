#include<iostream>
#include <math.h>
#include <fstream>
#include <io.h>
using namespace std;

int glenth = 32;

int main()
{
    string path;
    cin >> path;
    ifstream in(path);
    int eof = 0;
    int size = 0; //число чисел
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
    FILE *fp = fopen(path.c_str(), "r");
    fseek(fp, 0L, SEEK_END);
    int size_ = ftell(fp);       // размер (число позиций)
    rewind(fp);
    fclose(fp);
    cout << endl << size << " " << size_ << endl;
    
    in.close();
    return 0;
}
