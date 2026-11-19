// Host-side test for the freestanding core.
//
// This binary runs on the OS, so it is compiled with the normal host flags
// and may use the full C++23 standard library. It only exercises the
// freestanding core (src/freestanding_core.hpp) by linking against it,
// verifying that the freestanding library compiles, links and behaves
// correctly.

#include <cstdlib>
#include <iostream>

#include "freestanding_core.hpp"

namespace {

int failures = 0;

void check(bool ok, const char* what) {
    if (ok) {
        std::cout << "[PASS] " << what << '\n';
    } else {
        std::cout << "[FAIL] " << what << '\n';
        ++failures;
    }
}

} // namespace

int main() {
    check(core::add(2, 3) == 5, "core::add(2, 3) == 5");
    check(core::diff(10, 4) == 6, "core::diff(10, 4) == 6");
    check(core::factorial(6) == 720ULL, "core::factorial(6) == 720");

    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return EXIT_SUCCESS;
    }
    std::cout << failures << " test(s) failed.\n";
    return EXIT_FAILURE;
}
