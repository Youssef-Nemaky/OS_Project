
# Parallel Zip

We built wzip, a basic compression tool based on run-length encoding, in a previous project. We'll do something similar here, except instead of using threads, we'll utilise them to create a parallel version of zip. This version will be known as pzip.


## Overview

The underlying specification will be the same, with run-length encoding as the basic approach.

Externally, your parallel zip (pzip) will seem the same; the usual command-line usage will be as follows:

```bash
  joe@Joe-Laptop:~/os_project/pzip file > file.z
```
## Considerations

Doing so effectively and with high performance will require  to address (at least) the following issues:


- Make threads equal to the number of the files
- Calculate the Size of each file
- Create a buffer with size equal to the sum of all files size
- Divide the Buffer on the threads Equally
- The rest of the buffer size will return to the main to compress it
- The threads will compress the buffer and the compressed data is sent to another buffer its size equal twice the buffer size(array of array of structures)
- using for loop we will merge the the data on array of array of structure the thae main is added 
- Now the buffer contain the zipped data merged 
## Authors

- [Yousef Mamdouh El Nemaky]  {BN:191901741}
- [Ahmed Almotasem Mohamed] {BN:191901716}
- [Mourad El Deen Nashaat] {BN:1919017}
- [Abdelaziz Abouzied Abdelaziz] {BN:191901741}
- [Nada Mohamed Hanafy] {BN:191901741}

