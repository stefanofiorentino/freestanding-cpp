// Host-side test for the freestanding core.
//
// This binary runs on the OS, so it is compiled with the normal host flags
// and may use the full C++23 standard library. It only exercises the
// freestanding core (src/freestanding_core.hpp) by linking against it,
// verifying that the freestanding library compiles, links and behaves
// correctly.

#include <cstdlib>

#include "freestanding_core.hpp"
#include "test_utils.hpp"

int main() {
    test::Reporter r;

    r.check(core::add(2, 3) == 5, "core::add(2, 3) == 5");
    r.check(core::diff(10, 4) == 6, "core::diff(10, 4) == 6");
    r.check(core::factorial(6) == 720ULL, "core::factorial(6) == 720");

    return r.finish();
}
