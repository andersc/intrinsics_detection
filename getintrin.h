//
// Created by Anders Cedronius on 2023-05-15.
//

#pragma once

#include <iostream>
#include <vector>
#include <bitset>
#include <string>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

#if __x86_64__ || _M_X64
#else
#error Architecture not supported
#endif

class GetIntrin {
public:
    enum Instructions : uint32_t {
        MMX,    // Section MMX
        SSE,    // Section SSE
        SSE2,
        SSE3,
        SSSE3,
        SSE41,
        SSE42,
        AVX,    // Section AVX
        F16C,
        FMA,
        AVX2,
        AVX_VNNI,
        AVX_VNNI_INT8,
        AVX_NE_CONVERT,
        AVX_IFMA,
        AVX_512F, // Section AVX-512
        AVX_512BW,
        AVX_512CD,
        AVX_512DQ,
        AVX_512ER,
        AVX_512IFMA52,
        AVX_512PF,
        AVX_512VL,
        AVX_512VPOPCNTDQ,
        AVX_512_4FMAPS,
        AVX_512_4VNNIW,
        AVX_512_BF16,
        AVX_512_BITALG,
        AVX_512_VBMI,
        AVX_512_VBMI2,
        AVX_512_VNNI,
        AVX_512_VP2INTERSECT,
        AVX_512_FP16,
        AMX_BF16,   // Section AMX
        AMX_INT8,
        AMX_TILE,
        AMX_FP16,
        AMX_COMPLEX,
        ADX,        // Section Other
        AES,
        BMI1,
        BMI2,
        CET_SS,
        CLDEMOTE,
        CLFLUSHOPT,
        CLWB,
        CMPCCXADD,
        CRC32,
        ENQCMD,
        FSGSBASE,
        FXSR,
        GFNI,
        HRESET,
        INVPCID,
        KEYLOCKER,              // Not implemented
        KEYLOCKER_WIDE,         // Not implemented
        LZCNT,
        MONITOR,
        MOVBE,
        MOVDIR64B,
        MOVDIRI,
        MPX,
        PCLMULQDQ,
        PCONFIG,
        POPCNT,
        PREFETCHI,
        PREFETCHWT1,
        PRFCHW,
        RAO_INT,
        RDPID,
        RDRAND,
        RDSEED,
        RDTSCP,
        RTM,
        SERIALIZE,
        SHA,
        TSC,
        TSXLDTRK,
        UINTR,
        VAES,
        VPCLMULQDQ,
        WAITPKG,
        WBNOINVD,
        XSAVE,
        XSAVEC, // Not implemented
        XSAVEOPT, // Not implemented
        XSS, // Not implemented
        InstructionsMax
    };

    struct CPUInfo {
        std::string mVendor;
        std::string mName;
        uint8_t mStepping = 0;
        uint8_t mModel = 0;
        uint8_t mFamilyID = 0;
        uint8_t mProcessorType = 0;
        uint8_t mExtendedModelID = 0;
        uint8_t mExtendedFamilyID = 0;
        std::vector<Instructions> mCapabilities;
    };

