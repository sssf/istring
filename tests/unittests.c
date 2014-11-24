#include <string.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "../source/istring.h"
#include "../source/istring_rep.h"




int init_suite_1 (void) { return 0; }
int clean_suite_1(void) { return 0; }
int init_suite_2 (void) { return 0; }
int clean_suite_2(void) { return 0; }


void testISTRING_MK(void)
{
    char *str1 = istring_mk(NULL);
    char str2[] = "foo";
    char *str3 = istring_mk(str2);

    CU_ASSERT(str1 == NULL);
    CU_ASSERT(istrlen(str3) == strlen(str2));
    CU_ASSERT_STRING_EQUAL(str2, str3);
    CU_ASSERT(str3[0]  == 'f');
    CU_ASSERT(str3[1]  == 'o');
    CU_ASSERT(str3[2]  == 'o');
    CU_ASSERT(str3[3]  == '\0');
    str3[0]  = 'F';
    CU_ASSERT(strcmp(str2, str3) > 0);

    istring_rm(str1);
    istring_rm(str3);
}

void testISTRING_RM(void)
{
    istring_rm(istring_mk("test"));
}

void testISTRLEN(void)
{
    char *str1 = istring_mk("spam");
    char *str2 = istring_mk("ekieki");

    CU_ASSERT(istrlen(str1) == 4);
    CU_ASSERT(istrlen(str2) == 6);
    START(str2)->length = 3;
    CU_ASSERT(istrlen(str2) == 3);
    CU_ASSERT(strlen(str1) == 4);
    CU_ASSERT(strlen(str2) == 6);

    istring_rm(str1);
    istring_rm(str2);
}

void testISTRING_TO_STRING(void)
{
    char *str1 = istring_mk("spam");
    char *cstr = istring_to_string(str1);

    CU_ASSERT_STRING_EQUAL(cstr, "spam");

    istring_rm(str1);
    free(cstr);
}

void testISTRFIXLEN(void)
{
    char *istr1 = istring_mk("babak");
    char *istr2 = istring_mk("want");
    char *istr3 = istring_mk("awesome");
    char *istr4 = istring_mk("BROCCOLLI");

    CU_ASSERT(istrlen(istr1) == 5);
    istr1[3] = '\0'; // make the string shorter
    CU_ASSERT(istrfixlen(istr1) == 3);

    CU_ASSERT(istrlen(istr2) == 4);
    istr2[2] = (char)1; // insert non-printable ascii character
    CU_ASSERT(istrfixlen(istr2) == 4);

    CU_ASSERT(istrlen(istr3) == 7);
    istr3[7] = 'a'; // remove null-termination
    CU_ASSERT(istrfixlen(istr3) == 7);

    CU_ASSERT(istrlen(istr4) == 9);
    istr4[4] = (char)1; // insert non-printable ascii character
    istr4[9] = 'a';     // remove the null-termination
    CU_ASSERT(istrfixlen(istr4) == 4);

    istring_rm(istr1);
    istring_rm(istr2);
    istring_rm(istr3);
    istring_rm(istr4);

}

void testISTRSLEN(void) {
    char *str0 = istring_mk("babak");
    char *str1 = NULL;
    char *str2 = NULL;

    CU_ASSERT(strlen(str0) == 5);
    CU_ASSERT_PTR_NULL(str1);
    CU_ASSERT_PTR_NULL(str2);

    str1 = istrslen(str0, 7);
    CU_ASSERT(strlen(str0) == 5);   // TODO: This is so stupid!! how can we know if we get a new string or the old one??!!!
    CU_ASSERT(strlen(str1) == 7);
    CU_ASSERT_STRING_EQUAL(str1, "babakkk");

    str2 = istrslen(str0, 3);
    CU_ASSERT(strlen(str0) == 3);   // TODO: this changes the given object! But why??!! specification??
    CU_ASSERT(strlen(str2) == 3);
    CU_ASSERT_STRING_EQUAL(str2, "bab");


    //istring_rm(str2); // TODO: this is not a new string!!!!
    istring_rm(str1);
    istring_rm(str0);
}

