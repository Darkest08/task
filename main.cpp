#define _WIN32_WINNT  0x0501
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
#include <algorithm>
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

int ReadValue(ifstream & in, bool & eof)
{
    int val = 0;
    char buf = 0;
    int mul = 1;
    eof = 0;
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
    }
    else
    {
        eof = 1;
    }
    return val * mul;
}

void WriteValue(ofstream & out, int & val)
{
        int buf = val;
        if (buf < 0)
            out << 1;
        else
            out << 0;
        int res = 0;
        buf = abs(buf);
        for (int i = glenth - 2; i > -1; --i)
        {
            res = trunc(buf / pow(2, i));
            out << res;
            if (res == 1)
                buf-=pow(2, i);
        }
}

void ShotSort(ifstream & in, ofstream & out, int len) // сортировка файла
{
    vector<int> buf(len); //буфер чисел
    bool eof = 0;
    for (int i=0; i < len; ++i)
    {
        buf[i] = ReadValue(in, eof);
    }
    sort(begin(buf), end(buf)); // сортировка
    for(auto& val: buf )
    {
        WriteValue(out, val);
    }
}

int MinFinder(int * arr, const int & n)
{
    int min = INT_FAST32_MAX;
    int minId = 0;
    for (int i = 0; i < n; ++i)
    {
        if (arr[i] <= min)
        {
            min = arr[i];
            minId = i;
        }
    }
    return minId;
}

int main()
{
    // блок объявления основных данных
        string inpath = "input.bi";
        const string base_child_name = "child";
        const string outpath = "output.bi";
        unsigned int fsize = FileSize(inpath); //размер читаемого файла
            MEMORYSTATUSEX statex;
            statex.dwLength = sizeof (statex);
            GlobalMemoryStatusEx (&statex);
        unsigned int global_msize = (int)statex.ullTotalPhys; //размер оперативной памяти
        unsigned int msize = 40;//ceil(global_msize / 4); //размер доступной для кучи оперативной памяти

        int buf = ceil(fsize / msize);
        unsigned int iterations = max(buf, 1); //вычисляю число итераций работы программы (точнее число малых файлов)

    // Блок чтения и первичной сортировки

        ifstream in(inpath, ifstream::binary); // поток чтения основного файла

        ofstream * children = new ofstream[iterations]; // выделяю память для подфайлов

        int defSize = ceil(fsize / iterations / glenth); // стандартное число бит на файл
        int minSize = (fsize / glenth - (iterations-1)*defSize); //минимальное число бит на файл (остаток)
        for (int i = 0; i < iterations; ++i)
        {
            children[i] = ofstream(base_child_name + to_string(i) + ".bi"); // создаём файл буфера
            ShotSort(in, children[i], min(defSize, minSize)); // сортируем буферный файл
            children[i].close(); //закрываю сортированный файл
        }
        delete[] children; // удаляю массив на запись, так как мне он больше не нужен;
        in.close(); // закрываю входной файл, т.к. он уже считан
   
    // Блок слияния
   
        int nClosed = 0; // число закрытых буферных файлов
        ifstream * sorts = new ifstream[iterations]; // массив считываемых файлов (всё теже буферные но на чтение)
        int * redVals = new int [iterations]; // считываемые значения
        bool * eofs = new bool[iterations]; // достигнут ли конец файла i
        ofstream out(outpath);
        for (int i = 0; i < iterations; ++i)
        {
            sorts[i] = ifstream(base_child_name + to_string(i) + ".bi"); // файлы открываются
            eofs[i] = false;
        }
        for (int i = 0; i < iterations; ++i) // первичное считывание
        {
            redVals[i] = ReadValue(sorts[i], eofs[i]);
        }
        int smallestId = 0; //id наименьшего элемента
        system("PAUSE");
        do{ // сортировка вставкой по всем буферным файлам до тех пор, пока не не зкароются все
            smallestId = MinFinder(redVals, iterations);
            cout << " " << smallestId << " "<< redVals[smallestId] << endl;
            WriteValue(out, redVals[smallestId]);
            redVals[smallestId] = ReadValue(sorts[smallestId], eofs[smallestId]);

            if (eofs[smallestId] == 1)
            {
                ++nClosed;
                redVals[smallestId] = INT_FAST32_MAX;
                sorts[smallestId].close();
                    string childPath = (base_child_name + to_string(smallestId) + ".bi");
                    const int length = childPath.length();
                    char* char_array = new char[length + 1]; 
                    strcpy(char_array, childPath.c_str()); 
                    remove(char_array);
            }
        }while(nClosed != iterations);
        delete[] eofs;
        delete[] redVals;
        delete[] sorts;
        out.close();
    return 0;
}