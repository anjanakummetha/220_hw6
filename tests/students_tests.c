/*
 * Anjana Kummetha
 * 117162837
 * student_tests.c
 */

#include "unit_tests.h"
#include "hw6.h"

static char args[ARGS_TEXT_LEN];

TestSuite(student_tests, .timeout=TEST_TIMEOUT);

//Part 1 (cases 1-28)

//valid run with -l range
Test(student_tests, doc_case01) {
    char *test_name = "st_doc_case01";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s the -r end -l 10,15 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//unknown flags ignored
Test(student_tests, doc_case02) {
    char *test_name = "st_doc_case02";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s the -r end -h -Z moo -l 10,15 -q hello tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//flag order does not matter
Test(student_tests, doc_case03) {
    char *test_name = "st_doc_case03";
    prep_files("unix.txt", test_name);
    sprintf(args, "-l 10,15 -s the -r end tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//infile becomes -s value
Test(student_tests, doc_case04) {
    char *test_name = "st_doc_case04";
    prep_files("unix.txt", test_name);
    sprintf(args, "-l 10,15 -r the -s tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//-w treated as infile, cannot open
Test(student_tests, doc_case05) {
    char *test_name = "st_doc_case05";
    sprintf(args, "-r end -s \"*the*\" -l xyz10,15 -w tests.out/%s.out.txt",
            test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, INPUT_FILE_MISSING);
}

//output in root directory
Test(student_tests, doc_case06) {
    char *test_name = "st_doc_case06";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s the -r end tests.in/%s.in.txt /st_doc_case06_output.txt",
            test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, OUTPUT_FILE_UNWRITABLE);
}

//output file is read-only
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

//no arguments
Test(student_tests, doc_case08) {
    char *test_name = "st_doc_case08";
    sprintf(args, "");
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, MISSING_ARGUMENT);
}

//fewer than 7 args
Test(student_tests, doc_case09) {
    char *test_name = "st_doc_case09";
    sprintf(args, "-s the -r end /trash.txt");
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, MISSING_ARGUMENT);
}

//only -s and files, missing -r
Test(student_tests, doc_case10) {
    char *test_name = "st_doc_case10";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s stony tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, MISSING_ARGUMENT);
}

//only filenames, no flags
Test(student_tests, doc_case11) {
    char *test_name = "st_doc_case11";
    prep_files("unix.txt", test_name);
    sprintf(args, "tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, MISSING_ARGUMENT);
}

//-s missing value, grabs -r
Test(student_tests, doc_case12) {
    char *test_name = "st_doc_case12";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s -r end -l 10,15 -w tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, S_ARGUMENT_MISSING);
}

//-r missing value, grabs -l
Test(student_tests, doc_case13) {
    char *test_name = "st_doc_case13";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s the -r -l 10,15 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, R_ARGUMENT_MISSING);
}

//-l grabs filename, no comma
Test(student_tests, doc_case14) {
    char *test_name = "st_doc_case14";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s the -r end -l tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, L_ARGUMENT_INVALID);
}

//-r missing, higher priority than -l
Test(student_tests, doc_case15) {
    char *test_name = "st_doc_case15";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s the -r -l tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, R_ARGUMENT_MISSING);
}

//10sbu parsed as 10, valid
Test(student_tests, doc_case16) {
    char *test_name = "st_doc_case16";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s the -r end -l 10sbu,15 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//sbu15 parsed as 0, invalid
Test(student_tests, doc_case17) {
    char *test_name = "st_doc_case17";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s the -r end -l 10,sbu15 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, L_ARGUMENT_INVALID);
}

//-l no comma
Test(student_tests, doc_case18) {
    char *test_name = "st_doc_case18";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s the -r end -l 10 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, L_ARGUMENT_INVALID);
}