    CPUInfo getCapabilities() {
        mCPUInfo = CPUInfo();
        uint32_t eax, ebx, ecx, edx;
        uint32_t maxCapabilities = 0;
        uint32_t maxExtendedCapabilities = 0;
        uint32_t extendedLeaf = 0;

        // Is extended feature information available?
        eax = 0;
        ecx = 0;
        cpuid(&eax, &ebx, &ecx, &edx);
        maxCapabilities = eax;
        mCPUInfo.mVendor = std::string((const char *) &ebx, 4) + std::string((const char *) &edx, 4) +
                           std::string((const char *) &ecx, 4);

        // Get extended information size
        eax = 0x80000000;
        ecx = 0;
        cpuid(&eax, &ebx, &ecx, &edx);
        maxExtendedCapabilities = 0x7fffffff & eax;

        if (maxExtendedCapabilities >= 4) {
            eax = 0x80000002;
            ecx = 0;
            cpuid(&eax, &ebx, &ecx, &edx);
            mCPUInfo.mName = std::string((const char *) &eax, 4) + std::string((const char *) &ebx, 4) +
                             std::string((const char *) &ecx, 4) + std::string((const char *) &edx, 4);
            eax = 0x80000003;
            ecx = 0;
            cpuid(&eax, &ebx, &ecx, &edx);
            mCPUInfo.mName += std::string((const char *) &eax, 4) + std::string((const char *) &ebx, 4) +
                              std::string((const char *) &ecx, 4) + std::string((const char *) &edx, 4);
            eax = 0x80000004;
            ecx = 0;
            cpuid(&eax, &ebx, &ecx, &edx);
            mCPUInfo.mName += std::string((const char *) &eax, 4) + std::string((const char *) &ebx, 4) +
                              std::string((const char *) &ecx, 4) + std::string((const char *) &edx, 4);
        }

        if (maxExtendedCapabilities >= 1) {
            // Get extended information
            eax = 0x80000001;
            ecx = 0;
            cpuid(&eax, &ebx, &ecx, &edx);
            if (ecx & (1 << 5)) mCPUInfo.mCapabilities.emplace_back(Instructions::LZCNT);
            if (ecx & (1 << 8)) mCPUInfo.mCapabilities.emplace_back(Instructions::PRFCHW);

            if (edx & (1 << 4)) mCPUInfo.mCapabilities.emplace_back(Instructions::TSC);
            if (edx & (1 << 24)) mCPUInfo.mCapabilities.emplace_back(Instructions::FXSR);
            if (edx & (1 << 27)) mCPUInfo.mCapabilities.emplace_back(Instructions::RDTSCP);
        }

        if (maxExtendedCapabilities >= 8) {
            // Get extended information
            eax = 0x80000008;
            ecx = 0;
            cpuid(&eax, &ebx, &ecx, &edx);
            if (ebx & (1 << 9)) mCPUInfo.mCapabilities.emplace_back(Instructions::WBNOINVD);
        }

        // Feature information
        eax = 1;
        ecx = 0;
        cpuid(&eax, &ebx, &ecx, &edx);

        if (ecx & (1 << 0)) mCPUInfo.mCapabilities.emplace_back(Instructions::SSE3);
        if (ecx & (1 << 1)) mCPUInfo.mCapabilities.emplace_back(Instructions::PCLMULQDQ);
        if (ecx & (1 << 3)) mCPUInfo.mCapabilities.emplace_back(Instructions::MONITOR);
        if (ecx & (1 << 9)) mCPUInfo.mCapabilities.emplace_back(Instructions::SSSE3);
        if (ecx & (1 << 12)) mCPUInfo.mCapabilities.emplace_back(Instructions::FMA);
        if (ecx & (1 << 19)) mCPUInfo.mCapabilities.emplace_back(Instructions::SSE41);
        if (ecx & (1 << 20)) {
            mCPUInfo.mCapabilities.emplace_back(Instructions::SSE42);
            mCPUInfo.mCapabilities.emplace_back(Instructions::CRC32);
        }
        if (ecx & (1 << 22)) mCPUInfo.mCapabilities.emplace_back(Instructions::MOVBE);
        if (ecx & (1 << 23)) mCPUInfo.mCapabilities.emplace_back(Instructions::POPCNT);
        if (ecx & (1 << 25)) mCPUInfo.mCapabilities.emplace_back(Instructions::AES);
        if (ecx & (1 << 26)) mCPUInfo.mCapabilities.emplace_back(Instructions::XSAVE);
        if (ecx & (1 << 28)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX);
        if (ecx & (1 << 29)) mCPUInfo.mCapabilities.emplace_back(Instructions::F16C);
        if (ecx & (1 << 30)) mCPUInfo.mCapabilities.emplace_back(Instructions::RDRAND);

        if (edx & (1 << 23)) mCPUInfo.mCapabilities.emplace_back(Instructions::MMX);
        if (edx & (1 << 25)) mCPUInfo.mCapabilities.emplace_back(Instructions::SSE);
        if (edx & (1 << 26)) mCPUInfo.mCapabilities.emplace_back(Instructions::SSE2);

        mCPUInfo.mStepping = eax & 0xF;
        mCPUInfo.mModel = (eax >> 4) & 0xF;
        mCPUInfo.mFamilyID = (eax >> 8) & 0xF;
        mCPUInfo.mProcessorType = (eax >> 12) & 0x3;
        mCPUInfo.mExtendedModelID = (eax >> 16) & 0xF;
        mCPUInfo.mExtendedFamilyID = (eax >> 20) & 0xFF;

        if (maxCapabilities <= 7) {
            return mCPUInfo;
        }

        // Extended feature information
        eax = 0x7;
        ecx = 0x0;
        cpuid(&eax, &ebx, &ecx, &edx);
        extendedLeaf = eax;

        if (ebx & (1 << 0)) mCPUInfo.mCapabilities.emplace_back(Instructions::FSGSBASE);
        if (ebx & (1 << 3)) mCPUInfo.mCapabilities.emplace_back(Instructions::BMI1);
        if (ebx & (1 << 5)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX2);
        if (ebx & (1 << 8)) mCPUInfo.mCapabilities.emplace_back(Instructions::BMI2);
        if (ebx & (1 << 10)) mCPUInfo.mCapabilities.emplace_back(Instructions::INVPCID);
        if (ebx & (1 << 11)) mCPUInfo.mCapabilities.emplace_back(Instructions::RTM);
        if (ebx & (1 << 14)) mCPUInfo.mCapabilities.emplace_back(Instructions::MPX);
        if (ebx & (1 << 16)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512F);
        if (ebx & (1 << 17)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512DQ);
        if (ebx & (1 << 18)) mCPUInfo.mCapabilities.emplace_back(Instructions::RDSEED);
        if (ebx & (1 << 19)) mCPUInfo.mCapabilities.emplace_back(Instructions::ADX);
        if (ebx & (1 << 21)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512IFMA52);
        if (ebx & (1 << 23)) mCPUInfo.mCapabilities.emplace_back(Instructions::CLFLUSHOPT);
        if (ebx & (1 << 24)) mCPUInfo.mCapabilities.emplace_back(Instructions::CLWB);
        if (ebx & (1 << 26)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512PF);
        if (ebx & (1 << 27)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512ER);
        if (ebx & (1 << 28)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512CD);
        if (ebx & (1 << 29)) mCPUInfo.mCapabilities.emplace_back(Instructions::SHA);
        if (ebx & (1 << 30)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512BW);
        if (ebx & (1 << 31)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512VL);

        if (ecx & (1 << 0)) mCPUInfo.mCapabilities.emplace_back(Instructions::PREFETCHWT1);
        if (ecx & (1 << 1)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512_VBMI);
        if (ecx & (1 << 5)) mCPUInfo.mCapabilities.emplace_back(Instructions::WAITPKG);
        if (ecx & (1 << 6)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512_VBMI2);
        if (ecx & (1 << 7)) mCPUInfo.mCapabilities.emplace_back(Instructions::CET_SS);
        if (ecx & (1 << 8)) mCPUInfo.mCapabilities.emplace_back(Instructions::GFNI);
        if (ecx & (1 << 9)) mCPUInfo.mCapabilities.emplace_back(Instructions::VAES);
        if (ecx & (1 << 10)) mCPUInfo.mCapabilities.emplace_back(Instructions::VPCLMULQDQ);
        if (ecx & (1 << 11)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512_VNNI);
        if (ecx & (1 << 12)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512_BITALG);
        if (ecx & (1 << 14)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512VPOPCNTDQ);
        if (ecx & (1 << 22)) mCPUInfo.mCapabilities.emplace_back(Instructions::RDPID);
        if (ecx & (1 << 25)) mCPUInfo.mCapabilities.emplace_back(Instructions::CLDEMOTE);
        if (ecx & (1 << 27)) mCPUInfo.mCapabilities.emplace_back(Instructions::MOVDIRI);
        if (ecx & (1 << 28)) mCPUInfo.mCapabilities.emplace_back(Instructions::MOVDIR64B);
        if (ecx & (1 << 29)) mCPUInfo.mCapabilities.emplace_back(Instructions::ENQCMD);

        if (edx & (1 << 2)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512_4VNNIW);
        if (edx & (1 << 3)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512_4FMAPS);
        if (edx & (1 << 5)) mCPUInfo.mCapabilities.emplace_back(Instructions::UINTR);
        if (edx & (1 << 8)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512_VP2INTERSECT);
        if (edx & (1 << 14)) mCPUInfo.mCapabilities.emplace_back(Instructions::SERIALIZE);
        if (edx & (1 << 16)) mCPUInfo.mCapabilities.emplace_back(Instructions::TSXLDTRK);
        if (edx & (1 << 18)) mCPUInfo.mCapabilities.emplace_back(Instructions::PCONFIG);
        if (edx & (1 << 22)) mCPUInfo.mCapabilities.emplace_back(Instructions::AMX_BF16);
        if (edx & (1 << 23)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512_FP16);
        if (edx & (1 << 24)) mCPUInfo.mCapabilities.emplace_back(Instructions::AMX_TILE);
        if (edx & (1 << 25)) mCPUInfo.mCapabilities.emplace_back(Instructions::AMX_INT8);

        if (extendedLeaf) {
            // Extended feature leaf information
            eax = 7;
            ecx = 1;
            cpuid(&eax, &ebx, &ecx, &edx);

            if (eax & (1 << 3)) mCPUInfo.mCapabilities.emplace_back(Instructions::RAO_INT);
            if (eax & (1 << 4)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_VNNI);
            if (eax & (1 << 5)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_512_BF16);
            if (eax & (1 << 7)) mCPUInfo.mCapabilities.emplace_back(Instructions::CMPCCXADD);
            if (eax & (1 << 21)) mCPUInfo.mCapabilities.emplace_back(Instructions::AMX_FP16);
            if (eax & (1 << 22)) mCPUInfo.mCapabilities.emplace_back(Instructions::HRESET);
            if (eax & (1 << 23)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_IFMA);

            if (edx & (1 << 4)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_VNNI_INT8);
            if (edx & (1 << 5)) mCPUInfo.mCapabilities.emplace_back(Instructions::AVX_NE_CONVERT);
            if (edx & (1 << 8)) mCPUInfo.mCapabilities.emplace_back(Instructions::AMX_COMPLEX);
            if (edx & (1 << 14)) mCPUInfo.mCapabilities.emplace_back(Instructions::PREFETCHI);
        }

        return mCPUInfo;
    }

