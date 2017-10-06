#################################
# Parallel Gaussian Elimination #
#################################

How to compile:
---------------
From a Linux/Unix terminal execute the following:
```
$ gcc -pthread src/GE.c -o bin/GE
$ gcc src/input.c -o bin/output
```

How to generate data:
---------------------
```
$ ./bin/input <MATRIX_DIMENSION> >> <OUTPUT_FILE_NAME>
```

How to execute Gaussian Elimination program
-------------------------------------------
```
$ ./bin/GE <INPUT_DATA_FILE_PATH> <NUMBER_OF_THREADS>
```

How to run 19 trials of Gaussian Elimination
--------------------------------------------
```
$ chmod +x run.sh
$ ./run.sh <INPUT_DATA_FILE_PATH> <NUMBER_OF_THREADS> <OUTPUT_FILE_PATH>
$ cat <OUTPUT_FILE_PATH>
```
