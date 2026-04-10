/*
* Anjana Kummetha
* 117162837
* student_tests.c
*
* Test cases covering:
*   - All 28 cases from the document table (Part 1 validation)
*   - Part 2: simple search and replace behavior
*   - Part 3: wildcard search and replace behavior
*/


#include "unit_tests.h"
#include "hw6.h"


static char args[ARGS_TEXT_LEN];


TestSuite(student_tests, .timeout=TEST_TIMEOUT);


/* =========================================================================
* PART 1: ALL 28 CASES FROM THE DOCUMENT TABLE
* ========================================================================= */


/*
* Case 1: -s the -r end -l 10,15 input.txt output.txt
* Expected: Successful run (return 0)
*/
Test(student_tests, doc_case01) {
   char *test_name = "st_doc_case01";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s the -r end -l 10,15 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/*
* Case 2: -s the -r end -h -Z moo -l 10,15 -q hello input.txt output.txt
* Expected: Successful run — unknown flags -h -Z -q are ignored
*/
Test(student_tests, doc_case02) {
   char *test_name = "st_doc_case02";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s the -r end -h -Z moo -l 10,15 -q hello tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/*
* Case 3: -l 10,15 -s the -r end input.txt output.txt
* Expected: Successful run — order of valid flags does not matter
*/
Test(student_tests, doc_case03) {
   char *test_name = "st_doc_case03";
   prep_files("unix.txt", test_name);
   sprintf(args, "-l 10,15 -s the -r end tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/*
* Case 4: -l 10,15 -r the -s input.txt output.txt
* Expected: Successful run — infile becomes the argument to -s per the spec
*/
Test(student_tests, doc_case04) {
   char *test_name = "st_doc_case04";
   prep_files("unix.txt", test_name);
   sprintf(args, "-l 10,15 -r the -s tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/*
* Case 5: -r end -s *the* -l xyz10,15 -w input.txt output.txt
* Expected: INPUT_FILE_MISSING — "-w" is treated as infile, cannot be opened
*/
Test(student_tests, doc_case05) {
   char *test_name = "st_doc_case05";
   sprintf(args, "-r end -s \"*the*\" -l xyz10,15 -w tests.out/%s.out.txt",
           test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, INPUT_FILE_MISSING);
}


/*
* Case 6: -s the -r end input.txt /output.txt
* Expected: OUTPUT_FILE_UNWRITABLE — cannot write to root directory
*/
Test(student_tests, doc_case06) {
   char *test_name = "st_doc_case06";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s the -r end tests.in/%s.in.txt /st_doc_case06_output.txt",
           test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, OUTPUT_FILE_UNWRITABLE);
}


/*
* Case 7: -s the -r end input.txt readonly.txt
* Expected: OUTPUT_FILE_UNWRITABLE — output file is read-only
*/
Test(student_tests, doc_case07) {
   char *test_name = "st_doc_case07";
   prep_files("unix.txt", test_name);
   char cmd[200];
   sprintf(cmd, "touch tests.out/%s.out.txt && chmod 444 tests.out/%s.out.txt",
           test_name, test_name);
   system(cmd);
   sprintf(args, "-s the -r end tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, OUTPUT_FILE_UNWRITABLE);
}


/*
* Case 8: (no arguments)
* Expected: MISSING_ARGUMENT
*/
Test(student_tests, doc_case08) {
   char *test_name = "st_doc_case08";
   sprintf(args, "");
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, MISSING_ARGUMENT);
}


/*
* Case 9: -s the -r end /trash.txt
* Expected: MISSING_ARGUMENT — fewer than 7 args total
*/
Test(student_tests, doc_case09) {
   char *test_name = "st_doc_case09";
   sprintf(args, "-s the -r end /trash.txt");
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, MISSING_ARGUMENT);
}


/*
* Case 10: -s stony input.txt output.txt
* Expected: MISSING_ARGUMENT — fewer than 7 args total
*/
Test(student_tests, doc_case10) {
   char *test_name = "st_doc_case10";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s stony tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, MISSING_ARGUMENT);
}


/*
* Case 11: input.txt output.txt
* Expected: MISSING_ARGUMENT — fewer than 7 args total
*/
Test(student_tests, doc_case11) {
   char *test_name = "st_doc_case11";
   prep_files("unix.txt", test_name);
   sprintf(args, "tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, MISSING_ARGUMENT);
}


/*
* Case 12: -s -r end -l 10,15 -w input.txt output.txt
* Expected: S_ARGUMENT_MISSING — -s grabs -r as its value (starts with -)
*/
Test(student_tests, doc_case12) {
   char *test_name = "st_doc_case12";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s -r end -l 10,15 -w tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, S_ARGUMENT_MISSING);
}


/*
* Case 13: -s the -r -l 10,15 input.txt output.txt
* Expected: R_ARGUMENT_MISSING — -r grabs -l as its value (starts with -)
*/
Test(student_tests, doc_case13) {
   char *test_name = "st_doc_case13";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s the -r -l 10,15 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, R_ARGUMENT_MISSING);
}


/*
* Case 14: -s the -r end -l input.txt output.txt
* Expected: L_ARGUMENT_INVALID — -l gets the filename as its value (no comma)
*/
Test(student_tests, doc_case14) {
   char *test_name = "st_doc_case14";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s the -r end -l tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, L_ARGUMENT_INVALID);
}


/*
* Case 15: -s the -r -l input.txt output.txt
* Expected: R_ARGUMENT_MISSING — higher priority than L_ARGUMENT_INVALID
*/
Test(student_tests, doc_case15) {
   char *test_name = "st_doc_case15";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s the -r -l tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, R_ARGUMENT_MISSING);
}