    [[nodiscard]] bool hasFeature(Instructions feature) const {
        return std::find(mCPUInfo.mCapabilities.begin(), mCPUInfo.mCapabilities.end(), feature) !=
               mCPUInfo.mCapabilities.end();
    }

    std::string getFeatureName(Instructions feature) {
        auto lIndex = static_cast<uint32_t>(feature);
        if (lIndex >= InstructionsMax) {
            return "Unknown";
        }
        return mFeatureNames[lIndex];
    }


private:
    static inline void cpuid(uint32_t *pEAX, uint32_t *pEBX, uint32_t *pECX,
                             uint32_t *pEDX) {
#if defined(_MSC_VER)
        int lCupReg[4];
  __cpuidex(lCupReg, *pEAX, *pECX);
  *pEAX = lCupReg[0];
  *pEBX = lCupReg[1];
  *pECX = lCupReg[2];
  *pEDX = lCupReg[3];
#else
        uint32_t lEAX = *pEAX, lEBX, lECX = *pECX, lEDX;
        asm volatile("cpuid\n\t" : "+a"(lEAX), "=b"(lEBX), "+c"(lECX), "=d"(lEDX));
        *pEAX = lEAX;
        *pEBX = lEBX;
        *pECX = lECX;
        *pEDX = lEDX;
#endif
    }

