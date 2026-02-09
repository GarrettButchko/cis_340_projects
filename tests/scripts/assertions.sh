#!/usr/bin/env bash
set -u

TESTS_TOTAL=0
TESTS_FAILED=0

pass() {
    local context="$1"
    printf '[PASS] %s\n' "$context"
}

fail() {
    local context="$1"
    local detail="$2"
    TESTS_FAILED=$((TESTS_FAILED + 1))
    printf '[FAIL] %s\n' "$context"
    if [[ -n "$detail" ]]; then
        printf '       %s\n' "$detail"
    fi
}

assert_contains() {
    local haystack="$1"
    local needle="$2"
    local context="$3"

    TESTS_TOTAL=$((TESTS_TOTAL + 1))
    if grep -Fq -- "$needle" <<<"$haystack"; then
        pass "$context"
    else
        fail "$context" "expected to find: $needle"
    fi
}

assert_equals() {
    local actual="$1"
    local expected="$2"
    local context="$3"

    TESTS_TOTAL=$((TESTS_TOTAL + 1))
    if [[ "$actual" == "$expected" ]]; then
        pass "$context"
    else
        fail "$context" "expected '$expected' but got '$actual'"
    fi
}

assert_exit_code() {
    local actual="$1"
    local expected="$2"
    local context="$3"

    TESTS_TOTAL=$((TESTS_TOTAL + 1))
    if [[ "$actual" -eq "$expected" ]]; then
        pass "$context"
    else
        fail "$context" "expected exit code $expected but got $actual"
    fi
}

assert_file_exists() {
    local path="$1"
    local context="$2"

    TESTS_TOTAL=$((TESTS_TOTAL + 1))
    if [[ -f "$path" ]]; then
        pass "$context"
    else
        fail "$context" "file not found: $path"
    fi
}

assert_file_not_exists() {
    local path="$1"
    local context="$2"

    TESTS_TOTAL=$((TESTS_TOTAL + 1))
    if [[ ! -e "$path" ]]; then
        pass "$context"
    else
        fail "$context" "unexpected path exists: $path"
    fi
}

assert_file_line_count() {
    local path="$1"
    local expected_count="$2"
    local context="$3"

    TESTS_TOTAL=$((TESTS_TOTAL + 1))
    if [[ ! -f "$path" ]]; then
        fail "$context" "file not found: $path"
        return
    fi

    local actual_count
    actual_count=$(wc -l < "$path")
    if [[ "$actual_count" == "$expected_count" ]]; then
        pass "$context"
    else
        fail "$context" "expected $expected_count lines but got $actual_count"
    fi
}

assert_file_equals() {
    local actual_path="$1"
    local expected_path="$2"
    local context="$3"

    TESTS_TOTAL=$((TESTS_TOTAL + 1))
    if [[ ! -f "$actual_path" ]]; then
        fail "$context" "actual file not found: $actual_path"
        return
    fi
    if [[ ! -f "$expected_path" ]]; then
        fail "$context" "expected file not found: $expected_path"
        return
    fi

    if cmp -s "$actual_path" "$expected_path"; then
        pass "$context"
    else
        fail "$context" "file content mismatch"
        diff -u "$expected_path" "$actual_path" || true
    fi
}

finish_assertions() {
    local passed
    passed=$((TESTS_TOTAL - TESTS_FAILED))
    printf '\nAssertions: %d passed, %d failed, %d total.\n' "$passed" "$TESTS_FAILED" "$TESTS_TOTAL"

    if [[ "$TESTS_FAILED" -ne 0 ]]; then
        return 1
    fi
    return 0
}