//valid prefix wildcard
Test(student_tests, doc_case19) {
    char *test_name = "st_doc_case19";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s \"the*\" -w -r end -l 10,15 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//valid wildcard, trailing letters in end line
Test(student_tests, doc_case20) {
    char *test_name = "st_doc_case20";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s \"the*\" -w -r end -l 10,15bonk tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//valid suffix wildcard
Test(student_tests, doc_case21) {
    char *test_name = "st_doc_case21";
    prep_files("unix.txt", test_name);
    sprintf(args, "-w -r end -s \"*the\" -l 10,15bonk tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//no * in search, -w present
Test(student_tests, doc_case22) {
    char *test_name = "st_doc_case22";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s the -w -r end -l 10,15 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, WILDCARD_INVALID);
}

//* on both ends
Test(student_tests, doc_case23) {
    char *test_name = "st_doc_case23";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s \"*the*\" -w -r end -l 10,15 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, WILDCARD_INVALID);
}

//duplicate beats wildcard invalid
Test(student_tests, doc_case24) {
    char *test_name = "st_doc_case24";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s the -w -r end -r end -s hello -l 10,15 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, DUPLICATE_ARGUMENT);
}

//-s appears twice
Test(student_tests, doc_case25) {
    char *test_name = "st_doc_case25";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s the -r end -l 10,15 -s hello tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, DUPLICATE_ARGUMENT);
}

//-r appears twice
Test(student_tests, doc_case26) {
    char *test_name = "st_doc_case26";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s the -r end -l 10,15 -r what tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, DUPLICATE_ARGUMENT);
}

