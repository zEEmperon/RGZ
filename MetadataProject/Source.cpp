
//Дана программа може считувати частину метаданних с mp3 файлів (ID3v1, метадані знаходяться у кінці файлу)



#include <string.h>
#include <windows.h>
#include <iostream>
#include <fstream>

using namespace std;

#define ID3_MAX_SIZE 125

typedef struct IDv3Tag
{
    char name[30];
    char artist[30];
} ID3TAG;



int main()
{


    HANDLE hFile;
    _WIN32_FIND_DATAA foundFile;
    char OriginFilePath[50], filePath[50];
    const char* fileFormat = "*.mp3";
    unsigned int pos;

    cout << "Enter the folder path (for example, C://Window//): ";
    cin.getline(OriginFilePath, 50);
    strcpy(filePath, OriginFilePath);

	hFile = FindFirstFileA(strcat(filePath, fileFormat), &foundFile);
    if (hFile == NULL) {
        cout << "There aren't *.mp3 files in such directory" << endl;
        return 1;
    }


    do{
        char* buf = new char[ID3_MAX_SIZE];
        memset((void*)buf, 0x00, ID3_MAX_SIZE);

        fstream workingOnFile;

        char* fullFilePath = new char[50];
        strcpy(fullFilePath, OriginFilePath);

        workingOnFile.open(strcat(fullFilePath, foundFile.cFileName));
        if (!workingOnFile.is_open()) {
            return 1;
        }

        workingOnFile.seekp(-ID3_MAX_SIZE, ios_base::end);

        workingOnFile.read(buf, 60);

        ID3TAG* pId3Tag = NULL;
        pId3Tag = reinterpret_cast<ID3TAG*>(buf);

        cout << "\nName:  " << pId3Tag->name << endl;
        cout << "Artist:  " << pId3Tag->artist << endl << endl;

        

        int choice;

        cout << "What do you want to do next: " << endl;
        cout << "1) Change metadata of this file\n2) Move to the next file\n\n";
        cout << "Your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:

            workingOnFile.seekp(-ID3_MAX_SIZE, ios_base::end);
            pos = workingOnFile.tellp();

            cin.get();
            cout << "\nName:  ";
            cin.getline(pId3Tag->name, 30);
            cout << "Artist:  "; 
            cin.getline(pId3Tag->artist, 30);

            workingOnFile.write(reinterpret_cast<const char*>(pId3Tag), 60);

            break;
        default: 
            continue;
            break;
        }

        delete[] buf, fullFilePath;
        workingOnFile.close();
   
    }while (FindNextFileA(hFile,&foundFile));
    
    cout << "There are no more files with .mp3 extension" << endl;

    return 0;
}