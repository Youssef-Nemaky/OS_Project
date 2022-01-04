
# wzip and wunzip Project 

Because one (wzip) is a file compression tool and the other (wunzip) is a file decompression tool, the next tools you'll construct will come as a pair.

The sort of compression utilised here is run-length encoding, which is a simple form of compression (RLE). RLE is straightforward: if you have n characters of the same kind in a row, the compression tool (wzip) will convert them to the number n and a single occurrence of the character.




## Usage/Examples

```javascript
motasem@DESKTOP-VRS349I:~/OS_Project/wzip$ cat test.txt
aabaaaab
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
abababababababababbbbcccccaaaa
aaaaaaaacccccc
```
However, the compressed file's particular format is critical; in this case, you'll write out a 4-byte binary integer followed by a single ASCII character. As a result, a compressed file will be made up of a certain number of 5-byte entries, each of which will include a 4-byte integer (the run length) and a single character.

Use fwrite to write out an integer in binary format (rather than ASCII) (). For further information, see the man page. All output from wzip should be written to standard output (the stdout file stream, which, as with stdin, is already open when the programme starts running).
## Running Tests

To run tests, run the following command

```bash
motasem@DESKTOP-VRS349I:~/OS_Project/wzip$ cat test.txt
aabaaaab
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
abababababababababbbbcccccaaaa
aaaaaaaacccccc

motasem@DESKTOP-VRS349I:~/OS_Project/wzip$ ./wzip test.txt > test_zip.txt
motasem@DESKTOP-VRS349I:~/OS_Project/wzip$ cp test_zip.txt ../wunzip/
motasem@DESKTOP-VRS349I:~/OS_Project/wzip$ cd ../wunzip/
motasem@DESKTOP-VRS349I:~/OS_Project/wunzip$ ./wunzip test_zip.txt
aabaaaab
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
abababababababababbbbcccccaaaa
aaaaaaaacccccc

```


## Authors

- [Yousef Mamdouh El Nemaky]      {BN:191901741}
- [Ahmed Almotasem Mohamed]       {BN:191901716}
- [Mourad El Deen Nashaat]        {BN:1919017}
- [Abdelaziz Abouzied Abdelaziz]  {BN:191901741}
- [Nada Mohamed Hanafy]           {BN:191901741}