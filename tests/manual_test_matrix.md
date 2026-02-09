# Manual Test Matrix

Run manual cases from `src/` after building with:

```bash
make clean && make february5
```

Record outcomes in the `Observed` and `Pass/Fail` columns.

## Tier 1 (Core Regression)

| Case ID | Purpose | Preconditions | Input Steps | Expected Result | Observed | Pass/Fail |
| --- | --- | --- | --- | --- | --- | --- |
| T1-01 | Startup without file | No `cards.txt` in cwd | Run app, choose `6` | Shows missing-file message and exits cleanly | | |
| T1-02 | Add/list/save single card | No `cards.txt` in cwd | Add card, list, save, exit | Card appears in list and `cards.txt` has one valid line | | |
| T1-03 | Load persisted cards | `cards.txt` has valid records | Run app, list, exit | Reports loaded count and shows cards | | |
| T1-04 | Delete first exact duplicate | `cards.txt` has duplicate name rows | Delete by duplicate name, save, exit | First match removed, later duplicate preserved | | |
| T1-05 | Delete missing name | `cards.txt` has valid records | Delete unknown name, exit without save | Shows not-found message, file unchanged | | |
| T1-06 | Menu out-of-range | Any startup state | Enter `0`, then `6` | Shows menu range validation message | | |
| T1-07 | Menu non-integer | Any startup state | Enter `abc`, then `6` | Shows integer validation and reprompts | | |
| T1-08 | Delimiter rejection | Any startup state | Add card with `|` in name/type, then valid input | Rejects delimiter and eventually accepts valid input | | |
| T1-09 | Numeric retry paths | Any startup state | Add card with invalid/negative hp and value before valid entries | Shows validation messages for each invalid number | | |
| T1-10 | Mixed malformed load | `cards.txt` has valid + malformed rows | Start app, exit | Loads valid rows and prints skipped malformed count | | |
| T1-11 | Save empty list | No cards loaded/added | Choose save, then exit | Creates/overwrites `cards.txt` as empty file | | |
| T1-12 | Save decimal formatting | Any startup state | Add card with value `3`, save, exit | Saved value formatted as `3.00` | | |

## Tier 2 (Stress and Rare Edges)

| Case ID | Purpose | Preconditions | Input Steps | Expected Result | Observed | Pass/Fail |
| --- | --- | --- | --- | --- | --- | --- |
| T2-01 | Boundary accepted lengths | Any startup state | Add name length `NAME_LEN-1`, type length `TYPE_LEN-1`, save | Accepted and persisted correctly | | |
| T2-02 | Boundary rejected on load | `cards.txt` has name length `NAME_LEN` | Start app, exit | Row skipped as malformed and loaded count excludes it | | |
| T2-03 | Long interactive line handling | Any startup state | Enter very long name while adding card | Program remains stable; line is truncated safely | | |
| T2-04 | Long file line without newline | `cards.txt` has >512 char line with no newline | Start app, exit | Long line skipped and parse continues safely | | |
| T2-05 | Capacity growth stress | Any startup state | Add 20+ cards, save, exit | All records persist with no corruption | | |
| T2-06 | Whitespace preservation | Any startup state | Add card with spaces in name/type, save/reload/list | Spaces preserved across save/load | | |
| T2-07 | Zero-value numeric fields | Any startup state | Add hp `0` and value `0`, save | Accepted and persisted as `0` and `0.00` | | |
| T2-08 | Save failure path | Run in read-only working directory | Choose save | Displays save failure cleanly, app remains responsive | | |
| T2-09 | Extra delimiter malformed line | `cards.txt` has line with extra `|` | Start app, exit | Extra-delimiter line skipped as malformed | | |
