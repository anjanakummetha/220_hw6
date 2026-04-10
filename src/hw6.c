/*
 * Anjana Kummetha
 * 11716283
 */

#include "hw6.h"
#include <limits.h>


//function to replace words if the replacement type is wildcard
static void wildcard_replace_line(FILE *output_file,const char *line,const char *pattern,const char *replace) {
    const char *position = line;
    int pattern_len = (int) strlen(pattern);

    //move until end of line 
    while (*position != '\0') {
        if (isalnum((unsigned char)*position)) {

            const char *word_start = position;

            while (isalnum((unsigned char)*position))
                position++;
            //get word length anf make a copy of the string 
            int word_len = (int) (position - word_start);
            char word[MAX_LINE + 1];

            if (word_len > MAX_LINE) {
                word_len = MAX_LINE;
            }
            strncpy(word, word_start, (size_t)word_len);
            word[word_len] = '\0';

            int is_match = 0;
            
            //prefix case 
            if (pattern[pattern_len - 1] == '*') {

                int prefix_length = pattern_len - 1;

                if (word_len >= prefix_length) {
                    if (strncmp(word, pattern, (size_t)prefix_length) == 0) {
                        is_match = 1;
                    }
                }

            //suffix case 
            } else {
                int suffix_length = pattern_len - 1;
                const char *suffix = pattern + 1;

                if (word_len >= suffix_length){
                    if (strcmp(word + word_len - suffix_length, suffix) == 0) {
                        is_match = 1;
                    }
                }
            }

        //replace for original word and copy everything else 
            if (is_match == 1) {
                fputs(replace, output_file);
            } else {
                fputs(word, output_file);
            }
        } else {

            fputc((unsigned char)*position, output_file);
            position = position + 1;
        }
    }
}


//taking a line of text and input filed every occurrence of search to replaces and outputs
static void simple_replace_line(FILE *output_file, const char *line, const char *search, const char *replace) {
    int search_len = (int) strlen(search);
    const char *position = line;
    const char *found_pos;
    while ((found_pos = strstr(position, search)) != NULL) {
        fwrite(position, 1, (size_t)(found_pos - position), output_file);
        fputs(replace, output_file);
        position = found_pos + search_len;
    }
    //writes to any stream (doesn't append new line)
    fputs(position, output_file);
}





int main(int argc, char *argv[]) {

    //fewer than 7 arguments
    if (argc < 7) {
        return MISSING_ARGUMENT;
    }
    char *search_text  = NULL;
    int s_count = 0;


    char *replace_text = NULL;
    int r_count = 0;

    char *l_arg        = NULL;
    int l_count = 0;

    int   wildcard     = 0;
    int w_count = 0;


    optind = 1; 
    //getting -s, -r, -1, and -w 
    int option;
    while ((option = getopt(argc, argv, "s:r:l:w")) != -1){
        switch (option) {
            case 's':
                search_text = optarg;
                s_count++;
                break;

            case 'r':
                replace_text = optarg;
                r_count++;
                break;

            case 'l':
                l_arg = optarg;
                l_count++;
                break;

            case 'w':
                wildcard = 1;
                w_count++;
                break;

            default:
                break;
        }
    }
    //-s, -r, -1, and -w have appeared more than once -> suplicate argument  
    if (s_count > 1 || r_count > 1  || l_count > 1 || w_count > 1) {
        return DUPLICATE_ARGUMENT;
    }


    //assume argv[argc-2] provides infile and argv[argc-1]
    char *infile  = argv[argc - 2];
    char *outfile = argv[argc - 1];


    //input file is missing or can't be opened
    FILE *input_file = fopen(infile, "r");
    if (input_file == NULL) {
        return INPUT_FILE_MISSING;
    }
    //output file is missing or can't be opened
    FILE *output_file = fopen(outfile, "w");
    if (output_file == NULL) {
        fclose(input_file);
        return OUTPUT_FILE_UNWRITABLE;
    }
    //-s option is missing
    if (search_text == NULL ||  search_text[0] == '-'){
        fclose(input_file);
        fclose(output_file);
        return S_ARGUMENT_MISSING;
    }
    //-r option is missing
    if (replace_text == NULL  || replace_text[0] == '-') {
        fclose(input_file);
        fclose(output_file);

        return R_ARGUMENT_MISSING;
    }
    //one or both of line numbers given to -l is missing/unparseable
    long start_line = 1;
    long end_line   = LONG_MAX;

    if (l_arg != NULL) {

        //actual value is missing 
        if (l_arg[0] == '-') {
            fclose(input_file);
            fclose(output_file);
            return L_ARGUMENT_INVALID;
        }

 
    //make a copy of the string and split from commas 
    char l_copy[MAX_LINE];
    strncpy(l_copy, l_arg, MAX_LINE - 1);
    l_copy[MAX_LINE - 1] = '\0';
    char *token1 = strtok(l_copy, ",");
    char *token2 = NULL;

    //check that values are not NULL and convert to numbers 
    if (token1 != NULL) {
        token2 =  strtok(NULL, ",");
    }
    if (token1 == NULL || token2 == NULL || *token2 == '\0'){
        fclose(input_file);
        fclose(output_file);
        return L_ARGUMENT_INVALID;
       }
    long start_val = strtol(token1, NULL, 10);
    long end_val = strtol(token2, NULL, 10);

    //make sure values are valid 
    if (start_val <= 0 || end_val <= 0 || start_val > end_val) {
        fclose(input_file);
        fclose(output_file);
        return L_ARGUMENT_INVALID;
    }
    start_line = start_val;
    end_line = end_val;
}

    //wildcard is invalid
    if (wildcard) {
        int star_count = 0;
        int str_len = (int) strlen(search_text);
        //check *
        for (int idx = 0; idx < str_len; idx++) {
            if (search_text[idx] == '*') {
                star_count++;
            }
         }
        //check that * is in a valid position 
        int valid_pos = (search_text[0] == '*' || search_text[str_len - 1] == '*');
        if (star_count != 1 || !valid_pos) {
            fclose(input_file);
            fclose(output_file);
            return WILDCARD_INVALID;
        }
    }


    // from start line to end line either do wild card replacement or a substring, copy the lines not between start and end lines 
    char line[MAX_LINE + 2]; 
    //line number 
    long current_line = 1;

    //reading each line and checking range t
    while (fgets(line, (int)sizeof(line), input_file) != NULL) {
        //check if simple replacement or wildcard replacement 
        if ((current_line >= start_line && current_line <= end_line)) {
            if (wildcard) {
                wildcard_replace_line(output_file, line, search_text, replace_text);
            } else{
                simple_replace_line(output_file, line, search_text, replace_text);
            }
        } else {
            fputs(line, output_file);
        }
        current_line++;
    }
    fclose(input_file);
    fclose(output_file);
    return 0;
}