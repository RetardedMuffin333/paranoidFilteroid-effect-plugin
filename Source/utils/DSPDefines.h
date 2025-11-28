#pragma once

#include <cmath>

// DSP constants
namespace DSP {
    // Telephone mode specs (per DSP_SPEC.md)
    constexpr float TELEPHONE_LOW_CUT_HZ = 300.0f;
    constexpr float TELEPHONE_HIGH_CUT_HZ = 3400.0f;

    // Radio mode specs (per DSP_SPEC.md)
    constexpr float RADIO_LOW_CUT_HZ = 200.0f;
    constexpr float RADIO_HIGH_CUT_HZ = 5000.0f;

    // Mode enum
    enum class Mode {
        Telephone = 0,
        Radio = 1,
        Custom = 2
    };
}