void testISTRCAT(void)
{
    char buffer[100];

#define RESET_BUFFER() (memset(buffer, 0, sizeof(char)* 100))

    RESET_BUFFER();
    strcpy(buffer, "Hello ");
    CU_ASSERT_STRING_EQUAL(istrcat(buffer, "World!"), "Hello World!");

    RESET_BUFFER();
    CU_ASSERT_STRING_EQUAL(istrcat(buffer, "Hello World!"), "Hello World!");

    RESET_BUFFER();
    strcpy(buffer, "Hello World!");
    CU_ASSERT_STRING_EQUAL(istrcat(buffer, ""), "Hello World!");

    RESET_BUFFER();
    strcpy(buffer, "");
    CU_ASSERT_STRING_EQUAL(istrcat(buffer, ""), "");

#undef RESET_BUFFER

    //CU_FAIL("Test not implemented yet");

}

void testISTRNCAT(void)
{
    char buffer[100];

#define RESET_BUFFER() (memset(buffer, 0, sizeof(char)* 100))

    RESET_BUFFER();
    strcpy(buffer, "Hello ");
    CU_ASSERT_STRING_EQUAL(istrncat(buffer, "World!", 50), "Hello World!");

    RESET_BUFFER();
    CU_ASSERT_STRING_EQUAL(istrncat(buffer, "Hello World!", 50), "Hello World!");

    RESET_BUFFER();
    strcpy(buffer, "Hello World!");
    CU_ASSERT_STRING_EQUAL(istrncat(buffer, "", 50), "Hello World!");

    RESET_BUFFER();
    strcpy(buffer, "");
    CU_ASSERT_STRING_EQUAL(istrncat(buffer, "", 50), "");

#undef RESET_BUFFER

}

void testISTRCHR(void)
{
    char *str = istring_mk("spam spam bacon spam");

    CU_ASSERT(istrchr(str, 's') == str);
    CU_ASSERT(istrchr(str, 'b') == str + 10);
    CU_ASSERT(istrchr(str, 'c') == str + 12);
    CU_ASSERT(istrchr(str, 's') == str);
    CU_ASSERT(istrchr(str, '\0') == str + 20);    // NOT OK: call to istrRchr here is pure evil!!
    CU_ASSERT(istrchr(str, 'x') == NULL);

    istring_rm(str);
}

void testISTRRCHR(void)
{
    char *str = istring_mk("spam spam bacon spam");

    CU_ASSERT(istrrchr(str, 's') == str + 16);
    CU_ASSERT(istrrchr(str, 'b') == str + 10);
    CU_ASSERT(istrrchr(str, 'c') == str + 12);
    CU_ASSERT(istrrchr(str, 's') == str + 16);
    CU_ASSERT(istrrchr(str, '\0') == str + 20);
    CU_ASSERT(istrrchr(str, 'x') == NULL);

    istring_rm(str);
}

void testISTRCMP(void)
{
    char *str1 = istring_mk("spam");
    char *str2 = istring_mk("spam");
    char *str3 = istring_mk("Spam");

    CU_ASSERT(istrcmp(str1, str1) == 0);
    CU_ASSERT(istrcmp(str1, str2) == 0);
    CU_ASSERT(istrcmp(str2, str3) > 0);
    CU_ASSERT(istrcmp(str3, str2) < 0);

    istring_rm(str1);
    istring_rm(str2);
    istring_rm(str3);
}

void testISTRNCMP(void)
{
    char *str1 = istring_mk("spam ");
    char *str2 = istring_mk("spam");
    char *str3 = istring_mk("Spam");

    CU_ASSERT(istrncmp(str1, str2, 4) == 0);
    CU_ASSERT(istrncmp(str1, str2, 5) > 0);
    CU_ASSERT(istrncmp(str2, str3, 4) > 0);
    CU_ASSERT(istrncmp(str3, str2, 4) < 0);

    istring_rm(str1);
    istring_rm(str2);
    istring_rm(str3);
}

