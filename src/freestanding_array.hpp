// Freestanding container example: std::array.
//
// Compiled pure freestanding (`-ffreestanding -fno-exceptions -fno-rtti`), so
// this header may rely ONLY on the C++23 freestanding subset of the standard
// library. It demonstrates that the freestanding container <array> is fully
// usable in the core: fixed-size, no heap, no exceptions.
//
// <array> is part of the freestanding subset per P1642/P2213R1, guarded by the
// feature-test macro __cpp_lib_freestanding_array. Testing happens in test/ on
// the host OS.

#pragma once

#include <array>
#include <cstddef>

namespace core {
namespace array {

// Rotate a fixed-size std::array left by one. Fully constexpr and noexcept, so
// it works both at compile time (static_asserts below) and on bare metal.
template <typename T, std::size_t N>
[[nodiscard]] constexpr std::array<T, N> rotate_left(std::array<T, N> a) noexcept {
    if constexpr (N > 1) {
        const T first = a[0];
        for (std::size_t i = 1; i < N; ++i) {
            a[i - 1] = a[i];
        }
        a[N - 1] = first;
    }
    return a;
}

// Compile-time assertions: the freestanding container must be verifiable in
// the core itself, with no host runtime involved.
static_assert(rotate_left(std::array<int, 3>{1, 2, 3}) == std::array<int, 3>{2, 3, 1});
static_assert(rotate_left(std::array<int, 1>{7}) == std::array<int, 1>{7});
static_assert(rotate_left(std::array<char, 2>{'a', 'b'}) == std::array<char, 2>{'b', 'a'});

} // namespace array
} // namespace core
