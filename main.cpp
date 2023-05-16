#include <iostream>
#include <algorithm>
#include "getintrin.h"

int main() {

    std::cout << "Get CPU intrinsics capabilities" << std::endl << std::endl;

    // Create the GetIntrin object
    auto lGetCap = GetIntrin();

    // Get a vector containing this CPU's capabilities
    auto lCapabilities = lGetCap.getCapabilities();

    // Helper method for getting friendly names -> getFeatureName
    for (auto &rCap: lCapabilities) {
        std::cout << lGetCap.getFeatureName(rCap) << std::endl;
    }

    GetIntrin::Instructions::VAES

    std::cout << std::endl;

    // Do not use GetIntrin() in hot path. Extract what you need into hot path and use it there.
    if (std::find(lCapabilities.begin(), lCapabilities.end(), lGetCap.AVX2) != lCapabilities.end()) {
        // Set a variable to use in hot path
        std::cout << "This CPU has AVX2 support." << std::endl;
    } else {
        std::cout << "This CPU does not have AVX2 support." << std::endl;
    }

    return EXIT_SUCCESS;
}
