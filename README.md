# Traveling Salesman Solver
Two implementions are written in C++ and Python using a nearest neighbor algorithm. Results are optimized further through the use of a 2-opt algorithm. Several example input files are located in `cities`.

To compile the C++ version, run the following command to create `nn-2opt`:

`g++ nn-2opt.cpp -O3 -o nn-2opt -std=c++11`

In order to find the solution for a given set of locations, run the following command:

`./nn-2opt cities/test-input-1.txt`

The equivalent command for the python version is `./nn-2opt.py cities/test-input-1.txt`.

This will output a solutions file in `cities` named `test-input-1.txt.tour`.
