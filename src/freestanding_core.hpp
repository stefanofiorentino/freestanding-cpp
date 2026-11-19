// Freestanding-only core library.
//
// This library is compiled pure freestanding (`-ffreestanding -fno-exceptions
// -fno-rtti`) and may therefore rely ONLY on the C++23 freestanding subset of
// the standard library (P1642, guarded by __cpp_lib_freestanding_feature_set
// and friends). No hosted-only standard library facilities may be used here.
//
// It cannot print to a terminal, allocate via the runtime, or throw: it is the
// code that could run on bare metal. Testing happens in test/ on the host OS.

#pragma once

#include <type_traits>

namespace core {

// Trivial math, usable in both constant and runtime contexts.
constexpr int add(int a, int b) noexcept { return a + b; }

// Difference helper, defined out-of-line in the .cpp to prove the
// freestanding translation unit compiles and links.
int diff(int a, int b) noexcept;

// Constexpr factorial: smoke test that the code is C++23-freestanding friendly
// and works at compile time.
constexpr unsigned long long factorial(unsigned n) noexcept {
    return n <= 1U ? 1ULL : static_cast<unsigned long long>(n) * factorial(n - 1U);
}

// Compile-time assertions: these must hold in the freestanding core itself.
static_assert(add(1, 2) == 3);
static_assert(factorial(5) == 120ULL);

// Proof we can use the freestanding part of <type_traits>.
static_assert(std::is_same_v<decltype(add(1, 2)), int>);

} // namespace core
