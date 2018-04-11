/*
 * Terminal.cpp
 *
 *  Created on: 31 במרץ 2018
 *      Author: avihai atias
 */

#include "Terminal.h"
#include <sstream>
#include <chrono>
//ctor of terminal initialize the all data members
Terminal::Terminal():commandsBank(),currentPath(),fileSys(new FileSystem),currentFolder(fileSys->getRoot()){
    setupCommands();
    start();
}
//private method that include the bank of the all command in my terminal <using std::map>;
void Terminal::setupCommands(){
    commandsBank.insert(pair<string,int>("read",0));
    commandsBank.insert(pair<string,int>("write",1));
    commandsBank.insert(pair<string,int>("touch",2));
    commandsBank.insert(pair<string,int>("copy",3));
    commandsBank.insert(pair<string,int>("remove",4));
    commandsBank.insert(pair<string,int>("move",5));
    commandsBank.insert(pair<string,int>("cat",6));
    commandsBank.insert(pair<string,int>("wc",7));
    commandsBank.insert(pair<string,int>("ln",8));
    commandsBank.insert(pair<string,int>("mkdir",9));
    commandsBank.insert(pair<string,int>("chdir",10));
    commandsBank.insert(pair<string,int>("rmdir",11));
    commandsBank.insert(pair<string,int>("ls",12));
    commandsBank.insert(pair<string,int>("lproot",13));
    commandsBank.insert(pair<string,int>("pwd",14));
    commandsBank.insert(pair<string,int>("exit",15));
}
//the main method that actually activate teminal.
void Terminal::start(){
    currentPath.push_back(fileSys->getRoot()->getName());
    while(true){
        vector<string> commandParts;
        printThePath();
        try {
            getCommand(commandParts);
            identificationAndActivation(commandParts);
        }catch(noCommandActivated e){ //if no command activated like push on enter button
            continue;
        }catch(illegalCommand e){ //if illegal command
            cerr << "Error:" << commandParts[0] << e.what() << endl;
        }
    }
}
//method repeated print the currend path enviroment in the start of the line;
void Terminal::printThePath()const{
    for(int i = 0;i < currentPath.size();i++){
        cout<<currentPath[i];
        if(i+1 < currentPath.size()) cout<<'/';
    }
    cout<<"$ ";
}
//this method take a peaces of the command and order it in vector<string> for the proccess
void Terminal::getCommand(vector<string>& commandParts){
    char n;
    string commandLine;
    string buff("");
    getline(cin,commandLine);
    if(commandLine == "\n" || commandLine == "")throw noCommandActivated();
    for(int i = 0;i<commandLine.length();i++){
        n = commandLine[i];
        if(n!=' ') buff+=n;
        else if((n == ' ' || n == '\t'|| n == '\n') && buff != ""){commandParts.push_back(buff);buff = "";}
    }
    if(buff != "")commandParts.push_back(buff);buff = "";

    return;
}
//this method identify the write command from the bank and activate it
void Terminal::identificationAndActivation(vector<string>& commandParts){

    map<string,int>::const_iterator it;
    it = commandsBank.find(commandParts[0]);
    if(it == commandsBank.end()) throw illegalCommand(" is illegal command!");
    if(commandParts.size() == 1) commandParts.push_back(".");
    activation(it->second,commandParts);
}
//the activation of the command
/**
 *
 * @param commandIndex  - the index of the command in map(for the switch case)
 * @param commandParts  - the all command in pieces (vector<string>)
 */
