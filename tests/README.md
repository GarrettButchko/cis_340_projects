# Test Suite Guide

This project uses a two-tier test strategy:
- Tier 1: must-run regression tests for core behavior.
- Tier 2: stress and rare edge-case tests.

The scripted harness is shell-based and runs the compiled `src/february5` binary in isolated temp directories.

## Quick Start

From repository root:

```bash
bash tests/scripts/run_tier1.sh
bash tests/scripts/run_tier2.sh
```

Or run both suites in one pass:

```bash
make -C src february5
bash tests/scripts/smoke.sh all
```

## Manual Matrix

Use `tests/manual_test_matrix.md` when running tests interactively in a terminal or during group reviews.

Recommended manual workflow:
1. `cd src`
2. `make clean && make february5`
3. Run the cases listed in the matrix and record outcomes.

## Fixture Files

Fixtures live in `tests/data/`:
- `valid_cards.txt`
- `duplicate_names.txt`
- `mixed_malformed.txt`
- `malformed_extra_delimiter.txt`

## Pass/Fail Behavior

The scripts print `[PASS]` / `[FAIL]` lines per assertion and end with an assertion summary.
The script exits non-zero if any assertion fails.
