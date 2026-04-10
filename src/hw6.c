/*
 * Anjana Kummetha
 * 117162837
 */

#include "hw6.h"
#include <limits.h>


//taking a line of text and find every occurrence of search and replaces it and outputs it 
static void simple_replace_line(FILE *fout, const char *line,
                                 const char *search, const char *replace) {
    int slen      = (int)strlen(search);
    const char *pos   = line;
    const char *found;

    while ((found = strstr(pos, search)) != NULL) {
        fwrite(pos, 1, (size_t)(found - pos), fout);
        fputs(replace, fout);
        pos = found + slen;
    }
    //writes to any stream (doesn't append new line)
    fputs(pos, fout);
}


//
/* =========================================================================
 * PART 3 HELPER: wildcard_matches
 *
 * Returns 1 if `word` matches the wildcard `pattern`, 0 otherwise.
 * Pattern has exactly one '*' at the start or end (already validated).
 *
 *   "foo*"  -> prefix match: word must START with "foo"
 *   "*foo"  -> suffix match: word must END   with "foo"
 * ========================================================================= */
static int wildcard_matches(const char *word, const char *pattern) {
    int plen = (int)strlen(pattern);
    int wlen = (int)strlen(word);

    if (pattern[plen - 1] == '*') {
        /* prefix pattern e.g. "foo*" — strip the trailing '*' */
        int prefix_len = plen - 1;
        if (wlen < prefix_len) return 0;
        return strncmp(word, pattern, (size_t)prefix_len) == 0;
    } else {
        /* suffix pattern e.g. "*foo" — skip the leading '*' */
        int suffix_len = plen - 1;
        const char *suffix = pattern + 1;
        if (wlen < suffix_len) return 0;
        return strcmp(word + wlen - suffix_len, suffix) == 0;
    }
}

/* =========================================================================
 * PART 3 HELPER: wildcard_replace_line
 *
 * Writes one line to fout, replacing whole WORDS that match the wildcard
 * pattern with `replace`. Non-matching words and non-word characters
 * (whitespace, punctuation) are copied verbatim.
 *
 * A "word" is a maximal sequence of alphanumeric characters (isalnum()).
 * Word boundaries are whitespace (isspace()), punctuation (ispunct()),
 * or the start/end of the line.
 *
 * How it works:
 *   - Walk character by character through the line
 *   - When we hit an alphanumeric char, collect the entire word
 *   - Check if the word matches the pattern
 *   - If yes: write the replacement; if no: write the original word
 *   - Non-alphanumeric chars are written directly with fputc
 * ========================================================================= */
static void wildcard_replace_line(FILE *fout, const char *line,
                                   const char *pattern, const char *replace) {
    const char *pos = line;

    while (*pos != '\0') {
        if (isalnum((unsigned char)*pos)) {
            /* find the end of this word */
            const char *word_start = pos;
            while (isalnum((unsigned char)*pos)) {
                pos++;
            }

            /* copy the word into a null-terminated buffer */
            int wlen = (int)(pos - word_start);
            char word[MAX_LINE + 1];
            if (wlen > MAX_LINE) wlen = MAX_LINE;
            strncpy(word, word_start, (size_t)wlen);
            word[wlen] = '\0';

            /* replace if it matches, otherwise keep original */
            if (wildcard_matches(word, pattern)) {
                fputs(replace, fout);
            } else {
                fputs(word, fout);
            }
        } else {
            /* whitespace, punctuation, newline — copy as-is */
            fputc((unsigned char)*pos, fout);
            pos++;
        }
    }
}








