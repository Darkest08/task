#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

const unsigned int glength = 4; // длина слова/числа в бинарном файле (в байтах)

int FileSize(string & filename) // функция определения длины файла (в байтах, хотя должно быть в битах ._.), получает путь к файлу, выводит его размер
{
    FILE * fp = fopen(filename.c_str(), "r");
    fseek(fp, 0L, SEEK_END);    // ищем EOF
    int size = ftell(fp);       // получаем текущую позицию
    rewind(fp);
    fclose(fp);
    return size;
}

void ReadValue(ifstream & in, int & val, bool & eof) // функция чтения значения (поток чтения, возвращаемое значение (достигнут конец или нет))
{

    in.read((char*) & val, sizeof(val));
    if (in.eof())
    {
        eof = 1;
    }
}

void ShotSort(ifstream & in, ofstream & out, int len) // сортировка файла (входной поток, поток записи, длина файла)
{
    vector<int> buf(len); //буфер чисел
    bool eof = 0; // заглушка
    for (int i=0; i < len; ++i) //чтение файла
    {
        ReadValue(in, buf[i], eof); //чтение числа
    }
    sort(begin(buf), end(buf)); // сортировка
    for(auto & val: buf )
    {
        out.write((char*) & val, sizeof(val));// запись в выбранный файл
    }
}

int MinFinder(vector<int> & arr, int n, vector<bool> & eofs) //функция нахождения минимального числа в векторе (стандарт)
{
    int min = INT32_MAX;
    int minId = 0;
    for (int i = 0; i < n; ++i)
    {
        if (!eofs[i]  && arr[i] <= min ) // находим минимум, исключаем повторный выбор закрытых файлов
        {
            min = arr[i];
            minId = i;
        }
    }
    return minId;
}

void SplitSort(string inPath, int iterations, int fsize, string baseChildName, unsigned int & progress, unsigned int & progressLimit)
{
        ifstream in(inPath, ifstream::binary); // поток чтения основного файла
        vector<ofstream> children(iterations); // выделяю вектор для подфайлов
        
        int defSize = ceil(fsize / iterations / glength); // стандартное число бит на файл
        for (int i = 0; i < iterations; ++i)
        {
            children[i] = ofstream(baseChildName + to_string(i) + ".bin", ios::binary); // создаём файл буфера
            ShotSort(in, children[i], defSize); // сортируем буферный файл
            children[i].close(); //закрываю сортированный файл
            cout << ((double)(++progress) / (double)progressLimit * 100) << "%" << endl; //выводим прогресс выполнения
        }

        children.clear(); // освобождаю память с вектора
        children.shrink_to_fit(); // ^
        in.close(); // закрываю входной файл, т.к. он уже считан
}

void InsertionSort(int iterations, string outPath, string baseChildName, unsigned int & progress, unsigned int & progressLimit)
{
        bool eof = 0;
        int nClosed = 0; // число закрытых буферных файлов
        vector<ifstream> sorts(iterations); // вектор считываемых файлов (всё теже буферные но на чтение)
        vector<int> redVals(iterations); // считываемые значения
        vector<bool> eofs(iterations); // закончено ли чтение файла i
        ofstream out(outPath); // поток записи

        for (int i = 0; i < iterations; ++i)
        {
            sorts[i] = ifstream(baseChildName + to_string(i) + ".bin", ios::binary); // файлы открываются
        }

        for (int i = 0; i < iterations; ++i) // первичное считывание
        {
            ReadValue(sorts[i], redVals[i], eof);
            eofs[i] = 0;
        }

        int smallestId = 0; //id наименьшего элемента
        do{ // сортировка вставкой по всем буферным файлам до тех пор, пока не не зкароются все
            smallestId = MinFinder(redVals, iterations, eofs); //нахожу Id наименьшего числа в массиве вставки
            out.write((char*) & redVals[smallestId], sizeof(redVals[smallestId]));// записываю наименьшее в файл вывода
            ReadValue(sorts[smallestId],redVals[smallestId], eof); // читаю новое число из файла, откуда произошла запись

            if (eof) // если достигнут конец буферного файла, то он закрывается, на место числа ставится заглушка в виде максимально возможного числа
            {
                cout << ((double)(++progress) / (double)progressLimit * 100) << "%" << endl; //выводим прогресс выполнения
                ++nClosed;
                redVals[smallestId] = INT32_MAX;
                eofs[smallestId] = 1;
                sorts[smallestId].close();
                    string childPath = (baseChildName + to_string(smallestId) + ".bin");
                    remove(childPath.c_str()); // удаляю буферный файл
                eof = 0;
            }
        }while(nClosed != iterations); //когда все файлы закрылись цикл заканчивается

        out.close();
}
int main()
{
    // блок объявления основных данных
        unsigned int progress = 0; //прогресс выполнения программы
        unsigned int progressLimit = 0; // максимальный прогресс
        string inPath = "input.bin"; // стндартный путь к файлу ввода
        const string baseChildName = "child"; // имя для буферных файлов
        const string outPath = "output.bin"; //путь результирующего файла
        unsigned int msize = 1073741824; // 1 гигабайт

        // вводим имя файла
            string userInput = "";
            unsigned int userInputInt = 0;
            cout << "Input sorting binary file name[Y] or select default(input.bin)[N] the file have to be located in the same directory as this program [Y/N]" << endl;
            cin >> userInput;
            if (userInput == "Y")
            {
                cout << "Input file name to sort the file must be binary (*.bin): "<< endl;
                cin >> inPath;
            }
            else
                if (userInput == "N")
                {
                    cout << "input.bin file selected for sorting" << endl;
                }
                else
                {
                    cout << "Invalid input, programm terminated" << endl;
                    return 1;
                }

        // проверка существования файла
        ifstream iff(inPath);
            if(!iff)
            {
                cout << "File is not presented, programm terminated" << endl;
                return 2;
            }
            else 
                cout << "File is presented" << endl;
        int fsize = FileSize(inPath); //размер читаемого файла
        if (fsize == -1)
        {
            cout << "File reading error. Programm terminated." << endl;
            return 3;
        }
        int buf = ceil((double)fsize / (double)msize);
        unsigned int iterations = max(buf, 1); //вычисляю число итераций работы программы (точнее число малых файлов)
        progressLimit = iterations * 2;

    // Блок чтения и разделённой сортировки
        SplitSort(inPath, iterations, fsize, baseChildName, progress, progressLimit);

    system("Pause");
    // Блок слияния
        InsertionSort(iterations, outPath, baseChildName, progress, progressLimit);

        cout << "Programm completed successfully. Please check output.bin for the results" << endl;
    return 0;
}