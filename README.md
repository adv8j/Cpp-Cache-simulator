# Cache Miss Simulator

## Overview
This C/C++ program simulates cache behavior based on various parameters, allowing customization for cache size, block size, associativity, replacement policy, and write policy. The simulation takes input from a configuration file (`cache.config`) and an access sequence file (`cache.access`) present in the same directory, providing information about cache hits or misses, set indices, and stored tags.

## Supported Features
The implementation covers all parts of the problem statement, with each part contributing to the overall weightage:

1. Supports read access modeling for a direct-mapped cache with FIFO replacement policy.

2. LRU and RANDOM replacement policies.

3. Cache with associativity.

4. Write access modeling.

## Usage
Follow the instructions below to compile and execute the code:

1. **Compilation:**
    ```bash
    g++ cache.cpp -o cache
    ```

2. **Execution:**
    ```bash
    ./cache 
    ```

## Input Files
- **cache.config:** Specifies cache parameters in the given format:
    ```
    SIZE_OF_CACHE (number)
    BLOCK_SIZE (number)
    ASSOCIATIVITY (number)
    REPLACEMENT_POLICY (FIFO or LRU or RANDOM)
    WRITEBACK_POLICY (WB or WT)
    ```

- **cache.access:** Provides the access sequence in the format:
    ```
    Mode: Address
    Mode: Address
    ...
    ```
    - Mode: R (Read) or W (Write)
    - Address: Memory address accessed

## Sample
**Sample config file:**
```plaintext
32768
512
8
LRU
WB
```

**Sample input file:**
```plaintext
R: 0x00001234
R: 0x00005678
W: 0x00001234
W: 0x00009ABC
R: 0x00005678
R: 0x00001234
W: 0x00009ABC
W: 0x0000DEFF
R: 0x0000DEFF
R: 0x00001234
W: 0x00009ABC
R: 0x00005678
```

**Sample output :**
```plaintext
Address: 1234, Set: 1, Miss, Tag: 1
Address: 5678, Set: 3, Miss, Tag: 5
Address: 1234, Set: 1, Hit, Tag: 1
Address: 9abc, Set: 5, Miss, Tag: 9
Address: 5678, Set: 3, Hit, Tag: 5
Address: 1234, Set: 1, Hit, Tag: 1
Address: 9abc, Set: 5, Hit, Tag: 9
Address: deff, Set: 7, Miss, Tag: d
Address: deff, Set: 7, Hit, Tag: d
Address: 1234, Set: 1, Hit, Tag: 1
Address: 9abc, Set: 5, Hit, Tag: 9
Address: 5678, Set: 3, Hit, Tag: 5
```