/*
* Case 16: -s the -r end -l 10sbu,15 input.txt output.txt
* Expected: Successful run — "10sbu" parsed as 10 by strtol (valid per spec)
*/
Test(student_tests, doc_case16) {
   char *test_name = "st_doc_case16";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s the -r end -l 10sbu,15 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/*
* Case 17: -s the -r end -l 10,sbu15 input.txt output.txt
* Expected: L_ARGUMENT_INVALID — "sbu15" parsed as 0 by strtol (invalid)
*/
Test(student_tests, doc_case17) {
   char *test_name = "st_doc_case17";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s the -r end -l 10,sbu15 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, L_ARGUMENT_INVALID);
}


/*
* Case 18: -s the -r end -l 10 input.txt output.txt
* Expected: L_ARGUMENT_INVALID — no comma, only one number
*/
Test(student_tests, doc_case18) {
   char *test_name = "st_doc_case18";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s the -r end -l 10 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, L_ARGUMENT_INVALID);
}


/*
* Case 19: -s the* -w -r end -l 10,15 input.txt output.txt
* Expected: Successful run — "the*" is a valid prefix wildcard
*/
Test(student_tests, doc_case19) {
   char *test_name = "st_doc_case19";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s \"the*\" -w -r end -l 10,15 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/*
* Case 20: -s the* -w -r end -l 10,15bonk input.txt output.txt
* Expected: Successful run — "15bonk" parsed as 15 by strtol (valid)
*/
Test(student_tests, doc_case20) {
   char *test_name = "st_doc_case20";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s \"the*\" -w -r end -l 10,15bonk tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/*
* Case 21: -w -r end -s *the -l 10,15bonk input.txt output.txt
* Expected: Successful run — "*the" is a valid suffix wildcard
*/
Test(student_tests, doc_case21) {
   char *test_name = "st_doc_case21";
   prep_files("unix.txt", test_name);
   sprintf(args, "-w -r end -s \"*the\" -l 10,15bonk tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/*
* Case 22: -s the -w -r end -l 10,15 input.txt output.txt
* Expected: WILDCARD_INVALID — "the" has no '*' but -w is present
*/
Test(student_tests, doc_case22) {
   char *test_name = "st_doc_case22";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s the -w -r end -l 10,15 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, WILDCARD_INVALID);
}


/*
* Case 23: -s *the* -w -r end -l 10,15 input.txt output.txt
* Expected: WILDCARD_INVALID — '*' on both ends
*/
Test(student_tests, doc_case23) {
   char *test_name = "st_doc_case23";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s \"*the*\" -w -r end -l 10,15 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, WILDCARD_INVALID);
}


/*
* Case 24: -s the -w -r end -r end -s hello -l 10,15 input.txt output.txt
* Expected: DUPLICATE_ARGUMENT — higher priority than WILDCARD_INVALID
*/
Test(student_tests, doc_case24) {
   char *test_name = "st_doc_case24";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s the -w -r end -r end -s hello -l 10,15 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, DUPLICATE_ARGUMENT);
}


/*
* Case 25: -s the -r end -l 10,15 -s hello input.txt output.txt
* Expected: DUPLICATE_ARGUMENT — -s appears twice
*/
Test(student_tests, doc_case25) {
   char *test_name = "st_doc_case25";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s the -r end -l 10,15 -s hello tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, DUPLICATE_ARGUMENT);
}


/*
* Case 26: -s the -r end -l 10,15 -r what input.txt output.txt
* Expected: DUPLICATE_ARGUMENT — -r appears twice
*/
Test(student_tests, doc_case26) {
   char *test_name = "st_doc_case26";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s the -r end -l 10,15 -r what tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, DUPLICATE_ARGUMENT);
}


