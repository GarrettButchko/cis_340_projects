#!/usr/bin/env bash
set -u
set -o pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEST_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
REPO_ROOT="$(cd "$TEST_ROOT/.." && pwd)"
SRC_DIR="$REPO_ROOT/src"
DATA_DIR="$TEST_ROOT/data"
BIN="$SRC_DIR/february5"

# shellcheck source=tests/scripts/assertions.sh
source "$SCRIPT_DIR/assertions.sh"

TEMP_PATHS=()

register_temp() {
    local path="$1"
    TEMP_PATHS+=("$path")
}

cleanup_temps() {
    local path
    for path in "${TEMP_PATHS[@]}"; do
        rm -rf "$path"
    done
}

trap cleanup_temps EXIT

create_temp_file_with_content() {
    local content="$1"
    local temp_file
    temp_file=$(mktemp)
    register_temp "$temp_file"
    printf '%s' "$content" > "$temp_file"
    printf '%s' "$temp_file"
}

RUN_WORKDIR=""
RUN_OUTPUT_FILE=""
RUN_OUTPUT=""
RUN_EXIT_CODE=0

run_program() {
    local case_id="$1"
    local fixture_path="$2"
    local input_payload="$3"
    local mode="${4:-normal}"

    local workdir
    local output_file
    local exit_code

    workdir=$(mktemp -d)
    output_file=$(mktemp)
    register_temp "$workdir"
    register_temp "$output_file"

    if [[ -n "$fixture_path" ]]; then
        cp "$fixture_path" "$workdir/cards.txt"
    fi

    if [[ "$mode" == "readonly_dir" ]]; then
        chmod 555 "$workdir"
    fi

    exit_code=0
    (
        cd "$workdir" || exit 98
        printf '%s' "$input_payload" | "$BIN"
    ) > "$output_file" 2>&1 || exit_code=$?

    if [[ "$mode" == "readonly_dir" ]]; then
        chmod 700 "$workdir"
    fi

    RUN_WORKDIR="$workdir"
    RUN_OUTPUT_FILE="$output_file"
    RUN_OUTPUT="$(cat "$output_file")"
    RUN_EXIT_CODE="$exit_code"

    printf '\n[CASE] %s\n' "$case_id"
}

case_t1_01_missing_file_start() {
    run_program "T1-01" "" $'6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T1-01 exits successfully"
    assert_contains "$RUN_OUTPUT" "No existing file at cards.txt. Starting with an empty list." "T1-01 missing-file startup message"
    assert_contains "$RUN_OUTPUT" "Exiting without saving." "T1-01 exit message"
    assert_file_not_exists "$RUN_WORKDIR/cards.txt" "T1-01 no file created without save"
}

case_t1_02_add_list_save_single() {
    local expected_file
    run_program "T1-02" "" $'2\nPikachu\nElectric\n35\n3.99\n1\n4\n6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T1-02 exits successfully"
    assert_contains "$RUN_OUTPUT" "Card added. Total cards: 1" "T1-02 add confirmation"
    assert_contains "$RUN_OUTPUT" '1) Name: Pikachu | Type: Electric | Hit Points: 35 | Dollar Value: $3.99' "T1-02 list formatting"
    assert_contains "$RUN_OUTPUT" "Saved 1 card(s) to cards.txt." "T1-02 save confirmation"
    expected_file=$(create_temp_file_with_content $'Pikachu|Electric|35|3.99\n')
    assert_file_equals "$RUN_WORKDIR/cards.txt" "$expected_file" "T1-02 file content"
}

case_t1_03_load_existing_cards() {
    run_program "T1-03" "$DATA_DIR/valid_cards.txt" $'1\n6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T1-03 exits successfully"
    assert_contains "$RUN_OUTPUT" "Loaded 2 card(s) from cards.txt." "T1-03 loaded count"
    assert_contains "$RUN_OUTPUT" "Name: Charizard | Type: Fire | Hit Points: 120 | Dollar Value: \$250.00" "T1-03 list includes second card"
}

