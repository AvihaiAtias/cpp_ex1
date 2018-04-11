/*
 * Terminal.h
 *
 *  Created on: 31 במרץ 2018
 *      Author: aviha
 */

#ifndef TERMINAL_H_
#define TERMINAL_H_
#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include "FileSystem.h"

using namespace std;
class Terminal {
    enum command{read,write,touch,copy,remove,move,cat,wc,ln,mkdir,chdir,rmdir,ls,lproot,pwd,exit};
public:
    Terminal();
    virtual ~Terminal();
    void start();
    void tRead(string source,string pos);
    void tWrite(string source,string character,string pos);
    void tTouch(string source);
    void tCopy(string source,string target);
    void tRemove(string source);
    void tMove(string source,string target);
    void tCat(string source);
    void tWc(string source);
    void tLn(string source,string target);
    void tMkdir(string source);
    void tChdir(string source);
    void tRmdir(string source);
    void tExit();
    void tPwd();
    void tLproot();
    void tLs(string source);
private:
    map<string,int> commandsBank;
    vector<string> currentPath;
    FileSystem* fileSys;
    Folder* currentFolder;

    void identificationAndActivation(vector<string>& command);
    void getCommand(vector<string>& commandParts);
    void printThePath()const;
    void activation(int commandIndex,vector<string>& commandParts);
    void setupCommands();
    Folder* getLocation(vector<string>& location);
    void splitThePath(vector<string>& splitPath,const string& source);
    void setCurrentPath(vector<string> newPath){currentPath = newPath;}
};

#endif /* TERMINAL_H_ */
