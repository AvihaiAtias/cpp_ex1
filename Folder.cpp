/*
 * Folder.cpp
 *
 *  Created on: 31 במרץ 2018
 *      Author: avihai atias
 */

#include "Folder.h"
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <vector>
//d`tor of folder - destroy the all files and folder that included
Folder::~Folder(){
    list<Folder*>::iterator itFolder = folders.begin();
    while(itFolder != folders.end()) delete(*itFolder++);
    folders.clear();

    list<File*>::iterator itFile = files.begin();
    while(itFile != files.end()) delete(*itFile++);

    files.clear();
}
//make the directory
void Folder::mkdir(string folderName){
    Folder* newFolder = new Folder(folderName,this);
    folders.push_back(newFolder);
}
//recursive printing for all the file system;
void Folder::lproot(){
    if(this == NULL) return;
    printCurrentDetails();
    printAllIncluded();
    list<Folder*>::iterator first = folders.begin();
    list<Folder*>::iterator last = folders.end();

    while(first != last){
        (*first)->lproot();
        first++;
    }
}
//private method support lproot command
void Folder::printAllIncluded(){
    list<File*>::iterator first = files.begin();
    list<File*>::iterator last = files.end();
    while(first != last) {
        (*first)->printCurrentDetails();
        ++first;
    }
}
//private method support lproot command - print the details of the current folder
void Folder::printCurrentDetails()const{//ordering the all path of the folder
    Folder* folder = const_cast<Folder*>(this);
    list<string> currPath;
    vector<string> temp;
  while(folder != NULL){
      currPath.push_front(folder->getName());
      folder = folder->getParent();
  }
    list<string>::iterator first = currPath.begin();
    list<string>::iterator last = currPath.end();

    while(first != last)
        temp.push_back(*first++);
    for(unsigned int i = 0;i < temp.size();i++){
        cout << temp[i];
        if(i+1 < temp.size()) cout << '/';
    }
    cout <<':'<<endl;
}
//private method search the match folder and return iterator
list<Folder*>::iterator Folder::findFolderMatch(string folderName){
    list<Folder*>::iterator it = folders.begin();

    while(it != folders.end()){
        if((*it)->name == folderName) return it;
        ++it;
    }
    return folders.end();
}
//private method search the match file and return iterator
list<File*>::iterator Folder::findFileMatch(string fileName){
    list<File*>::iterator it = files.begin();
    list<File*>::iterator last = files.end();
    while(it != last){
        if((*it)->getName() == fileName) return it;
        it++;
    }
    return files.end();
}
//search the match file and return pointer to file
File* Folder::findFile(string fileName){
    list<File*>::const_iterator it = findFileMatch(fileName);
    list<File*>::const_iterator last = files.end();
    if(it != last) return *it;

    return NULL;
}
//the copy command
void Folder::copy(string sourceFileName,Folder* target,string targetFileName){
    File* src = findFile(sourceFileName);
    File* tgt = target->findFile(targetFileName);
    if(tgt == NULL) tgt = target->makeFile(targetFileName);//make file if target file not exist
    src->copy(tgt);
}
//same as copy but remove the source after
void Folder::move(string sourceFileName,Folder* target,string targetFileName){
    File* src = findFile(sourceFileName);
    File* tgt = target->findFile(targetFileName);
    if(tgt == NULL) tgt = target->makeFile(targetFileName);
    src->move(tgt);
    files.remove(src);//TODO probably its will be a-runTime error try to remove deletedItem
}
//search the match folder and return pointer to folder
Folder* Folder::findFolder(string folderName){
    list<Folder*>::const_iterator it = findFolderMatch(folderName);
    list<Folder*>::const_iterator last = folders.end();
    if(it != last) return *it;

    return NULL;
}
//make file in this folder
File* Folder::makeFile(string fileName){
    if(findFile(fileName) != NULL) files.remove(findFile(fileName));

    files.push_front(new File(fileName,this));
    return findFile(fileName);
}
//remove folder from the list
void Folder::removeFolder(string folderName){
    Folder* folder;
    if((folder = findFolder(folderName))!= NULL) folders.remove(folder);
}
//remove file from the list
void Folder::removeFile(string fileName){
    File* file;
    if((file = findFile(fileName)) != NULL){
        file->remove();
        files.remove(file);
    }
}
//print the all included file
void Folder::ls(){
    list<Folder*>::const_iterator folderIt;
    list<File*>::const_iterator fileIt;

    cout<<"total "<<getTotal()<<endl;
    for(folderIt = folders.begin();folderIt != folders.end();folderIt++)
        cout<<setw(15)<<"drwxr-xr-x" <<" FOLDER: " << (*folderIt)->name<<"  "<<ctime(&((*folderIt)->lastTime));


    for(fileIt = files.begin();fileIt != files.end();fileIt++){
        time_t tt = (*fileIt)->getLastTime();
        cout<<setw(15)<<"-rw-r--r--"<<" FILE:   "<<(*fileIt)->getName()<<"  "<<ctime(&tt);
    }

}









