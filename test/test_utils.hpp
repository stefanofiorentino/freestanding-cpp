// Shared minimal test harness for host-run test binaries.
//
// Host-side only: uses <iostream>/<cstdlib>, so it must never be included from
// src/ (which is compiled pure freestanding). Both test binaries include this
// header to get the same PASS/FAIL reporting and exit-code bookkeeping.
//
// Header-only by design: it is small, and header guards make it safe to include
// from multiple translation units without a .cpp or link step.

#pragma once

#include <cstdlib>
#include <iostream>

namespace test {

// Tracks check results and turns them into a process exit code. Usage:
//
//   test::Reporter r;
//   r.check(cond, "description");
//   return r.finish();   // EXIT_SUCCESS if all passed, EXIT_FAILURE otherwise
class Reporter {
  public:
    // Report a single assertion. Prints [PASS]/[FAIL]; a failed check is
    // counted so finish() can decide the exit code.
    void check(bool ok, const char* what) {
        if (ok) {
            std::cout << "[PASS] " << what << '\n';
        } else {
            std::cout << "[FAIL] " << what << '\n';
            ++failures_;
        }
    }

    // Print the summary and return the exit code for main().
    [[nodiscard]] int finish() const {
        if (failures_ == 0) {
            std::cout << "All tests passed.\n";
            return EXIT_SUCCESS;
        }
        std::cout << failures_ << " test(s) failed.\n";
        return EXIT_FAILURE;
    }

    // Number of failed checks so far (useful for early-exit or diagnostics).
    [[nodiscard]] int failures() const { return failures_; }

  private:
    int failures_ = 0;
};

} // namespace test
