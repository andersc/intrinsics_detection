//
// Created by Anders Cedronius on 2023-05-15.
//

#pragma once

#include <iostream>
#include <vector>
#include <bitset>

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

    std::vector<Instructions> getCapabilities() {
        uint32_t eax, ebx, ecx, edx;
        uint32_t maxCapabilities = 0;
        uint32_t maxExtendedCapabilities = 0;
        uint32_t extendedLeaf = 0;

        // Feature information
        eax = 0;
        ecx = 0;
        cpuid(&eax, &ebx, &ecx, &edx);
        maxCapabilities = eax;

        // Extended information
        eax = 0x80000000;
        ecx = 0;
        cpuid(&eax, &ebx, &ecx, &edx);
        maxExtendedCapabilities = 0x7fffffff & eax;

        if (maxExtendedCapabilities >= 1) {
            eax = 0x80000001;
            ecx = 0;
            cpuid(&eax, &ebx, &ecx, &edx);
            if (edx & (1 << 24)) mInstructions.emplace_back(Instructions::FXSR);
            if (ecx & (1 << 5)) mInstructions.emplace_back(Instructions::LZCNT);
            if (ecx & (1 << 8)) mInstructions.emplace_back(Instructions::PRFCHW);
            if (edx & (1 << 27)) mInstructions.emplace_back(Instructions::RDTSCP);
            if (edx & (1 << 4)) mInstructions.emplace_back(Instructions::TSC);
        }

        if (maxExtendedCapabilities >= 8) {
            eax = 0x80000008;
            ecx = 0;
            cpuid(&eax, &ebx, &ecx, &edx);
            if (ebx & (1 << 9)) mInstructions.emplace_back(Instructions::WBNOINVD);
        }

        // Feature information
        eax = 1;
        ecx = 0;
        cpuid(&eax, &ebx, &ecx, &edx);
        if (edx & (1 << 23)) mInstructions.emplace_back(Instructions::MMX);
        if (edx & (1 << 25)) mInstructions.emplace_back(Instructions::SSE);
        if (edx & (1 << 25)) mInstructions.emplace_back(Instructions::SSE2);
        if (ecx & (1 << 0)) mInstructions.emplace_back(Instructions::SSE3);
        if (ecx & (1 << 9)) mInstructions.emplace_back(Instructions::SSSE3);
        if (ecx & (1 << 19)) mInstructions.emplace_back(Instructions::SSE41);
        if (ecx & (1 << 20)) {
            mInstructions.emplace_back(Instructions::SSE42);
            mInstructions.emplace_back(Instructions::CRC32);
        }
        if (ecx & (1 << 28)) mInstructions.emplace_back(Instructions::AVX);
        if (ecx & (1 << 29)) mInstructions.emplace_back(Instructions::F16C);
        if (ecx & (1 << 12)) mInstructions.emplace_back(Instructions::FMA);
        if (ecx & (1 << 25)) mInstructions.emplace_back(Instructions::AES);
        if (ecx & (1 << 3)) mInstructions.emplace_back(Instructions::MONITOR);
        if (ecx & (1 << 22)) mInstructions.emplace_back(Instructions::MOVBE);
        if (ecx & (1 << 1)) mInstructions.emplace_back(Instructions::PCLMULQDQ);
        if (ecx & (1 << 23)) mInstructions.emplace_back(Instructions::POPCNT);
        if (ecx & (1 << 30)) mInstructions.emplace_back(Instructions::RDRAND);
        if (ecx & (1 << 26)) mInstructions.emplace_back(Instructions::XSAVE);

        if (maxCapabilities <= 7) {
            return mInstructions;
        }

        // Extended feature information
        eax = 0x7;
        ecx = 0x0;
        cpuid(&eax, &ebx, &ecx, &edx);
        extendedLeaf = eax;

        if (ebx & (1 << 5)) mInstructions.emplace_back(Instructions::AVX2);
        if (ebx & (1 << 16)) mInstructions.emplace_back(Instructions::AVX_512F);
        if (ebx & (1 << 30)) mInstructions.emplace_back(Instructions::AVX_512BW);
        if (ebx & (1 << 28)) mInstructions.emplace_back(Instructions::AVX_512CD);
        if (ebx & (1 << 17)) mInstructions.emplace_back(Instructions::AVX_512DQ);
        if (ebx & (1 << 27)) mInstructions.emplace_back(Instructions::AVX_512ER);
        if (ebx & (1 << 21)) mInstructions.emplace_back(Instructions::AVX_512IFMA52);
        if (ebx & (1 << 26)) mInstructions.emplace_back(Instructions::AVX_512PF);
        if (ebx & (1 << 31)) mInstructions.emplace_back(Instructions::AVX_512VL);
        if (ecx & (1 << 14)) mInstructions.emplace_back(Instructions::AVX_512VPOPCNTDQ);
        if (edx & (1 << 3)) mInstructions.emplace_back(Instructions::AVX_512_4FMAPS);
        if (edx & (1 << 2)) mInstructions.emplace_back(Instructions::AVX_512_4VNNIW);
        if (ecx & (1 << 12)) mInstructions.emplace_back(Instructions::AVX_512_BITALG);
        if (ecx & (1 << 1)) mInstructions.emplace_back(Instructions::AVX_512_VBMI);
        if (ecx & (1 << 6)) mInstructions.emplace_back(Instructions::AVX_512_VBMI2);
        if (ecx & (1 << 11)) mInstructions.emplace_back(Instructions::AVX_512_VNNI);
        if (edx & (1 << 8)) mInstructions.emplace_back(Instructions::AVX_512_VP2INTERSECT);
        if (edx & (1 << 23)) mInstructions.emplace_back(Instructions::AVX_512_FP16);
        if (ebx & (1 << 19)) mInstructions.emplace_back(Instructions::ADX);
        if (ebx & (1 << 3)) mInstructions.emplace_back(Instructions::BMI1);
        if (ebx & (1 << 8)) mInstructions.emplace_back(Instructions::BMI2);
        if (ecx & (1 << 7)) mInstructions.emplace_back(Instructions::CET_SS);
        if (ecx & (1 << 25)) mInstructions.emplace_back(Instructions::CLDEMOTE);
        if (ebx & (1 << 23)) mInstructions.emplace_back(Instructions::CLFLUSHOPT);
        if (ebx & (1 << 24)) mInstructions.emplace_back(Instructions::CLWB);
        if (ecx & (1 << 29)) mInstructions.emplace_back(Instructions::ENQCMD);
        if (ebx & (1 << 0)) mInstructions.emplace_back(Instructions::FSGSBASE);
        if (ecx & (1 << 8)) mInstructions.emplace_back(Instructions::GFNI);
        if (ebx & (1 << 10)) mInstructions.emplace_back(Instructions::INVPCID);
        if (ecx & (1 << 28)) mInstructions.emplace_back(Instructions::MOVDIR64B);
        if (ecx & (1 << 27)) mInstructions.emplace_back(Instructions::MOVDIRI);
        if (ebx & (1 << 14)) mInstructions.emplace_back(Instructions::MPX);
        if (edx & (1 << 18)) mInstructions.emplace_back(Instructions::PCONFIG);
        if (ecx & (1 << 0)) mInstructions.emplace_back(Instructions::PREFETCHWT1);
        if (ecx & (1 << 22)) mInstructions.emplace_back(Instructions::RDPID);
        if (ebx & (1 << 18)) mInstructions.emplace_back(Instructions::RDSEED);
        if (ebx & (1 << 11)) mInstructions.emplace_back(Instructions::RTM);
        if (edx & (1 << 14)) mInstructions.emplace_back(Instructions::SERIALIZE);
        if (ebx & (1 << 29)) mInstructions.emplace_back(Instructions::SHA);
        if (edx & (1 << 16)) mInstructions.emplace_back(Instructions::TSXLDTRK);
        if (edx & (1 << 5)) mInstructions.emplace_back(Instructions::UINTR);
        if (ecx & (1 << 9)) mInstructions.emplace_back(Instructions::VAES);
        if (ecx & (1 << 10)) mInstructions.emplace_back(Instructions::VPCLMULQDQ);
        if (ecx & (1 << 5)) mInstructions.emplace_back(Instructions::WAITPKG);

        if (!extendedLeaf) {
            return mInstructions;
        }

        // Extended feature leaf information
        eax = 7;
        ecx = 1;
        cpuid(&eax, &ebx, &ecx, &edx);

        if (eax & (1 << 4)) mInstructions.emplace_back(Instructions::AVX_VNNI);
        if (edx & (1 << 4)) mInstructions.emplace_back(Instructions::AVX_VNNI_INT8);
        if (edx & (1 << 5)) mInstructions.emplace_back(Instructions::AVX_NE_CONVERT);
        if (edx & (1 << 23)) mInstructions.emplace_back(Instructions::AVX_IFMA);
        if (eax & (1 << 5)) mInstructions.emplace_back(Instructions::AVX_512_BF16);
        if (eax & (1 << 7)) mInstructions.emplace_back(Instructions::CMPCCXADD);
        if (eax & (1 << 22)) mInstructions.emplace_back(Instructions::HRESET);
        if (edx & (1 << 14)) mInstructions.emplace_back(Instructions::PREFETCHI);
        if (eax & (1 << 3)) mInstructions.emplace_back(Instructions::RAO_INT);

        return mInstructions;
    }

    std::string getFeatureName(Instructions feature) {
        auto lIndex = static_cast<int>(feature);
        if (lIndex >= InstructionsMax) {
            return "Unknown";
        }
        return mFeatureNames[lIndex];
    }


private:
    static inline void cpuid(uint32_t *eax, uint32_t *ebx, uint32_t *ecx,
               uint32_t *edx) {
#if defined(_MSC_VER)
        int lCupReg[4];
  __cpuidex(lCupReg, *eax, *ecx);
  *eax = lCupReg[0];
  *ebx = lCupReg[1];
  *ecx = lCupReg[2];
  *edx = lCupReg[3];
#else
        uint32_t lEAX = *eax, lEBX, lECX = *ecx, lEDX;
        asm volatile("cpuid\n\t" : "+a"(lEAX), "=b"(lEBX), "+c"(lECX), "=d"(lEDX));
        *eax = lEAX;
        *ebx = lEBX;
        *ecx = lECX;
        *edx = lEDX;
#endif
    }

    std::vector<Instructions> mInstructions;
    std::string mFeatureNames[InstructionsMax] = {
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