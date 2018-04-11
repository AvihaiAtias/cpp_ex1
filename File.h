/*
 * File.h
 *
 *  Created on: 31 במרץ 2018
 *      Author: aviha
 */

#ifndef FILE_H_
#define FILE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "Exceptions.h"

using namespace std;
class Folder;
class File {
public:
    File(string name,Folder* parent);
    File(const File& rhs);
    File& operator=(const File& rhs);
    char operator[](int index)const throw (BoundException);
    char operator[](pair<int,char>) throw (BoundException);
    void touch()const;
    void remove();
    void move();
    void cat()const;
    void wc();
    void move(File *tgt);
    void copy(File *tgt);
    void printCurrentDetails();
    File& ln(File* src);
    string getName(){return name;}
    ifstream& getInFile(){return *value->inFile;}
    ofstream& getInOutFile(){return *value->outFile;}
    virtual ~File(){if(--value->refCount == 0) delete value;}
    time_t getLastTime(){return value->lastTime;}
    int getRefCount(){return value->refCount;}
private:
    struct File_RC{
         File_RC(string name);
        ~File_RC();
        int refCount;
        ifstream* inFile;
        ofstream* outFile;
        time_t lastTime;
    };
    string name;
    File_RC *value;
    Folder* parent;//reference to the folder that include this file;
};

#endif /* FILE_H_ */