    CPUInfo mCPUInfo;
    const std::string mFeatureNames[InstructionsMax] = {
            "MMX",
            "SSE",
            "SSE2",
            "SSE3",
            "SSSE3",
            "SSE41",
            "SSE42",
            "AVX",
            "F16C",
            "FMA",
            "AVX2",
            "AVX_VNNI",
            "AVX_VNNI_INT8",
            "AVX_NE_CONVERT",
            "AVX_IFMA",
            "AVX_512F",
            "AVX_512BW",
            "AVX_512CD",
            "AVX_512DQ",
            "AVX_512ER",
            "AVX_512IFMA52",
            "AVX_512PF",
            "AVX_512VL",
            "AVX_512VPOPCNTDQ",
            "AVX_512_4FMAPS",
            "AVX_512_4VNNIW",
            "AVX_512_BF16",
            "AVX_512_BITALG",
            "AVX_512_VBMI",
            "AVX_512_VBMI2",
            "AVX_512_VNNI",
            "AVX_512_VP2INTERSECT",
            "AVX_512_FP16",
            "AMX_BF16",
            "AMX_INT8",
            "AMX_TILE",
            "AMX_FP16",
            "AMX_COMPLEX",
            "ADX",
            "AES",
            "BMI1",
            "BMI2",
            "CET_SS",
            "CLDEMOTE",
            "CLFLUSHOPT",
            "CLWB",
            "CMPCCXADD",
            "CRC32",
            "ENQCMD",
            "FSGSBASE",
            "FXSR",
            "GFNI",
            "HRESET",
            "INVPCID",
            "KEYLOCKER",
            "KEYLOCKER_WIDE",
            "LZCNT",
            "MONITOR",
            "MOVBE",
            "MOVDIR64B",
            "MOVDIRI",
            "MPX",
            "PCLMULQDQ",
            "PCONFIG",
            "POPCNT",
            "PREFETCHI",
            "PREFETCHWT1",
            "PRFCHW",
            "RAO_INT",
            "RDPID",
            "RDRAND",
            "RDSEED",
            "RDTSCP",
            "RTM",
            "SERIALIZE",
            "SHA",
            "TSC",
            "TSXLDTRK",
            "UINTR",
            "VAES",
            "VPCLMULQDQ",
            "WAITPKG",
            "WBNOINVD",
            "XSAVE",
            "XSAVEC",
            "XSAVEOPT",
            "XSS"
    };
};