case_t1_04_delete_first_exact_match() {
    local expected_file
    run_program "T1-04" "$DATA_DIR/duplicate_names.txt" $'3\nEevee\n4\n6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T1-04 exits successfully"
    assert_contains "$RUN_OUTPUT" 'Deleted first match for "Eevee". Remaining cards: 2' "T1-04 delete confirmation"
    expected_file=$(create_temp_file_with_content $'Eevee|Dark|65|7.50\nMew|Psychic|100|99.99\n')
    assert_file_equals "$RUN_WORKDIR/cards.txt" "$expected_file" "T1-04 file reflects first-match delete"
}

case_t1_05_delete_missing_name() {
    run_program "T1-05" "$DATA_DIR/valid_cards.txt" $'3\nMissingNo\n6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T1-05 exits successfully"
    assert_contains "$RUN_OUTPUT" 'No card found with name "MissingNo".' "T1-05 missing-name message"
    assert_file_equals "$RUN_WORKDIR/cards.txt" "$DATA_DIR/valid_cards.txt" "T1-05 file unchanged without save"
}

case_t1_06_menu_out_of_range() {
    run_program "T1-06" "" $'0\n6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T1-06 exits successfully"
    assert_contains "$RUN_OUTPUT" "Please enter a menu option from 1 to 6." "T1-06 range validation"
}

case_t1_07_menu_non_integer() {
    run_program "T1-07" "" $'abc\n6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T1-07 exits successfully"
    assert_contains "$RUN_OUTPUT" "Please enter a valid integer." "T1-07 integer validation"
}

case_t1_08_reject_delimiter_input() {
    run_program "T1-08" "" $'2\nBad|Name\nPika\nElectric\n35\n3.99\n6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T1-08 exits successfully"
    assert_contains "$RUN_OUTPUT" "Character '|' is not allowed in this field." "T1-08 delimiter rejection"
    assert_contains "$RUN_OUTPUT" "Card added. Total cards: 1" "T1-08 recovers with valid name"
}

case_t1_09_numeric_validation_retries() {
    run_program "T1-09" "" $'2\nAbra\nPsychic\nabc\n-5\n25\n1..2\n-3\n2.5\n6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T1-09 exits successfully"
    assert_contains "$RUN_OUTPUT" "Please enter a valid integer." "T1-09 invalid integer text rejected"
    assert_contains "$RUN_OUTPUT" "Please enter a non-negative integer." "T1-09 negative integer rejected"
    assert_contains "$RUN_OUTPUT" "Please enter a valid number (example: 12.50)." "T1-09 malformed decimal rejected"
    assert_contains "$RUN_OUTPUT" "Please enter a non-negative value." "T1-09 negative decimal rejected"
}

case_t1_10_load_mixed_malformed_rows() {
    run_program "T1-10" "$DATA_DIR/mixed_malformed.txt" $'6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T1-10 exits successfully"
    assert_contains "$RUN_OUTPUT" "Skipped 4 malformed line(s) while reading cards.txt." "T1-10 malformed line count"
    assert_contains "$RUN_OUTPUT" "Loaded 2 card(s) from cards.txt." "T1-10 valid line count"
}

case_t1_11_save_empty_list() {
    run_program "T1-11" "" $'4\n6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T1-11 exits successfully"
    assert_contains "$RUN_OUTPUT" "Saved 0 card(s) to cards.txt." "T1-11 empty save message"
    assert_file_exists "$RUN_WORKDIR/cards.txt" "T1-11 cards file created"
    assert_file_line_count "$RUN_WORKDIR/cards.txt" 0 "T1-11 empty file line count"
}

case_t1_12_save_decimal_formatting() {
    local expected_file
    run_program "T1-12" "" $'2\nSnorlax\nNormal\n160\n3\n4\n6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T1-12 exits successfully"
    expected_file=$(create_temp_file_with_content $'Snorlax|Normal|160|3.00\n')
    assert_file_equals "$RUN_WORKDIR/cards.txt" "$expected_file" "T1-12 dollar value saved with two decimals"
}

