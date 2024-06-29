# Page-Ranking
PageRank (PR) is an algorithm used in by Google, to rank web pages in their search results. 

## Overview
This project is a C implementation of the PageRank algorithm, designed to run in parallel using pthreads and a thread pool. The implementation provides faster performance compared to a non-parallel version, making it suitable for handling large datasets efficiently.

## Features
- Parallel Processing: Utilizes pthreads for parallel execution, significantly improving performance.
- Thread Pool: Implements a thread pool to manage threads efficiently.
- Directed Graph: The graph is implemented as a directed graph, where each node can have multiple outgoing edges.
- Dynamic Memory Management: Uses malloc and free for dynamic memory allocation.
- Configurable: The number of indexes (nodes) and their edges can be easily adjusted in the code.

  
### PageRank Formula
Below an image that shows the mathematical formula used in the PageRank algorithm, which calculates the rank of each page based on its incoming links.

<img src="https://github.com/tomerp1812/Page-Ranking/assets/110912180/082fa9de-1107-4111-9d86-bb175b1724ae" alt="PageRank Formula" width="600">

### Sequential Code
This image illustrates a pseudo code of the sequential implementation of the PageRank algorithm.

<img src="https://github.com/tomerp1812/Page-Ranking/assets/110912180/54bbae76-3252-4bce-b209-2d9df557042a" alt="Sequential PageRank Code" width="600">

## Installation
To build and run the code, follow these steps:

Clone the repository:

```bash
git clone https://github.com/yourusername/pagerank-parallel.git
cd pagerank-parallel
```
Build the executable:

```bash
make
```
Run the program:

```bash
./main.out
```

## Requirements
- An operating system that supports pthreads (e.g., Linux, macOS).
- A C compiler (e.g., gcc).
