#include <cstring>
#include <optional>
#include <vector>
#include "const.h"

namespace beagle {
namespace gpio {

std::optional<pins_t> getPin(const char* key) noexcept {
    for (auto pin: pinTable) {
        if (strcmp(pin.key, key) == 0 || strcmp(pin.name, key) == 0) {
            return pin;
        }
    }

    return std::nullopt;
}

std::optional<pins_t> getPin(unsigned int pinID) noexcept {
    for (auto pin: pinTable) {
        if (pin.gpio == pinID) {
            return pin;
        }
    }

    return std::nullopt;
}

std::optional<int> getEdgeIndex(const char* value) noexcept {
    for (int i = 0; i < static_cast<int>(sizeof(edge)); i++) {
        if (strcmp(edge[i], value) == 0)
            return i;
    }

    return std::nullopt;
}

std::vector<pins_t> findMode(const char* mode) noexcept {
    std::vector<pins_t> out;

    for (auto pin: pinTable) {
        if (strcmp(pin.modes.mode0, mode) == 0) {
            out.push_back(pin); continue;
        }

        if (strcmp(pin.modes.mode1, mode) == 0) {
            out.push_back(pin); continue;
        }

        if (strcmp(pin.modes.mode2, mode) == 0) {
            out.push_back(pin); continue;
        }

        if (strcmp(pin.modes.mode3, mode) == 0) {
            out.push_back(pin); continue;
        }

        if (strcmp(pin.modes.mode4, mode) == 0) {
            out.push_back(pin); continue;
        }

        if (strcmp(pin.modes.mode5, mode) == 0) {
            out.push_back(pin); continue;
        }

        if (strcmp(pin.modes.mode6, mode) == 0) {
            out.push_back(pin); continue;
        }

        if (strcmp(pin.modes.mode7, mode) == 0) {
            out.push_back(pin); continue;
        }
    }

    return out;
}

}}
