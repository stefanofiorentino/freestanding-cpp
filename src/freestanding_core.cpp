// Freestanding-only core library implementation.
//
// Compiled with -ffreestanding -fno-exceptions -fno-rtti (see CMakeLists.txt),
// so this translation unit may use only the C++23 freestanding subset of the
// standard library. See freestanding_core.hpp for the constraint rationale.

#include "freestanding_core.hpp"

namespace core {

int diff(int a, int b) noexcept { return a - b; }

} // namespace core
