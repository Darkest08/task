#include<iostream>
#include <math.h>
#include <fstream>
#include <io.h>
using namespace std;

int glenth = 32;

int main()
{
    cout<< "Enter the path" << endl;
    string path;
    cin >> path;
    ifstream in(path,  ios::out | ios::binary);
    int eof = 0;
    int size = 0; //число чисел
    FILE *fp = fopen(path.c_str(), "r");
    fseek(fp, 0L, SEEK_END);
    int size_ = ftell(fp);       // размер (число позиций)
    rewind(fp);
    fclose(fp);
    for (int i = 0; i < size_ / 4; ++i)
    {
        int val = 0;
        in.read((char*)&val, sizeof(val));
        ++size;
        cout << val << ' ';
    }
    in.close();

    cout << endl << size << " " << size_ << endl;
    
    
    return 0;
}
