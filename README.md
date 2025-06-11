
# Distributed DCX
This repository contains my [master thesis project](https://github.com/HaagManuel/distributed_suffix_sorting/blob/main/documents/Haag_Manuel_Master_Thesis_Distributed_Suffix_Sorting.pdf) of 2025 on distributed suffix array contruction.
The code is written in C++20 using MPI for interprocess communication and the (zero-overhead) MPI Wrapper [KaMPIng](https://github.com/kamping-site/kamping).

Suffix arrays are one of the most fundamental text indices that form the basis for fast
substring search in DNA or other text corpora, text compression and many other string
algorithms. 
Their construction, also referred to as suffix sorting, consists in determining
the lexicographic order of all suffixes of a text.

Our algorithm is a distributed variant of *Difference Cover Modulo $X$ (DCX)* algorithm [[1]](https://www.cs.helsinki.fi/u/tpkarkka/publications/jacm05-revised.pdf) [[2]](https://publikationen.bibliothek.kit.edu/1000097661) that leverages the combinatorial structure of *difference covers* to efficiently sort the suffixes of the text.
A preliminary publication can be found [here](https://arxiv.org/abs/2412.10160).


![example_suffix_sorting](/images/sa_example.png)

## Resources

[[1] Juha Kärkkäinen, Peter Sanders, and Stefan Burkhardt. Linear work suffix array
construction.](https://www.cs.helsinki.fi/u/tpkarkka/publications/jacm05-revised.pdf)

[[2] Fabian Kulla and Peter Sanders. Scalable Parallel Suffix Array Construction](https://publikationen.bibliothek.kit.edu/1000097661)


## Compiling

To compile the code use the following instructions:
```
git clone --recursive git@github.com:HaagManuel/distributed_suffix_sorting.git
cmake -B release -DCMAKE_BUILD_TYPE=Release -DINCLUDE_ALL_SORTERS=ON -DOPTIMIZE_DATA_TYPES=OFF -DIPS4O_DISABLE_PARALLEL=ON
cd release && make -j 16
```

## Usage

To execute our currently best configuration run:

```sh
mpirun -n $NTASK release/cli $INPUT_FILE -c -x dc21 -s $TOTAL_INPUT_SIZE_BYTES -r ams -t 0.7 -l 2 -P 16,16 -M 64,64 -b -e 0 -Z -z 10000 -g -G -E -u 
```


## Notes
We tested our implementation with gcc 12.2.0 and IntelMPI 2021.11.0.

To reduce the memory footprint of MPI we use the following environment variables.

```sh
export I_MPI_SHM_CELL_FWD_NUM=0
export I_MPI_SHM_CELL_EXT_NUM_TOTAL=0
export I_MPI_SHM_CELL_BWD_SIZE=65536
export I_MPI_SHM_CELL_BWD_NUM=64
export I_MPI_MALLOC=0
export I_MPI_SHM_HEAP_VSIZE=0
```

## Some Results
Our best configuration is called **DC39**.

**Running times and memory blowup in our weak scaling experiments with 20MB per PE**
![scaling](/images/scaling.png)

**Throughput and memory blowup during our breakdown tests on 768 PEs.**
![throughput](/images/throughput.png)
