# Pseudocode interpreter / translator
This is a pseudocode interpreter wich can be used to execute real pseudocode.

It has three functions:
 + Interpreter
 + C++ -> Pseudo
 + Pseudo -> C++

# Setup
## Download
To download and compile this program you must have g++ installed, then you can do the following commands

```
git clone url
make
```

## Installation
### Windows
If you want to install this program by adding it to your program list just type
```
make install
```
and then you will be able to use the program as normal executable

If you want to even have the possibility to call this program via cmd you can type
```
make install cmd=true
```

### Linux & MacOS
Linux / MacOS installation adds this program in bash/zsh command form, so after installing it
you can call this program by typing `pseudo` or `pseudo <args>`
To install the program you can type
```
make install
```

## Uninstall
### Windows
On windows the program is installed in `C:\Programs\Pseudo` but if you remove the program it's not
going to remove all files in your OS so it's better to open a command prompt in the folder and type
```
make remove
```

### Linux & MacOS
Linux / MacOS installations require to have the makefile of this repository, so you need to download this
repository (or just the makefile) and then type
```
make remove
```