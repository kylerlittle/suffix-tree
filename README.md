# Suffix Tree Construction

## Usage
To build the code.
```
make
```

To run the code using Makefile
```
make run FASTAFILE?=<src/data/*.fasta> ALPHABETFILE?=<src/data/*.txt>
```
Note that ```$FASTAFILE``` and ```$ALPHABETFILE``` need not be passed as parameters to make. We use defaults for quick dev testing.

To run the code using the binary
```
./build/SuffixTree <src/data/*.fasta> <src/data/*.txt>
```

To clean up the build artifacts
```
make clean
```

To zip the project
```
make zip
```

## Docker Usage
Simply run
```
./start_docker.sh
```
You can run the code using the previous instructions now. I added this extra level of virtualization in order to make the code more portable.