case_t2_01_max_length_inputs_accepted() {
    local name63
    local type31
    local input_payload
    local expected_file
    local expected_row

    name63=$(printf 'N%.0s' $(seq 1 63))
    type31=$(printf 'T%.0s' $(seq 1 31))
    input_payload=$(printf '2\n%s\n%s\n1\n1\n4\n6\n' "$name63" "$type31")
    printf -v expected_row '%s|%s|1|1.00\n' "$name63" "$type31"

    run_program "T2-01" "" "$input_payload"
    assert_exit_code "$RUN_EXIT_CODE" 0 "T2-01 exits successfully"
    expected_file=$(create_temp_file_with_content "$expected_row")
    assert_file_equals "$RUN_WORKDIR/cards.txt" "$expected_file" "T2-01 max safe lengths persisted"
}

case_t2_02_overlength_name_skipped_on_load() {
    local fixture
    local name64
    local row
    name64=$(printf 'A%.0s' $(seq 1 64))
    printf -v row '%s|Fire|10|1.00\n' "$name64"
    fixture=$(create_temp_file_with_content "$row")

    run_program "T2-02" "$fixture" $'6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T2-02 exits successfully"
    assert_contains "$RUN_OUTPUT" "Skipped 1 malformed line(s) while reading cards.txt." "T2-02 overlength line skipped"
    assert_contains "$RUN_OUTPUT" "Loaded 0 card(s) from cards.txt." "T2-02 no valid cards loaded"
}