//-w appears twice
Test(student_tests, doc_case27) {
    char *test_name = "st_doc_case27";
    prep_files("unix.txt", test_name);
    sprintf(args, "-w -r end -s \"*the\" -l 10,15 -w tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, DUPLICATE_ARGUMENT);
}

//* in replace text is fine
Test(student_tests, doc_case28) {
    char *test_name = "st_doc_case28";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s the -r \"end*\" -l 10,15 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}







//part 2

//replace over entire file
Test(student_tests, simple_replace_whole_file) {
    char *test_name = "st_simple_replace_whole_file";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s the -r WOLFIE tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//start line == end line
Test(student_tests, simple_replace_single_line) {
    char *test_name = "st_simple_replace_single_line";
    prep_files("turing.txt", test_name);
    sprintf(args, "-s machine -r device -l 23,23 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//term exists outside range, only replaced inside
Test(student_tests, simple_replace_in_range_only) {
    char *test_name = "st_simple_replace_in_range_only";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s early -r \"WELL TIMED\" -l 11,23 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//replace with shorter text
Test(student_tests, simple_replace_shorter_text) {
    char *test_name = "st_simple_replace_shorter_text";
    prep_files("turing.txt", test_name);
    sprintf(args, "-s intelligence -r wisdom -l 1,22 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//replace with longer text
Test(student_tests, simple_replace_longer_text) {
    char *test_name = "st_simple_replace_longer_text";
    prep_files("turing.txt", test_name);
    sprintf(args, "-s Turing -r \"TURING THE GREAT\" tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//term at start of lines
Test(student_tests, simple_replace_start_of_line) {
    char *test_name = "st_simple_replace_start_of_line";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s In -r During tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//single word input file
Test(student_tests, simple_replace_one_word_file) {
    char *test_name = "st_simple_replace_one_word_file";
    prep_files("oneword.txt", test_name);
    sprintf(args, "-s Homework -r \"This homework is effortless.\" tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//search term not in file
Test(student_tests, simple_replace_no_match) {
    char *test_name = "st_simple_replace_no_match";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s CSE -r \"Computer Science\" -l 1,45 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//end line beyond file length
Test(student_tests, simple_replace_end_beyond_file) {
    char *test_name = "st_simple_replace_end_beyond_file";
    prep_files("oneword.txt", test_name);
    sprintf(args, "-s Homework -r Assignment -l 1,9999 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}











//part 3

//prefix wildcard whole file
Test(student_tests, wildcard_prefix_whole_file) {
    char *test_name = "st_wildcard_prefix_whole_file";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s \"re*\" -w -r \"New York\" tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//prefix wildcard single line
Test(student_tests, wildcard_prefix_single_line) {
    char *test_name = "st_wildcard_prefix_single_line";
    prep_files("turing.txt", test_name);
    sprintf(args, "-s \"mac*\" -w -r device -l 23,23 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//prefix wildcard within range
Test(student_tests, wildcard_prefix_in_range) {
    char *test_name = "st_wildcard_prefix_in_range";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s \"ea*\" -w -r \"WELL TIMED\" -l 11,23 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//prefix wildcard before punctuation
Test(student_tests, wildcard_prefix_before_punctuation) {
    char *test_name = "st_wildcard_prefix_before_punctuation";
    prep_files("turing.txt", test_name);
    sprintf(args, "-l 1,22 -s \"intel*\" -w -r wisdom tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//prefix wildcard multiple occurrences
Test(student_tests, wildcard_prefix_multiple_occurrences) {
    char *test_name = "st_wildcard_prefix_multiple_occurrences";
    prep_files("turing.txt", test_name);
    sprintf(args, "-w -s \"Tur*\" -r \"TURING THE GREAT\" tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//prefix wildcard single word file
Test(student_tests, wildcard_prefix_one_word_file) {
    char *test_name = "st_wildcard_prefix_one_word_file";
    prep_files("oneword.txt", test_name);
    sprintf(args, "-s \"Hom*\" -w -r \"This homework is effortless.\" tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//prefix wildcard no match
Test(student_tests, wildcard_prefix_no_match) {
    char *test_name = "st_wildcard_prefix_no_match";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s \"CSE*\" -w -r \"Computer Science\" -l 1,45 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//prefix wildcard no space after punctuation
Test(student_tests, wildcard_prefix_no_space_after_punct) {
    char *test_name = "st_wildcard_prefix_no_space_after_punct";
    prep_files("punctuation.txt", test_name);
    sprintf(args, "-w -s \"Bu*\" -r \" However,\" tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//prefix wildcard exact word match
Test(student_tests, wildcard_prefix_exact_word) {
    char *test_name = "st_wildcard_prefix_exact_word";
    prep_files("turing.txt", test_name);
    sprintf(args, "-s \"with*\" -w -r WITH tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//suffix wildcard whole file
Test(student_tests, wildcard_suffix_whole_file) {
    char *test_name = "st_wildcard_suffix_whole_file";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s \"*re\" -w -r \"New York\" tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//suffix wildcard multiple occurrences
Test(student_tests, wildcard_suffix_multiple_occurrences) {
    char *test_name = "st_wildcard_suffix_multiple_occurrences";
    prep_files("turing.txt", test_name);
    sprintf(args, "-w -s \"*ring\" -r TURING tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//suffix wildcard single line
Test(student_tests, wildcard_suffix_single_line) {
    char *test_name = "st_wildcard_suffix_single_line";
    prep_files("turing.txt", test_name);
    sprintf(args, "-s \"*chine\" -w -r device -l 23,23 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//suffix wildcard within range
Test(student_tests, wildcard_suffix_in_range) {
    char *test_name = "st_wildcard_suffix_in_range";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s \"*rly\" -w -r \"WELL TIMED\" -l 11,23 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//suffix wildcard before punctuation
Test(student_tests, wildcard_suffix_before_punctuation) {
    char *test_name = "st_wildcard_suffix_before_punctuation";
    prep_files("turing.txt", test_name);
    sprintf(args, "-l 1,22 -s \"*gence\" -w -r wisdom tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//suffix wildcard single word file
Test(student_tests, wildcard_suffix_one_word_file) {
    char *test_name = "st_wildcard_suffix_one_word_file";
    prep_files("oneword.txt", test_name);
    sprintf(args, "-w -s \"*ork\" -r \"This homework is effortless.\" tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//suffix wildcard no match
Test(student_tests, wildcard_suffix_no_match) {
    char *test_name = "st_wildcard_suffix_no_match";
    prep_files("unix.txt", test_name);
    sprintf(args, "-s \"*CSE\" -w -r \"Computer Science\" -l 1,45 tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//suffix wildcard no space after punctuation
Test(student_tests, wildcard_suffix_no_space_after_punct) {
    char *test_name = "st_wildcard_suffix_no_space_after_punct";
    prep_files("punctuation.txt", test_name);
    sprintf(args, "-s \"*So\" -w -r \" Therefore,\" tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//suffix wildcard exact word match
Test(student_tests, wildcard_suffix_exact_word) {
    char *test_name = "st_wildcard_suffix_exact_word";
    prep_files("turing.txt", test_name);
    sprintf(args, "-s \"*with\" -w -r WITH tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}

//suffix wildcard word at end of file
Test(student_tests, wildcard_suffix_end_of_file) {
    char *test_name = "st_wildcard_suffix_end_of_file";
    prep_files("punctuation.txt", test_name);
    sprintf(args, "-s \"*ode\" -w -r \"CODE.\" tests.in/%s.in.txt tests.out/%s.out.txt",
            test_name, test_name);
    int status = run_using_system_no_valgrind(test_name, args);
    expect_error_exit(status, 0);
}
