
# CLAUDE.md

Behavioral guidelines to reduce common LLM coding mistakes. Merge with project-specific instructions as needed.

## Project: Freestanding C++ (src/, test/)

This project explores **C++23 freestanding** (standard library subset defined by
P1642). Key rules:

- **Standard:** C++23 everywhere. Never lower it — the freestanding feature set
  only exists in C++23.
- **`src/` — freestanding-only core.** Compiled pure freestanding with
  `-ffreestanding -fno-exceptions -fno-rtti` (see `CMakeLists.txt`). It may use
  ONLY the C++23 freestanding subset of the standard library (facilities guarded
  by `__cpp_lib_freestanding_feature_set` / `__cpp_lib_freestanding_*`). No
  console output, no hosted-only standard library, no exceptions, no RTTI. This
  is code that could run on bare metal.
- **`test/` — host-run tests.** The test binary runs on the OS, is compiled
  with normal host flags, and may use the full C++23 standard library. It links
  the freestanding core to verify it compiles, links and behaves correctly.
- **Toolchain: prefer clang.** Use the **clang** toolchain (`clang++`,
  clang-format, clang-tidy). CMake picks `clang++` when available; `src/` and
  `test/` are formatted/checked with clang-format and clang-tidy.
- **Formatting & static analysis** (config in `.clang-format` / `.clang-tidy`):
  ```bash
  cmake --build build --target format   # clang-format -i on sources
  cmake --build build --target tidy     # clang-tidy on sources
  ```
  Note: on macOS the LLVM tools are often keg-only, so CMake looks for
  `clang-format`/`clang-tidy` under `/opt/homebrew/opt/llvm/bin` too. If they're
  not found, those targets are skipped with a warning.
- **Do NOT use `-nostdinc`/`-nostdlib`.** Since C++23 the freestanding subset is
  provided by the (host) standard library headers; rely on the freestanding
  feature macros instead of hiding the whole standard library.
- **Build & test:**
  ```bash
  cmake -S . -B build
  cmake --build build
  ctest --test-dir build --output-on-failure
  ```
- **When adding a feature:** add the freestanding implementation to `src/`
  first, then a host test in `test/` that exercises it.

There is intentionally no runtime/stdlib in the core: the project keeps the
library dependency-free and simple (no GoogleTest, etc.) unless we later choose
to add it solely to `test/`.

**Tradeoff:** These guidelines bias toward caution over speed. For trivial tasks, use judgment.

## 1. Think Before Coding

**Don't assume. Don't hide confusion. Surface tradeoffs.**

Before implementing:
- State your assumptions explicitly. If uncertain, ask.
- If multiple interpretations exist, present them - don't pick silently.
- If a simpler approach exists, say so. Push back when warranted.
- If something is unclear, stop. Name what's confusing. Ask.

## 2. Simplicity First

**Minimum code that solves the problem. Nothing speculative.**

- No features beyond what was asked.
- No abstractions for single-use code.
- No "flexibility" or "configurability" that wasn't requested.
- No error handling for impossible scenarios.
- If you write 200 lines and it could be 50, rewrite it.

Ask yourself: "Would a senior engineer say this is overcomplicated?" If yes, simplify.

## 3. Surgical Changes

**Touch only what you must. Clean up only your own mess.**

When editing existing code:
- Don't "improve" adjacent code, comments, or formatting.
- Don't refactor things that aren't broken.
- Match existing style, even if you'd do it differently.
- If you notice unrelated dead code, mention it - don't delete it.

When your changes create orphans:
- Remove imports/variables/functions that YOUR changes made unused.
- Don't remove pre-existing dead code unless asked.

The test: Every changed line should trace directly to the user's request.

## 4. Goal-Driven Execution

**Define success criteria. Loop until verified.**

Transform tasks into verifiable goals:
- "Add validation" → "Write tests for invalid inputs, then make them pass"
- "Fix the bug" → "Write a test that reproduces it, then make it pass"
- "Refactor X" → "Ensure tests pass before and after"

For multi-step tasks, state a brief plan:
```
1. [Step] → verify: [check]
2. [Step] → verify: [check]
3. [Step] → verify: [check]
```

Strong success criteria let you loop independently. Weak criteria ("make it work") require constant clarification.

---

**These guidelines are working if:** fewer unnecessary changes in diffs, fewer rewrites due to overcomplication, and clarifying questions come before implementation rather than after mistakes.
