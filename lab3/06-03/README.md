# C++ Modules build time comparison

## Measurements

### Initial build
* 1.6s - no modules
* 4.6s - with modules

### Change rational.hpp/rational.cppm
* 1.6s - no modules
* 0.8s - with modules

### Change rational.cpp
* 0.8s - no modules
* 0.4s - with modules

## Results
* The size of executables is the same (24K)
* Initial build is faster without modules
* Incremental build is faster with modules
