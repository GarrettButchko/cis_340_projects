#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEST_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
REPO_ROOT="$(cd "$TEST_ROOT/.." && pwd)"

make -C "$REPO_ROOT/src" february5

bash "$SCRIPT_DIR/smoke.sh" tier2