void testISTRCPY(void)
{
    char* istr1  = istring_mk("babak");
    char* istr2  = istring_mk("bannanas are for code-monkeys");
    char* result = NULL;
    char buffer[100];

#define RESET_BUFFER() (memset(buffer, 0, sizeof(char)* 100))

    RESET_BUFFER();
    result = istrcpy(buffer, istr1);
    CU_ASSERT_STRING_EQUAL(istr1, result);
    CU_ASSERT(strlen(istr1) == strlen(result));

    RESET_BUFFER();
    result = istrcpy(buffer, istr2);
    CU_ASSERT(istrcmp(istr2, result) == 0);
    CU_ASSERT(istrlen(istr2) == istrlen(result));

    istring_rm(istr1);
    istring_rm(istr2);

#undef RESET_BUFFER
}

void testISTRNCPY(void)
{
    char* istr1  = istring_mk("babak");
    char* istr2  = istring_mk("bannanas are for code-monkeys");
    char* istr3  = istring_mk("code-monkeys like bannanas");
    char* result = NULL;
    char buffer[100];

#define RESET_BUFFER() (memset(buffer, 0, sizeof(char)* 100))

    RESET_BUFFER();
    result = istrncpy(buffer, istr1, 100);
    CU_ASSERT_STRING_EQUAL(istr1, result);
    CU_ASSERT(istrlen(istr1) == istrlen(result));

    RESET_BUFFER();
    result = istrncpy(buffer, istr2, 100);
    CU_ASSERT_STRING_EQUAL(istr2, result);
    CU_ASSERT(istrlen(istr2) == istrlen(result));

    RESET_BUFFER();
    result = istrncpy(buffer, istr3, 5);
    CU_ASSERT(istrcmp(istr3, result) > 0);
    CU_ASSERT(istrncmp(istr3, result, 5) == 0);
    CU_ASSERT(istrlen(istr3) > istrlen(result));
    CU_ASSERT(istrlen(result) == 5);

    istring_rm(istr1);
    istring_rm(istr2);
    istring_rm(istr3);

#undef RESET_BUFFER
}

int main()
{
    CU_pSuite pSuite1 = NULL;
    CU_pSuite pSuite2 = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suites to the registry */
    pSuite1 = CU_add_suite("Basic Functions Suite", init_suite_1, clean_suite_1);
    if (NULL == pSuite1)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    pSuite2 = CU_add_suite("Advanced Functions Suite", init_suite_2, clean_suite_2);
    if (NULL == pSuite2)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suites */
    if (
            (NULL == CU_add_test(pSuite1, "test of istring_mk()       ", testISTRING_MK)) ||
            (NULL == CU_add_test(pSuite1, "test of istring_rm()       ", testISTRING_RM)) ||
            (NULL == CU_add_test(pSuite1, "test of istring_to_string()", testISTRING_TO_STRING)) ||
            (NULL == CU_add_test(pSuite1, "test of istfixlen()        ", testISTRFIXLEN)) ||
            (NULL == CU_add_test(pSuite1, "test of istslen()          ", testISTRSLEN)) ||
            (NULL == CU_add_test(pSuite1, "test of istrlen()          ", testISTRLEN))
       )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (
            (NULL == CU_add_test(pSuite2, "test of istrcat()          ", testISTRCAT))  ||
            (NULL == CU_add_test(pSuite2, "test of istrncat()         ", testISTRNCAT)) ||
            (NULL == CU_add_test(pSuite2, "test of istrchr()          ", testISTRCHR))  ||
            (NULL == CU_add_test(pSuite2, "test of istrrchr()         ", testISTRRCHR)) ||
            (NULL == CU_add_test(pSuite2, "test of istrcmp()          ", testISTRCMP))  ||
            (NULL == CU_add_test(pSuite2, "test of istrncmp()         ", testISTRNCMP)) ||
            (NULL == CU_add_test(pSuite2, "test of istrcpy()          ", testISTRCPY))  ||
            (NULL == CU_add_test(pSuite2, "test of istrncpy()         ", testISTRNCPY))
       )
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}