int main(int argc, char *argv[]) {

    //fewer than 7 arguments
    if (argc < 7) {
        return MISSING_ARGUMENT;
    }

    char *search_text  = NULL;
    char *replace_text = NULL;
    char *l_arg        = NULL;
    int   wildcard     = 0;

    /* track how many times each flag appears to catch duplicates */
    int s_count = 0;
    int r_count = 0;
    int l_count = 0;
    int w_count = 0;

    optind = 1; /* reset getopt */

    int opt;
    while ((opt = getopt(argc - 2, argv, "s:r:l:w")) != -1) {
        switch (opt) {
            case 's':
                s_count++;
                search_text = optarg;
                break;
            case 'r':
                r_count++;
                replace_text = optarg;
                break;
            case 'l':
                l_count++;
                l_arg = optarg;
                break;
            case 'w':
                w_count++;
                wildcard = 1;
                break;
            default:
                /* unknown flags are silently ignored per the spec */
                break;
        }
    }






    //-s, -r, -1, and -w have been provided more than once 
    if (s_count > 1 || r_count > 1 || l_count > 1 || w_count > 1) {
        return DUPLICATE_ARGUMENT;
    }

    char *infile  = argv[argc - 2];
    char *outfile = argv[argc - 1];

    //input file is missing or can't be opened
    FILE *fin = fopen(infile, "r");
    if (fin == NULL) {
        return INPUT_FILE_MISSING;
    }

    //output file is missing or can't be opened
    FILE *fout = fopen(outfile, "w");
    if (fout == NULL) {
        fclose(fin);
        return OUTPUT_FILE_UNWRITABLE;
    }

    //-s option is missing
    if (search_text == NULL || search_text[0] == '-') {
        fclose(fin);
        fclose(fout);
        return S_ARGUMENT_MISSING;
    }

    //-r option is missing
    if (replace_text == NULL || replace_text[0] == '-') {
        fclose(fin);
        fclose(fout);
        return R_ARGUMENT_MISSING;
    }

    //one or both of line numbers given to -l is missing/unparseable
    long start_line = 1;
    long end_line   = LONG_MAX;

    if (l_arg != NULL) {

        /* if l_arg starts with '-' the actual value was missing */
        if (l_arg[0] == '-') {
            fclose(fin);
            fclose(fout);
            return L_ARGUMENT_INVALID;
        }

        /* strtok modifies its input so work on a copy */
        char l_copy[MAX_LINE];
        strncpy(l_copy, l_arg, MAX_LINE - 1);
        l_copy[MAX_LINE - 1] = '\0';

        char *tok1 = strtok(l_copy, ",");
        char *tok2 = strtok(NULL, ",");

        /* missing comma, or nothing after the comma e.g. "20," */
        if (tok1 == NULL || tok2 == NULL || tok2[0] == '\0') {
            fclose(fin);
            fclose(fout);
            return L_ARGUMENT_INVALID;
        }

        start_line = strtol(tok1, NULL, 10);
        end_line   = strtol(tok2, NULL, 10);

        /* strtol returns 0 when it cannot parse at all; also reject <= 0 */
        if (start_line <= 0 || end_line <= 0) {
            fclose(fin);
            fclose(fout);
            return L_ARGUMENT_INVALID;
        }

        if (start_line > end_line) {
            fclose(fin);
            fclose(fout);
            return L_ARGUMENT_INVALID;
        }
    }



    //wildcard is invalid
    if (wildcard) {
        int slen = (int)strlen(search_text);

        int star_count = 0;
        for (int i = 0; i < slen; i++) {
            if (search_text[i] == '*') star_count++;
        }

        if (star_count != 1 ||
            (search_text[0] != '*' && search_text[slen - 1] != '*')) {
            fclose(fin);
            fclose(fout);
            return WILDCARD_INVALID;
        }
    }







    /* -------------------------------------------------------------------
     * PARTS 2 & 3: Process the file
     *
     * Read line by line with fgets.
     * For lines within [start_line, end_line]:
     *   - if -w: do wildcard word replacement
     *   - otherwise: do simple substring replacement
     * For lines outside the range: copy unchanged with fputs.
     * ------------------------------------------------------------------- */
    char line[MAX_LINE + 2]; /* +2 for '\n' and '\0' */
    long current_line = 1;

    while (fgets(line, (int)sizeof(line), fin) != NULL) {

        int in_range = (current_line >= start_line &&
                        current_line <= end_line);

        if (in_range) {
            if (wildcard) {
                wildcard_replace_line(fout, line, search_text, replace_text);
            } else {
                simple_replace_line(fout, line, search_text, replace_text);
            }
        } else {
            fputs(line, fout);
        }

        current_line++;
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

