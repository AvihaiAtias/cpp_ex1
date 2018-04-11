/*
 * File.cpp
 *
 *  Created on: 31 במרץ 2018
 *      Author: avihai atias
 */


#include <list>
#include <vector>
#include "File.h"

//ctor
File::File(string name,Folder* parent):value(new File_RC(name)),parent(parent){}
//copy c`tor
File::File(const File& rhs):value(rhs.value){

    ++value->refCount;
}
//operator= of file actualy relevant for the ln command for the ref count
File& File::operator=(const File& rhs){
    //TODO ask and fix the duplicate names in LN command;
    if(value == rhs.value) return *this;
    if(--value->refCount == 0) delete value;

    value = rhs.value;
    ++value->refCount;

    return *this;
}
//operator[] for read command
char File::operator[](int index)const throw (BoundException){

    char c;
    int length = getSizeOfFile();

    if(index < 0) throw BoundException();
    value->inFile->seekg(index, ios_base::beg);
    value->inFile->get(c);
    cout << c << endl;
    value->inFile->close();
    return c;
}
//operator[] for the write command
char File::operator[](pair<int,char> item) throw (BoundException){
    int length = getSizeOfFile();
    try{
        if(item.first < 0) throw BoundException("Error:illegal index");
    }catch (BoundException e){
        cerr << e.what() <<endl;
    }
    value->outFile->seekp(item.first, ios_base::beg);
    value->outFile->put(item.second);
    return item.second;
}
//method return the size of file;
int File::getSizeOfFile()const{

    value->inFile->seekg(0,ios_base::end);
    int length = value->inFile->tellg();
    value->inFile->seekg(0,ios_base::beg);
    return length;

}
//explained in the terminal
void File::touch()const{
    value->outFile->open(value->name);
    value->outFile->flush();
    value->outFile->close();
    value->lastTime = chrono::system_clock::to_time_t ( chrono::system_clock::now() );
}
//actualy make the copy between 2 files
void File::copy(File* tgt){
    string buffer;
    ifstream& in = *value->inFile;
    ofstream& out = *tgt->value->outFile;
    std::cout << __FILE__;
    in.seekg(0,ios_base::end);
    long length = in.tellg();
    try {
        if(!in.is_open() || !out.is_open())throw FailToOpenFile("cannot open the file");
            in.seekg(0,ios_base::beg);
            out.seekp(0,ios_base::beg);
            out.put('a');
            getline(in,buffer, static_cast<char>EOF);
            out.write(buffer.c_str(),length);
    }catch (FailToOpenFile e){
        cerr << e.what() << endl;
    }
}
//remove the file
void File::remove() {
    delete this;
}
//same as copt but with delete
void File::move(File* tgt){
    copy(tgt);
    delete this;
}
//print the content of the file
void File::cat()const{
    string buffer;
    ifstream& in = *value->inFile;
    in.seekg(0,ios_base::beg);

    try {
        if(!in.is_open())throw FailToOpenFile("cannot open the file");
        getline(in,buffer, static_cast<char>EOF);
        cout<<buffer;
    }catch (FailToOpenFile e){
        cerr << e.what() << endl;
    }

}
//cound the rows words and chars
void File::wc(){
    ifstream& in = *value->inFile;
    in.seekg(0,ios_base::beg);
    int sumOfChars = 0;
    int sumOfWords = 0;
    int sumOfRows = 0;
    string buffer;

    try {
        if(!in.is_open())throw FailToOpenFile("cannot open the file");
    }catch (FailToOpenFile e){
        cerr << e.what() << endl;
    }
    getline(in,buffer, static_cast<char>EOF);
    for(int i = 0;i<buffer.length();i++,sumOfChars++){
        if(buffer[i] == ' ' || buffer[i] == '\t')sumOfWords++;
        if(buffer[i] == '\n'){
            sumOfRows++;
            sumOfWords++;
        }
    }

    cout << "characters = " << sumOfChars << endl;
    cout << "words = "<< sumOfWords << endl;
    cout << "rows = "<< sumOfRows << endl;
}
//make the hard link and affect on the ref count
File& File::ln(File* src){
    operator=(*src);
}
//private method support at lproot command
void File::printCurrentDetails(){
    cout<< getName() << " " << getRefCount()<< endl;
}
//File_RC private struct in File object d`tor
File::File_RC::File_RC(string name):refCount(1),name(name),inFile(new ifstream),
outFile(new ofstream),lastTime(chrono::system_clock::to_time_t (chrono::system_clock::now())) {
    try{
        outFile->open(name.c_str());
        inFile->open(name.c_str());
        if(!inFile->is_open() || !outFile->is_open())
            throw FailToOpenFile("Error cannot open the file");
    }catch ( FailToOpenFile e){
        cerr << e.what() <<endl;
    }
}
File::File_RC::~File_RC(){
    delete inFile;
    delete outFile;
}

