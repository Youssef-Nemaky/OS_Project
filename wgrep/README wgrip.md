
# Wgrip Project 

The second tool we will build is called wgrep, a diffrent type of the UNIX tool grep. This tool search through a file, line by line, trying to find a user-specified search term in the line. If a line has the word within it, the line is printed out, otherwise it will return empty





## Usage/Examples

```javascript
joe@Joe-Laptop:~ ./wgrep foo bar.txt
this line has foo in it
so does this foolish line; do you see where?
even this line, which has barfood in it, will be printed.
```
wgrep is always passed a search term and zero or more files to grep through

The matching is case sensitive. Thus, if searching for foo, lines with Foo will not match

If wgrep is run through no command-line arguments, it will print "wgrep: searchterm

If you supply wgrep the empty text as a search string, it will match either NO lines or ALL lines, which is OK.
## Running Tests

To run tests, run the following command

```bash
wcat test.txt
For Example :
  joe@Joe-Laptop:~/os_project/wgrip$ ./wgrip test.txt
```


## Authors

- [Yousef Mamdouh El Nemaky]      {BN:191901741}
- [Ahmed Almotasem Mohamed]       {BN:191901716}
- [Mourad El Deen Nashaat]        {BN:1919017}
- [Abdelaziz Abouzied Abdelaziz]  {BN:191901741}
- [Nada Mohamed Hanafy]           {BN:191901741}