name: avihai atias
id: 301853446

explanation:
in this work i was tried to imagine as i can to the ubuntu terminal.
almost all the ways that work on ubuntu should be work here;

for example:
mkdir folder1
mkdir ./folder1
mkdir ../folder1

or
chdir folder1
chdir ./folder1
chdir ../folder1

and its relevant for all the commands.

explanation of the design(shortly from bottom to top):
1. File object
    include:
        private class FILE_RC that actually handle the file (name,fstream,refcount,last time changing and more);

2. Folder object
    include:
        list of Files and list of Folders that included
        also include name and the last time changing
3. FileSystem
    only decoration that include the root directory (Folder object)

4. Terminal
    include:
        the current path that demonstrate in the screen in the start of the lines in the console
        the bank of the all commands (in vector).
        the current activate folder from the path (pointer to Folder)
        and the FileSystem - dynamic allocation

also i made another header file that include the all exception handler that in use by the program (see Exceptions.h)
in my design I preffered to handle the exceptions from the terminal class to not spend running-time "for nothing" so all the exceptions except the file exeptions (mean file opening for example) handled from outside the file and folder objects.

thanks.
