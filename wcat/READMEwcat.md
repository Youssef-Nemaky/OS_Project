
# Wcat Project 

The program wcat is a simple program.  it reads a file as specified by the user and prints its contents. we can use it  as follows, in which the user wants to see the contents of main.c, and thus the user types






## Usage/Examples

```javascript
joe@Joe-Laptop:~ ./wcat main.c
#include <stdio.h>
```
As shown, wcat reads the file main.c and prints out its contents

To create the wcat binary, you'll be creating a single source file, wcat.c, and writing a little C code to implement this simplified version of cat. To compile this program, you will do the following

```javascript
joe@Joe-Laptop:~ gcc -o wcat wcat.c -Wall -Werror
#include <stdio.h>
```
The fopen() function "opens" a file, which is a common way in UNIX systems to begin the process of file access

```javascript
FILE *fp = fopen("main.c", "r");
if (fp == NULL) {
    printf("cannot open file\n");
    exit(1);
}
```

Your program wcat can be invoked with one or more files on the command line; it should just print out each file in turn.

In all non-error cases, wcat should exit with status code 0, usually by returning a 0 from main() (or by calling exit(0)).
## Running Tests

To run tests, run the following command

```bash
wcat test.txt
For Example :
  joe@Joe-Laptop:~/os_project/wcat$ ./wcat test.txt
```


## Authors

- [Yousef Mamdouh El Nemaky]      {BN:191901741}
- [Ahmed Almotasem Mohamed]       {BN:191901716}
- [Mourad El Deen Nashaat]        {BN:1919017}
- [Abdelaziz Abouzied Abdelaziz]  {BN:191901741}
- [Nada Mohamed Hanafy]           {BN:191901741}
