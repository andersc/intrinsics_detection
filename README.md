![Logo](cpuidlogo.jpg)

# Intrinsics detection

A header only x86_64 intrinsic detector. The detection is made through the usage of the **cpuid** instruction in runtime and is targeting the naming used at 
[Intel® Intrinsics Guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html).

This solution is used when you need to detect capabilities during runtime. For example if you got various optimisations targeting specific hardware support.

## Build and Run status

[![mac_x64](https://github.com/andersc/intrinsics_detection/actions/workflows/mac_x64.yml/badge.svg)](https://github.com/andersc/intrinsics_detection/actions/workflows/mac_x64.yml)

[![ubuntu_x64](https://github.com/andersc/intrinsics_detection/actions/workflows/ubuntu_x64.yml/badge.svg)](https://github.com/andersc/intrinsics_detection/actions/workflows/ubuntu_x64.yml)

[![win_x64](https://github.com/andersc/intrinsics_detection/actions/workflows/win_x64.yml/badge.svg)](https://github.com/andersc/intrinsics_detection/actions/workflows/win_x64.yml)

## Usage

Copy the header to your project and ->

```cpp

#include "getintrin.h"

// Create the GetIntrin object
auto lIntrinCap = GetIntrin();

// Returns a struct containing this CPU's information and capabilities
auto lCPUInfo = lIntrinCap.getCapabilities();

// Helper method for getting friendly names -> getFeatureName
for (auto &rCap: lCPUInfo.mCapabilities) {
    std::cout << lIntrinCap.getFeatureName(rCap) << std::endl;
}

// Helper method for evaluating if a feature is supported -> hasFeature
if (lIntrinCap.hasFeature(GetIntrin::Instructions::AVX2)) {
    std::cout << std::endl << "This CPU has AVX2 support." << std::endl;
} else {
    std::cout << std::endl << "This CPU does not have AVX2 support." << std::endl;
}

```

## License

*MIT*

Read *LICENCE* for details