case_t2_03_long_interactive_name_truncates_safely() {
    local long_name
    local input_payload
    local saved_name
    local saved_length

    long_name=$(printf 'X%.0s' $(seq 1 300))
    input_payload=$(printf '2\n%s\nElectric\n35\n3.99\n4\n6\n' "$long_name")

    run_program "T2-03" "" "$input_payload"
    assert_exit_code "$RUN_EXIT_CODE" 0 "T2-03 exits successfully"
    assert_contains "$RUN_OUTPUT" "Card added. Total cards: 1" "T2-03 card still added after long input"
    saved_name=$(cut -d'|' -f1 "$RUN_WORKDIR/cards.txt")
    saved_length=${#saved_name}
    assert_equals "$saved_length" 63 "T2-03 long name truncated to NAME_LEN-1"
}

case_t2_04_long_file_line_without_newline_skipped() {
    local fixture
    local long_line
    long_line=$(printf 'Y%.0s' $(seq 1 700))
    fixture=$(create_temp_file_with_content "$long_line")

    run_program "T2-04" "$fixture" $'6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T2-04 exits successfully"
    assert_contains "$RUN_OUTPUT" "Skipped 1 malformed line(s) while reading cards.txt." "T2-04 oversized line skipped"
    assert_contains "$RUN_OUTPUT" "Loaded 0 card(s) from cards.txt." "T2-04 no valid card loaded"
}

case_t2_05_capacity_growth_twenty_cards() {
    local input_payload
    local i

    input_payload=""
    for i in $(seq 1 20); do
        input_payload+=$'2\n'
        input_payload+="Card${i}"$'\n'
        input_payload+=$'Type\n1\n1\n'
    done
    input_payload+=$'4\n6\n'

    run_program "T2-05" "" "$input_payload"
    assert_exit_code "$RUN_EXIT_CODE" 0 "T2-05 exits successfully"
    assert_contains "$RUN_OUTPUT" "Saved 20 card(s) to cards.txt." "T2-05 save count"
    assert_file_line_count "$RUN_WORKDIR/cards.txt" 20 "T2-05 file has 20 lines"
    assert_contains "$(cat "$RUN_WORKDIR/cards.txt")" "Card20|Type|1|1.00" "T2-05 last card persisted"
}

case_t2_06_whitespace_preserved_roundtrip() {
    local first_fixture

    run_program "T2-06A" "" $'2\nMr Mime\nPsychic Fairy\n40\n2.2\n4\n6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T2-06A save exits successfully"
    assert_contains "$(cat "$RUN_WORKDIR/cards.txt")" "Mr Mime|Psychic Fairy|40|2.20" "T2-06A spaces persisted in file"

    first_fixture="$RUN_WORKDIR/cards.txt"
    run_program "T2-06B" "$first_fixture" $'1\n6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T2-06B reload exits successfully"
    assert_contains "$RUN_OUTPUT" 'Name: Mr Mime | Type: Psychic Fairy | Hit Points: 40 | Dollar Value: $2.20' "T2-06B spaces preserved on display"
}

case_t2_07_zero_values_accepted() {
    local expected_file
    run_program "T2-07" "" $'2\nZeroMon\nNull\n0\n0\n4\n6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T2-07 exits successfully"
    expected_file=$(create_temp_file_with_content $'ZeroMon|Null|0|0.00\n')
    assert_file_equals "$RUN_WORKDIR/cards.txt" "$expected_file" "T2-07 zero values saved"
}

case_t2_08_save_failure_readonly_dir() {
    run_program "T2-08" "" $'4\n6\n' "readonly_dir"
    assert_contains "$RUN_OUTPUT" "Save failed." "T2-08 save failure message"
}

case_t2_09_extra_delimiter_line_skipped() {
    run_program "T2-09" "$DATA_DIR/malformed_extra_delimiter.txt" $'6\n'
    assert_exit_code "$RUN_EXIT_CODE" 0 "T2-09 exits successfully"
    assert_contains "$RUN_OUTPUT" "Skipped 1 malformed line(s) while reading cards.txt." "T2-09 extra-delimiter line skipped"
    assert_contains "$RUN_OUTPUT" "Loaded 1 card(s) from cards.txt." "T2-09 valid line still loaded"
}

run_case_list() {
    local case_fn
    for case_fn in "$@"; do
        "$case_fn"
    done
}

run_tier1() {
    run_case_list \
        case_t1_01_missing_file_start \
        case_t1_02_add_list_save_single \
        case_t1_03_load_existing_cards \
        case_t1_04_delete_first_exact_match \
        case_t1_05_delete_missing_name \
        case_t1_06_menu_out_of_range \
        case_t1_07_menu_non_integer \
        case_t1_08_reject_delimiter_input \
        case_t1_09_numeric_validation_retries \
        case_t1_10_load_mixed_malformed_rows \
        case_t1_11_save_empty_list \
        case_t1_12_save_decimal_formatting
}

run_tier2() {
    run_case_list \
        case_t2_01_max_length_inputs_accepted \
        case_t2_02_overlength_name_skipped_on_load \
        case_t2_03_long_interactive_name_truncates_safely \
        case_t2_04_long_file_line_without_newline_skipped \
        case_t2_05_capacity_growth_twenty_cards \
        case_t2_06_whitespace_preserved_roundtrip \
        case_t2_07_zero_values_accepted \
        case_t2_08_save_failure_readonly_dir \
        case_t2_09_extra_delimiter_line_skipped
}

main() {
    local suite="${1:-all}"

    if [[ ! -x "$BIN" ]]; then
        printf 'Binary not found or not executable: %s\n' "$BIN" >&2
        printf 'Build first with: make -C src february5\n' >&2
        exit 2
    fi

    case "$suite" in
        tier1)
            run_tier1
            ;;
        tier2)
            run_tier2
            ;;
        all)
            run_tier1
            run_tier2
            ;;
        *)
            printf 'Usage: %s [tier1|tier2|all]\n' "$(basename "$0")" >&2
            exit 2
            ;;
    esac

    if finish_assertions; then
        exit 0
    fi
    exit 1
}

main "$@"
