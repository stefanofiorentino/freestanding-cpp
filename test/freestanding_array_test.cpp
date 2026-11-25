// Host-side test for the freestanding std::array example.
//
// This binary runs on the OS, so it is compiled with the normal host flags
// and may use the full C++23 standard library. It only exercises the header
// src/freestanding_array.hpp, verifying that the freestanding <array> example
// compiles, links and behaves correctly at runtime.

#include <array>
#include <cstdlib>

#include "freestanding_array.hpp"
#include "test_utils.hpp"

int main() {
    test::Reporter r;

    r.check(core::array::rotate_left(std::array<int, 4>{1, 2, 3, 4}) ==
                std::array<int, 4>{2, 3, 4, 1},
            "core::array::rotate_left({1,2,3,4}) == {2,3,4,1}");
    r.check(core::array::rotate_left(std::array<int, 1>{9}) == std::array<int, 1>{9},
            "core::array::rotate_left single-element array is a no-op");
    r.check(core::array::rotate_left(std::array<int, 2>{7, 2}) == std::array<int, 2>{2, 7},
            "core::array::rotate_left two-element {7,2} == {2,7}");

    return r.finish();
}
