#include <iostream>
#include <algorithm>
#include "getintrin.h"

int main() {

    std::cout << "Get CPU intrinsics capabilities" << std::endl << std::endl;

    // Create the GetIntrin object
    auto lIntrinCap = GetIntrin();

    // Returns a struct containing this CPU's information and capabilities
    auto lCPUInfo = lIntrinCap.getCapabilities();

    std::cout << "Vendor: " << lCPUInfo.mVendor << std::endl;
    std::cout << "Stepping: " << std::to_string(lCPUInfo.mStepping) << std::endl;
    std::cout << "Model: " << std::to_string(lCPUInfo.mModel) << std::endl;
    std::cout << "Family: " << std::to_string(lCPUInfo.mFamilyID) << std::endl;
    std::cout << "Type: " << std::to_string(lCPUInfo.mProcessorType) << std::endl;
    std::cout << "Extended Model: " << std::to_string(lCPUInfo.mExtendedModelID) << std::endl;
    std::cout << "Extended Family: " << std::to_string(lCPUInfo.mExtendedFamilyID) << std::endl;

    std::cout << std::endl << "Capabilities: " << std::endl;

    // Helper method for getting friendly names -> getFeatureName
    for (auto &rCap: lCPUInfo.mCapabilities) {
        std::cout << lIntrinCap.getFeatureName(rCap) << std::endl;
    }

    // Do not use hasFeature in hot path. Extract what you need then use simpler logic in hot path.
    if (lIntrinCap.hasFeature(GetIntrin::Instructions::AVX2)) {
        std::cout << std::endl << "This CPU has AVX2 support." << std::endl;
    } else {
        std::cout << std::endl << "This CPU does not have AVX2 support." << std::endl;
    }

    return EXIT_SUCCESS;
}
