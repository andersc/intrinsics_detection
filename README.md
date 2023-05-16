![Logo](cpuidlogo.jpg)

# Intrinsics detection

A header only X86 intrinsic detector. The detection is made through the usage of the **cupid** instruction and is targeting the naming used at 
[Intel® Intrinsics Guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html).

## Usage

Copy the header to your project and ->

```cpp

#include "getintrin.h"

// Create the GetIntrin object
auto lGetCap = GetIntrin();

// Get a vector containing the executing CPU's capabilities
auto lCapabilities = lGetCap.getCapabilities();

// The getFeatureName method returns a friendly string if handed 'GetIntrin::Instructions::VAES' 
// see the included example code for detailed

```

## License

*MIT*

Read *LICENCE* for details