/*
* Case 27: -w -r end -s *the -l 10,15 -w input.txt output.txt
* Expected: DUPLICATE_ARGUMENT — -w appears twice
*/
Test(student_tests, doc_case27) {
   char *test_name = "st_doc_case27";
   prep_files("unix.txt", test_name);
   sprintf(args, "-w -r end -s \"*the\" -l 10,15 -w tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, DUPLICATE_ARGUMENT);
}


/*
* Case 28: -s the -r end* -l 10,15 input.txt output.txt
* Expected: Successful run — '*' in replace text is fine, only search text
* is validated for wildcards
*/
Test(student_tests, doc_case28) {
   char *test_name = "st_doc_case28";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s the -r \"end*\" -l 10,15 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* =========================================================================
* PART 2: SIMPLE SEARCH AND REPLACE BEHAVIOR
* These tests check return code is 0 (success) for valid replacements.
* They use the same input files and args as the TA tests but with
* different test names so they run independently.
* ========================================================================= */


/* Replace a word over the entire file */
Test(student_tests, simple_replace_whole_file) {
   char *test_name = "st_simple_replace_whole_file";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s the -r WOLFIE tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Replace only on a single line (start == end) */
Test(student_tests, simple_replace_single_line) {
   char *test_name = "st_simple_replace_single_line";
   prep_files("turing.txt", test_name);
   sprintf(args, "-s machine -r device -l 23,23 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Replace within a range — search term also appears outside the range */
Test(student_tests, simple_replace_in_range_only) {
   char *test_name = "st_simple_replace_in_range_only";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s early -r \"WELL TIMED\" -l 11,23 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Replace with a shorter replacement text */
Test(student_tests, simple_replace_shorter_text) {
   char *test_name = "st_simple_replace_shorter_text";
   prep_files("turing.txt", test_name);
   sprintf(args, "-s intelligence -r wisdom -l 1,22 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Replace with a longer replacement text */
Test(student_tests, simple_replace_longer_text) {
   char *test_name = "st_simple_replace_longer_text";
   prep_files("turing.txt", test_name);
   sprintf(args, "-s Turing -r \"TURING THE GREAT\" tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Replace when search term appears at the start of lines */
Test(student_tests, simple_replace_start_of_line) {
   char *test_name = "st_simple_replace_start_of_line";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s In -r During tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Replace in a file that contains only one word */
Test(student_tests, simple_replace_one_word_file) {
   char *test_name = "st_simple_replace_one_word_file";
   prep_files("oneword.txt", test_name);
   sprintf(args, "-s Homework -r \"This homework is effortless.\" tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Replace when search term does not appear in the file at all */
Test(student_tests, simple_replace_no_match) {
   char *test_name = "st_simple_replace_no_match";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s CSE -r \"Computer Science\" -l 1,45 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Replace when end line is greater than number of lines in the file */
Test(student_tests, simple_replace_end_beyond_file) {
   char *test_name = "st_simple_replace_end_beyond_file";
   prep_files("oneword.txt", test_name);
   sprintf(args, "-s Homework -r Assignment -l 1,9999 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* =========================================================================
* PART 3: WILDCARD SEARCH AND REPLACE BEHAVIOR
* Tests cover prefix wildcards (foo*), suffix wildcards (*foo),
* single word files, no match cases, and punctuation edge cases.
* ========================================================================= */


/* Prefix wildcard over entire file */
Test(student_tests, wildcard_prefix_whole_file) {
   char *test_name = "st_wildcard_prefix_whole_file";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s \"re*\" -w -r \"New York\" tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Prefix wildcard on a single line */
Test(student_tests, wildcard_prefix_single_line) {
   char *test_name = "st_wildcard_prefix_single_line";
   prep_files("turing.txt", test_name);
   sprintf(args, "-s \"mac*\" -w -r device -l 23,23 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Prefix wildcard within a range */
Test(student_tests, wildcard_prefix_in_range) {
   char *test_name = "st_wildcard_prefix_in_range";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s \"ea*\" -w -r \"WELL TIMED\" -l 11,23 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Prefix wildcard — search term followed by punctuation */
Test(student_tests, wildcard_prefix_before_punctuation) {
   char *test_name = "st_wildcard_prefix_before_punctuation";
   prep_files("turing.txt", test_name);
   sprintf(args, "-l 1,22 -s \"intel*\" -w -r wisdom tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Prefix wildcard — multiple occurrences throughout the file */
Test(student_tests, wildcard_prefix_multiple_occurrences) {
   char *test_name = "st_wildcard_prefix_multiple_occurrences";
   prep_files("turing.txt", test_name);
   sprintf(args, "-w -s \"Tur*\" -r \"TURING THE GREAT\" tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Prefix wildcard on a single word file */
Test(student_tests, wildcard_prefix_one_word_file) {
   char *test_name = "st_wildcard_prefix_one_word_file";
   prep_files("oneword.txt", test_name);
   sprintf(args, "-s \"Hom*\" -w -r \"This homework is effortless.\" tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Prefix wildcard — search term does not appear in the file */
Test(student_tests, wildcard_prefix_no_match) {
   char *test_name = "st_wildcard_prefix_no_match";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s \"CSE*\" -w -r \"Computer Science\" -l 1,45 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Prefix wildcard — no space after punctuation marks */
Test(student_tests, wildcard_prefix_no_space_after_punct) {
   char *test_name = "st_wildcard_prefix_no_space_after_punct";
   prep_files("punctuation.txt", test_name);
   sprintf(args, "-w -s \"Bu*\" -r \" However,\" tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Prefix wildcard — search term is itself a complete word */
Test(student_tests, wildcard_prefix_exact_word) {
   char *test_name = "st_wildcard_prefix_exact_word";
   prep_files("turing.txt", test_name);
   sprintf(args, "-s \"with*\" -w -r WITH tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Suffix wildcard over entire file */
Test(student_tests, wildcard_suffix_whole_file) {
   char *test_name = "st_wildcard_suffix_whole_file";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s \"*re\" -w -r \"New York\" tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Suffix wildcard — multiple occurrences throughout file */
Test(student_tests, wildcard_suffix_multiple_occurrences) {
   char *test_name = "st_wildcard_suffix_multiple_occurrences";
   prep_files("turing.txt", test_name);
   sprintf(args, "-w -s \"*ring\" -r TURING tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Suffix wildcard on a single line */
Test(student_tests, wildcard_suffix_single_line) {
   char *test_name = "st_wildcard_suffix_single_line";
   prep_files("turing.txt", test_name);
   sprintf(args, "-s \"*chine\" -w -r device -l 23,23 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Suffix wildcard within a range */
Test(student_tests, wildcard_suffix_in_range) {
   char *test_name = "st_wildcard_suffix_in_range";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s \"*rly\" -w -r \"WELL TIMED\" -l 11,23 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Suffix wildcard — search term followed by punctuation */
Test(student_tests, wildcard_suffix_before_punctuation) {
   char *test_name = "st_wildcard_suffix_before_punctuation";
   prep_files("turing.txt", test_name);
   sprintf(args, "-l 1,22 -s \"*gence\" -w -r wisdom tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Suffix wildcard on a single word file */
Test(student_tests, wildcard_suffix_one_word_file) {
   char *test_name = "st_wildcard_suffix_one_word_file";
   prep_files("oneword.txt", test_name);
   sprintf(args, "-w -s \"*ork\" -r \"This homework is effortless.\" tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Suffix wildcard — search term does not appear in the file */
Test(student_tests, wildcard_suffix_no_match) {
   char *test_name = "st_wildcard_suffix_no_match";
   prep_files("unix.txt", test_name);
   sprintf(args, "-s \"*CSE\" -w -r \"Computer Science\" -l 1,45 tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Suffix wildcard — no space after punctuation marks */
Test(student_tests, wildcard_suffix_no_space_after_punct) {
   char *test_name = "st_wildcard_suffix_no_space_after_punct";
   prep_files("punctuation.txt", test_name);
   sprintf(args, "-s \"*So\" -w -r \" Therefore,\" tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Suffix wildcard — search term is itself a complete word */
Test(student_tests, wildcard_suffix_exact_word) {
   char *test_name = "st_wildcard_suffix_exact_word";
   prep_files("turing.txt", test_name);
   sprintf(args, "-s \"*with\" -w -r WITH tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}


/* Suffix wildcard — word appears at end of file with no trailing newline */
Test(student_tests, wildcard_suffix_end_of_file) {
   char *test_name = "st_wildcard_suffix_end_of_file";
   prep_files("punctuation.txt", test_name);
   sprintf(args, "-s \"*ode\" -w -r \"CODE.\" tests.in/%s.in.txt tests.out/%s.out.txt",
           test_name, test_name);
   int status = run_using_system_no_valgrind(test_name, args);
   expect_error_exit(status, 0);
}