void Terminal::activation(int commandIndex,vector<string>& commandParts){

    switch(commandIndex){
        case read:
            tRead(commandParts[1],commandParts[2]);
            break;
        case write:
            tWrite(commandParts[1],commandParts[2],commandParts[3]);
            break;
        case touch:
            tTouch(commandParts[1]);
            break;
        case copy:
            tCopy(commandParts[1],commandParts[2]);
            break;
        case remove:
            tRemove(commandParts[1]);
            break;
        case move:
            tMove(commandParts[1],commandParts[2]);
            break;
        case cat:
            tCat(commandParts[1]);
            break;
        case wc:
            tWc(commandParts[1]);
            break;
        case ln:
            tLn(commandParts[1],commandParts[2]);
            break;
        case mkdir:
            tMkdir(commandParts[1]);
            break;
        case chdir:
            tChdir(commandParts[1]);
            break;
        case rmdir:
            tRmdir(commandParts[1]);
            break;
        case ls:
            tLs(commandParts[1]);
            break;
        case lproot:
            tLproot();
            break;
        case pwd:
            tPwd();
            break;
        case exit:
            tExit();
            break;
    }
}
//activate the method read of file object
void Terminal::tRead(string source,string pos){
    int index;
    Folder *parentFolder;
    File* sourceFile;
    vector<string> splitSource;
    splitThePath(splitSource,source);
    try {
        parentFolder = getLocation(splitSource);//get location return the right folder after check
        if((sourceFile = parentFolder->findFile(splitSource[splitSource.size()-1])) == NULL)
            throw fileNotFoundException("NO such file or directory ");
    } catch (folderNotFoundException e){
        cerr << e.what()<< source << endl;
        return;
    }catch (fileNotFoundException e){
        cerr << e.what() << splitSource[splitSource.size()-1] <<endl;
        return;
    }
    stringstream ss(pos);
    ss>>index;
    cout<<(*sourceFile)[index]<<endl;//activate operator[] of file object
}
//activate the method write of file object
void Terminal::tWrite(string source,string pos,string character) {
    int index; //same like tRead method - prev method
    char c;
    Folder *parentFolder;
    File* sourceFile;
    pair<int, char> couple;
    vector<string> splitSource;
    splitThePath(splitSource, source);
    try {
        parentFolder = getLocation(splitSource);
        if((sourceFile = parentFolder->findFile(splitSource[splitSource.size()-1])) == NULL)
            throw fileNotFoundException();
    } catch (folderNotFoundException e){
        cerr << e.what() << source << endl;
        return;
    }catch (fileNotFoundException e){
        sourceFile = parentFolder->makeFile(splitSource[splitSource.size()-1]); //make the file if not exist before
    }
    stringstream ss(pos);
    ss>>index;
    ss.clear();
    c = character[0];
    couple.first = index;
    couple.second = c;
    (*sourceFile)[couple]; //operator[] define to get std::pair<int(for index in file),char(for character)>
}
//activate the method touch of file object
void Terminal::tTouch(string source) {
    vector<string>splitSource;
    splitThePath(splitSource,source);
    Folder *parentFolder;
    File* sourceFile = NULL;
    try{
        parentFolder = getLocation(splitSource);//get the write folder by the command
        if(parentFolder == NULL) throw fileNotFoundException("NO such file or directory ");
    }catch (folderNotFoundException e){ //getLocation method throw folderNotFound exception
        cerr << e.what() << source << endl;
        return;
    }catch (fileNotFoundException e){
        parentFolder = currentFolder;
    }
    try{//splitSource[splitSource.size()-1] its the name of the relevant folder that should make the command from the vector
        if(parentFolder->findFolder(splitSource[splitSource.size()-1]) != NULL)
            throw AllReadyExist("cannot create file: FOLDER exists "); //to disable the option for same names
        if((sourceFile = parentFolder->findFile(splitSource[splitSource.size()-1])) != NULL) sourceFile->touch();
        else parentFolder->makeFile(splitSource[splitSource.size()-1]);//make the file if wasn`t exist
    }catch(AllReadyExist e){
        cerr << e.what() << endl;
    }
}
//activate the method copy of file object
void Terminal::tCopy(string source,string target){
    vector<string>splitSource;//source location can be  a full path so need break to pieces
    vector<string>splitTarget; //target location can be a full path so need break to pieces
    Folder* sourceFolder;
    Folder* targetFolder;
    splitThePath(splitSource,source);//breaking the source
    splitThePath(splitTarget,target);//breaking the target
    try{
        sourceFolder = getLocation(splitSource);//get the exact location of the source
        //get the exact location of the target
        if((targetFolder = getLocation(splitTarget)) == 0) targetFolder = currentFolder;
        if(sourceFolder == 0) sourceFolder = currentFolder;
        if(sourceFolder->findFile(splitSource[splitSource.size()-1]) == NULL) //if file source wasn`t find throw exception
            throw fileNotFoundException("NO such file or directory ");
    }catch(folderNotFoundException e){ //getLocation method throw exception if folder not found
        cerr << e.what() << source << endl;
        return;
    }catch (fileNotFoundException e){
        cerr << e.what() << splitSource[splitSource.size()-1] << endl;
        return;
    }
    try{//if there is folder with the same name
        if(sourceFolder->findFolder(target) != NULL)throw AllReadyExist("cannot create file: FOLDER exists ");
        sourceFolder->copy(splitSource[splitSource.size()-1],targetFolder,splitTarget[splitTarget.size()-1]);
    }catch (AllReadyExist e){
        cerr << e.what() << endl;
        return;
    }
}
//activate the method remove of file object
void Terminal::tRemove(string source){
    vector<string> splitSource;
    splitThePath(splitSource,source);
    Folder *parentFolder;
    try{
        parentFolder = getLocation(splitSource);
        if((parentFolder->findFile(splitSource[splitSource.size()-1]))== NULL)
            throw fileNotFoundException("NO such file or directory ");
    }catch (folderNotFoundException e){
        cerr << e.what() << source << endl;
        return;
    }catch(fileNotFoundException e){
        cerr << e.what() << splitSource[splitSource.size()-1] << endl;
        return;
    }
    parentFolder->removeFile(source);//the parent folder is actually remove the file
}
//activate the method move of file object actually same like copy except the remove in the end
void Terminal::tMove(string source,string target){
    vector<string>splitSource;
    vector<string>splitTarget;
    Folder* sourceFolder;
    Folder* targetFolder;
    File* src;
    splitThePath(splitSource,source);
    splitThePath(splitTarget,target);
    try{
        sourceFolder = getLocation(splitSource);
        if((src = sourceFolder->findFile(splitSource[splitSource.size()-1])) == NULL)
            throw fileNotFoundException("NO such file or directory ");
    }catch(fileNotFoundException e){
        cerr << e.what() << splitSource[splitSource.size()-1] << endl;
        return;
    }catch (folderNotFoundException e){
        cerr << e.what() << source << endl;
        return;
    }
    if((targetFolder = getLocation(splitTarget)) == NULL) targetFolder = currentFolder;
    try{
        if(targetFolder->findFolder(splitTarget[splitTarget.size()-1]) != NULL)throw AllReadyExist("cannot create file: FOLDER exists ");
        targetFolder->makeFile(splitTarget[splitTarget.size()-1]);
        sourceFolder->move(splitSource[splitSource.size()-1],targetFolder,splitTarget[splitTarget.size()-1]);
    }catch (AllReadyExist e){
        cerr << e.what() << endl;
        return;
    }
}
//activate the method cat of file object - for printing the relevant file
void Terminal::tCat(string source){
    vector<string>splitSource;
    splitThePath(splitSource,source);
    Folder *parentFolder = getLocation(splitSource);
    File* sourceFile;
    try{
        if(parentFolder == NULL) parentFolder = currentFolder;
        if((sourceFile = parentFolder->findFile(splitSource[splitSource.size()-1]))  == NULL)
             throw fileNotFoundException("NO such file or directory ");
    }catch (fileNotFoundException e){
        cerr << e.what() << splitSource[splitSource.size()-1] << endl;
        return;
    }catch (folderNotFoundException e){
        cerr << e.what() << source << endl;
        return;
    }
    sourceFile->cat();
}
//activate the wc method of file object that count the rows words and chars
void Terminal::tWc(string source){
    vector<string> splitSource;
    splitThePath(splitSource,source);
    Folder *parentFolder;
    File* file;
    try{
        parentFolder = getLocation(splitSource);
        if((file = parentFolder->findFile(splitSource[splitSource.size()-1])) == NULL)
            throw fileNotFoundException("NO such file or directory ");
    }catch (folderNotFoundException e){
        cerr << e.what() << source << endl;
        return;
    }catch (fileNotFoundException e){
        cerr << e.what() << splitSource[splitSource.size()-1] << endl;
    }
    file->wc();
}
//activate the ln method and create hard link between 2 files
void Terminal::tLn(string source,string target){
    vector<string>splitSource;
    vector<string>splitTarget;
    Folder* sourceFolder;
    Folder* targetFolder;
    File* src,*tgt;
    splitThePath(splitSource,source);
    splitThePath(splitTarget,target);

    try{
        sourceFolder = getLocation(splitSource);
        targetFolder = getLocation(splitTarget);
        if(targetFolder == 0) targetFolder = currentFolder;
        if((src = sourceFolder->findFile(splitSource[splitSource.size()-1])) == NULL)
            throw fileNotFoundException("NO such file or directory ");
    }catch(fileNotFoundException e){
        cerr << e.what()  << endl;
        return;
    }catch (folderNotFoundException e){
        cerr << e.what() << source << endl;
        return;
    }
    if((tgt = targetFolder->findFile(splitTarget[splitTarget.size()-1])) == NULL)
        tgt = targetFolder->makeFile(splitTarget[splitTarget.size()-1]);
    tgt->ln(src);
}
//activate the mkdir method and create new folder in the match place
void Terminal::tMkdir(string source){
    vector<string> splitPath;
    splitThePath(splitPath,source);
    Folder* sourceFolder;
    try{
        sourceFolder = getLocation(splitPath);
        if(sourceFolder == NULL) sourceFolder = currentFolder;
    }catch (folderNotFoundException e){
        cerr << e.what() << endl;
        return;
    }
    try{
        if(sourceFolder->findFolder(splitPath[splitPath.size()-1]) != NULL)
            throw AllReadyExist("cannot create folder: FOLDER exists ");
        if(sourceFolder->findFile(splitPath[splitPath.size()-1]) != NULL)
            throw AllReadyExist("cannot create folder: FOLDER exists ");
        sourceFolder->mkdir(splitPath[splitPath.size()-1]);
    }catch (AllReadyExist e){
        cerr << e.what() << endl;
        return;
    }

}
//activate the chdir method and change the current directory
void Terminal::tChdir(string source){
    vector<string> splitPath;
    splitThePath(splitPath,source);
    Folder* folder;
    try{
        if((folder = getLocation(splitPath)) == NULL){
            if(currentFolder->findFolder(splitPath[splitPath.size()-1])){
                folder = currentFolder->findFolder(splitPath[splitPath.size()-1]);
                currentFolder = folder;
                currentPath.push_back(splitPath[splitPath.size()-1]);
            }else{
                throw folderNotFoundException("NO such file or directory ");
            }
        }else if(splitPath[0] != "." && splitPath[0] != ".."){
            if(folder->getParent()->findFolder(splitPath[splitPath.size()-1]) == NULL){
                if(folder->findFolder(splitPath[splitPath.size()-1]) == NULL)
                    throw folderNotFoundException("NO such file or directory ");
            }
            setCurrentPath(splitPath);
            currentFolder = folder->findFolder(splitPath[splitPath.size()-1]);
        }else{
            if(splitPath[0]== ".."){
                currentPath.pop_back();
                if(folder != NULL) currentFolder = folder;
                if(splitPath.size() > 1){
                    for(int i = 1;i<splitPath.size();i++) currentPath.push_back(splitPath[i]);
                }
            }else{
                currentPath.push_back(splitPath[splitPath.size()-1]);
                currentFolder = folder->findFolder(splitPath[splitPath.size()-1]);
            }
        }
    }catch (folderNotFoundException e){
        cerr << e.what() << endl;
        return;
    }
}
//private method to break the pathes locations to vector <string>
void Terminal::splitThePath(vector<string>& splitPath,const string& source) {
    string buff = "";
    char n;
    for(int i=0;i<source.length();i++){
        if((n = source[i]) != '/') buff += n;
        else if((n == '/' || n == '\n') && buff != ""){splitPath.push_back(buff);buff = "";}
    }
    if(buff != ""){splitPath.push_back(buff);buff = "";}
}
//activate the rmdir method for deleting the all included files and folders
void Terminal::tRmdir(string source){
    vector<string> splitSource;
    splitThePath(splitSource,source);
    Folder* sourceFolder;
    Folder* parentFolder;
    try{
        sourceFolder = getLocation(splitSource);
    }catch (folderNotFoundException e){
        cerr << e.what() << endl;
        return;
    }
    //TODO check if work
    parentFolder = sourceFolder->getParent();
    parentFolder->removeFolder(sourceFolder->getName());
    if(sourceFolder->getName() == currentPath[currentPath.size()-1]){
        currentPath.pop_back();


        delete sourceFolder;

    }



}
//activate the ls command that print the all included files and directories
void Terminal::tLs(string source){
    vector<string> splitPath;
    splitThePath(splitPath,source);
    Folder* sourceFolder;
    try {
        sourceFolder = getLocation(splitPath);
    }catch (folderNotFoundException e){
        cerr << e.what() << endl;
        return;
    }
    sourceFolder->ls();
}
//activate the lproot and print the all systemFile by hirarchy
void Terminal::tLproot(){
    fileSys->getRoot()->lproot();
    cout << endl;
}
//just printing the current location
void Terminal::tPwd(){
    string home("~/HOME");
    for(int i = 0;i<currentPath.size();i++){
        home += "/";
        home += currentPath[i];
    }
    cout << home << endl;
}
//activate the exit command - to finish the program after deleting the filesystem
void Terminal::tExit(){
    delete fileSys;
    std::exit(0);
}
//d`tor
Terminal::~Terminal() {delete fileSys;}
//private method to get the match location in all the commands
Folder* Terminal::getLocation(vector<string>& location){
    Folder* p2position = fileSys->getRoot(); //start the search in the root by default
    if(location.size() == 1){ //if the path is not full mean probably the current location
        if(location[0] == fileSys->getRoot()->getName()) return p2position;//if the current location is in the root
        else if(currentFolder->findFile(location[0])) return currentFolder;//if the command is on file - search if the file exist
        else if(location[0] == "."){//if the user use in the point char he mean the current location like linux
            p2position = currentFolder;
            return p2position;
        }else if(location[0] == "..") {//if the user use in double-points he mean the parent of the current location like linux
            p2position = currentFolder->getParent();
            return p2position;
        }else return NULL;
    }
    else if(location[0] == ".") p2position = currentFolder; //same like before but if the the path is more than one length;(like ./folder/file)
    else if(location[0] == "..") p2position = currentFolder->getParent();
        for(int i = 1;i < location.size();i++){ //running threw the current path and make sure that all correct, throw exception if not
            if(p2position->findFolder(location[i]) != NULL)
                p2position = p2position->findFolder(location[i]);
            else if(i == location.size()- 1) return p2position;
            else throw folderNotFoundException("NO such file or directory ");
        }
        return p2position;

}