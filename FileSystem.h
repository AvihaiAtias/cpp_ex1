/*
 * FileSystem.h
 *
 *  Created on: 31 במרץ 2018
 *      Author: aviha
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_
#include <list>
#include <iostream>
#include <vector>
#include "Folder.h"
#include "File.h"
class FileSystem {
public:
    FileSystem();
    virtual ~FileSystem();
    Folder* getRoot(){return root;}
    Folder *searchByPath(vector<string> currentPath);
private:
    Folder* root;
};

#endif /* FILESYSTEM_H_ */
