#include "istring.h"
#include "istring_rep.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

// TODO: Make sure we use uint32_t instead of plain int


IString* istring_alloc(size_t length) {
    assert(length >= 0);
    size_t size = sizeof(IString) + (sizeof(char) * (length + 1));
    IString *istring = malloc(size);

    if (istring != NULL) {
        memset(istring, 0, size);
    }

    return istring;
}

/*
 * Returns a new istring instance created from supplied
 * string. Returns NULL if out of memory. Returns NULL string if
 * argument str is NULL.
 */
char *istring_mk(const char* cstring) {

    if (cstring == NULL) {
        return NULL;
    }

    size_t cstring_length = strlen(cstring);

    // create new istring
    IString *istring = istring_alloc(cstring_length);


    if (istring == NULL) {
        return NULL;
    }

    istring->length = cstring_length;

    strcpy(istring->string, cstring);
    istring->string[cstring_length] = '\0';

    return STRING(istring);
}

/*
 * Deallocates the supplied istring.
 */
void istring_rm(char *string) {
    if (string == NULL) {
        return;
    }
    free(START(string));
}

/*
 * Returns a standard null terminated char* representation of the
 * supplied istring. Returns NULL if out of memory.
 */
char *istring_to_string(const char *string) {
    if (string == NULL) {
        return NULL;
    }

    IString *istring = START(string);

    size_t size = (istring->length + 1) * sizeof(char);

    char *cstring = malloc(size);

    if (cstring == NULL) {
        return NULL;
    }

    memset(cstring, 0, size);
    strcpy(cstring, istring->string);

    return cstring;
}

/*
 * Inspects the length of the string from str and if it differs from
 * length, updates length accordingly. If str does not contain a \0
 * token, it will be inserted at the first element which is not a
 * printable ascii character, and length updated accordingly. This
 * function must be as efficient as possible and not rescan the string
 * if it can be avoided.
 *
 * This function is useful when istrings have been manipulated as
 * regular C strings, to reestablish the length invariant.
 */
// TODO: add tests for this!!!
size_t istrfixlen(char *string) {
    assert(string != NULL);

    IString *istring = START(string);

    int length = istring->length;
    uint32_t first_non_printable = length;

    for (uint32_t n=0; n<=length; ++n) {

        if (istring->string[n] < (char)32 && n < first_non_printable) {
            first_non_printable = n;
        }
        if (istring->string[n] == '\0') {
            istring->length = n;
            return n;
        }
    }

    istring->string[first_non_printable] = '\0';
    istring->length = first_non_printable;
    //istring_set_length(istr, first_non_printable);

    return istring->length;
}


/*
 * Sets the length of an istring and inserts a corresponding '\0'
 * character. If the length argument is out of bounds for the string,
 * a new istring should be returned with the contents of the original
 * string. The last character of the original string will be repeated
 * to fill the string to its given length.
 * NOTE: if istr is NULL return NULL
 */
// FIXME: we have to change the acual object!!!
char* istrslen(char *string, size_t length) {
    assert(length >= 0);

    if (string == NULL) {
        return NULL;
    }

    IString *istring = START(string);

    size_t old_length = istring->length;

    // new length <= old length
    if (length <= old_length) {
        //istring_set_length(istr, length);
        istring->length = length;
        istring->string[length] = '\0';
        return STRING(istring);
    }

    // new length > old length
    char new_string[length+1];
    for (uint32_t n=0; n < length; ++n) {
        if (n < old_length) {
            new_string[n] = istring->string[n];
        }
        else {
            new_string[n] = istring->string[old_length-1];
        }
    }
    // NULL terminate the string
    new_string[length] = '\0';

    return istring_mk(new_string);
}

/*
 * For definitions, see the manual page of respective function on the
 * solaris system and the assignment text.
 *
 * Think about whether having O(1) access to the length can be used to
 * improve the implementations if the string.h equivalents and use
 * that to guide your implementations!
 */
char *istrchr(const char *string, int c) {
    assert(string != NULL);

    IString *istring = START(string);

    int n = 0;
    int length = istring->length;
    while (n <= length) {
        if (istring->string[n] == c) {
            return (char*)&(istring->string[n]);
        }
        ++n;
    }
    return NULL;
}
// TODO: YES O(1) access to the length helps
char *istrrchr(const char *string, int c) {
    assert(string != NULL);

    IString *istring = START(string);

    int n = istring->length;
    while (n >= 0) {
        if (istring->string[n] == c) {
            return (char*)&(istring->string[n]);
        }
        --n;
    }
    return NULL;
}

// The O(1) access to length doesn't help here
int istrcmp(const char *string1, const char *string2) {
    assert(string1 != NULL);
    assert(string2 != NULL);

    return strcmp(string1, string2);
}

int istrncmp(const char *string1, const char *string2, size_t n) {
    assert(string1 != NULL);
    assert(string2 != NULL);

    return strncmp(string1, string2, n);
}

/* TODO: fix documentation
 * istrlen - calculate the length of a string
 * The function calculates the length of the string istr, excluding the terminating null byte ('\0').
 * returns the number of bytes in the string istr.
 */
size_t istrlen(const char *string) {
    assert(string != NULL);
    IString *istring = START(string);

    return istring->length;
}

/*
 * I nedanstående funktioner är dst en pekare till en vanlig
 * sträng och inte en istring. Däremot skall minnesutrymmet
 * "konverteras" till en istring av funktionerna, d.v.s. efter att
 * t.ex. istrcpy anropats bör man vid anropsplatsen göra dst =
 * STRING(dst) för att hoppa över längd-delen av strängen.
 */
char *istrcpy(char *destination, const char *source) {

    IString *istring = (IString*)destination;
    strcpy(istring->string, source);
    istring->length = strlen(source);

    return STRING(istring);
}

char *istrncpy(char *destination, const char *source, size_t n) {

    IString *istring = (IString*)destination;
    strncpy(istring->string, source, n);

    size_t source_length = strlen(source);

/* TODO: ask about milos opinion on this!
//====================================================
//====================================================

    if (source_length > n) {
        source_length = n;
    }

    istring->length = source_length;

//====================================================

    istring->length = source_length;

    if (source_length > n) {
        istring->length = n;
    }


//====================================================

    if (source_length > n) {
        istring->length = n;
    } else {
        istring->length;
    }
*/
//====================================================

    istring->length = (source_length > n) ? n : source_length;

//====================================================
//====================================================
    return STRING(istring);
}




IString *cstring_to_istring(char *cstring) {
    size_t cstring_length = strlen(cstring);
    for (int n = cstring_length; n >= 0; --n) {
        cstring[n+4] = cstring[n];
    }

    return ((IString*)cstring);
}


// NOTE: make sure your assertions are correct!!!!
char *istrcat(char *destination, const char *source) {

    IString *istring = cstring_to_istring(destination);


    strcat(istring->string, source);
    istring->length = strlen(istring->string);
    return STRING(istring);
}


char *istrncat(char *destination, const char *source, size_t n) {

    IString *istring = cstring_to_istring(destination);

    strncat(istring->string, source, n);
    istring->length = strlen(istring->string);

    return STRING(istring);
}

