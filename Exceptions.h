//
// Created by avihai atias on 05/04/2018.
//
/**
 * this header file include the all exceptions that in use in the program
 */
#ifndef CPP_EX1_EXCEPTIONS_H
#define CPP_EX1_EXCEPTIONS_H
#include <iostream>
using namespace std;
class BoundException{
public:
    BoundException():errorMSG("BoundException"){}
    BoundException(const BoundException& rhs):errorMSG(rhs.errorMSG){}
    BoundException(string errorMassage):errorMSG(errorMassage){}
    virtual ~BoundException(){}
    const char* what(){return errorMSG.c_str();}
private:
    string errorMSG;
};
class FailToOpenFile{
public:
    FailToOpenFile():errorMSG("FailToOpenFile"){}
    FailToOpenFile(const FailToOpenFile& rhs):errorMSG(rhs.errorMSG){}
    FailToOpenFile(string errorMassage):errorMSG(errorMassage){}
    virtual ~FailToOpenFile(){}
    const char* what(){return errorMSG.c_str();}
private:
    string errorMSG;
};
class folderNotFoundException{
public:
    folderNotFoundException():errorMSG("folderNotFoundException"){}
    folderNotFoundException(const folderNotFoundException& rhs):errorMSG(rhs.errorMSG){}
    folderNotFoundException(string errorMassage):errorMSG(errorMassage){}
    virtual ~folderNotFoundException(){}
    const char* what(){return errorMSG.c_str();}
private:
    string errorMSG;
};
class fileNotFoundException{
public:
    fileNotFoundException():errorMSG("fileNotFoundException"){}
    fileNotFoundException(const fileNotFoundException& rhs):errorMSG(rhs.errorMSG){}
    fileNotFoundException(string errorMassage):errorMSG(errorMassage){}
    virtual ~fileNotFoundException(){}
    const char* what(){return errorMSG.c_str();}
private:
    string errorMSG;
};
class noCommandActivated{
public:
    noCommandActivated():errorMSG("noCommandActivated"){}
    noCommandActivated(const noCommandActivated& rhs):errorMSG(rhs.errorMSG){}
    noCommandActivated(string errorMassage):errorMSG(errorMassage){}
    virtual ~noCommandActivated(){}
    const char* what(){return errorMSG.c_str();}
private:
    string errorMSG;
};
class AllReadyExist{
public:
    AllReadyExist():errorMSG("AllReadyExist"){}
    AllReadyExist(const AllReadyExist& rhs):errorMSG(rhs.errorMSG){}
    AllReadyExist(string errorMassage):errorMSG(errorMassage){}
    virtual ~AllReadyExist(){}
    const char* what(){return errorMSG.c_str();}
private:
    string errorMSG;
};
class illegalCommand{
public:
    illegalCommand():errorMSG("illegalCommand"){}
    illegalCommand(const illegalCommand& rhs):errorMSG(rhs.errorMSG){}
    illegalCommand(string errorMassage):errorMSG(errorMassage){}
    virtual ~illegalCommand(){}
    const char* what(){return errorMSG.c_str();}
private:
    string errorMSG;
};

#endif //CPP_EX1_EXCEPTIONS_H
