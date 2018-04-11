/*
 * Folder.h
 *
 *  Created on: 31 במרץ 2018
 *      Author: aviha
 */

#ifndef FOLDER_H_
#define FOLDER_H_

#include <list>
#include <string>
#include <iomanip>
#include "File.h"

using namespace std;
class Folder {

public:
    Folder(string name,Folder* parent):name(name),files(),folders(),parent(parent),
        lastTime(chrono::system_clock::to_time_t (chrono::system_clock::now())){}
    virtual ~Folder();
    void mkdir(string folderName);
    File* findFile(string fileName);
    Folder* findFolder(string folderName);
    void ls();
    void lproot();
    void pwd();
    void move(string sourceFileName,Folder* target,string targetFileName);
    void removeFile(string fileName);
    void copy(string sourceFileName,Folder* target,string targetFileName);
    File* makeFile(string fileName);
    string getName(){return name;}
    int getTotal()const{return files.size()+folders.size();}
    Folder* getParent(){return parent;}
    void removeFolder(string folderName);
private:
    time_t lastTime;
    string name;
    list <File*> files;//list of the included files;
    list <Folder*> folders;//list of the included folders;
    Folder* parent;

    list<Folder*>::iterator findFolderMatch(string folderName);
    list<File*>::iterator findFileMatch(string fileName);
    void printCurrentDetails()const;
    void printAllIncluded();
};

#endif /* FOLDER_H_ */
