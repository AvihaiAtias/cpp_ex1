/*
 * FileSystem.cpp
 *
 *  Created on: 31 במרץ 2018
 *      Author: avihai atias
 */

#include "FileSystem.h"
//the file system only include the root file
FileSystem::FileSystem():root(new Folder("V",0)){}

FileSystem::~FileSystem() {delete root;}

