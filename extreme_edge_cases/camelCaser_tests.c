/**
 * Extreme Edge Cases Lab
 * CS 241 - Spring 2019
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "camelCaser.h"
#include "camelCaser_tests.h"

/*
 * added helper function to test input vs expected output
 */
int test_case(char ** output, char ** exp_out, int entries) {
	for (int i=0; i<entries; i++) {
		//printf("i: %d\n", i); 
		if ((output[i]==NULL && exp_out[i]==NULL)) {
			//printf("CORRECT:  ");
			//puts("OUT IS NULL, EXP IS NULL");
		}
		else if ((output[i]==NULL && exp_out[i]!=NULL)) {
			//puts("FAILED: OUT IS NULL, EXP IS NOT");
			return 0;
		}
		else if ((output[i]!=NULL && exp_out[i]==NULL)) {
			//puts("FAILED: EXP IS NULL, OUT IS NOT");
			return 0;
		}
		else if (strcmp(output[i], exp_out[i]) != 0) {
			/*
			printf("WRONG STRING:  ");
			printf("out: \"%s\" exp: \"%s\"\n", output[i], exp_out[i]);
			printf("%lu %lu\n", strlen(output[i]), strlen(exp_out[i]));
			for (size_t j=0; j<strlen(output[i]); j++) {
				printf("\'%c\' ", output[i][j]);
			}
			puts("");
			for (size_t j=0; j<strlen(exp_out[i]); j++) {
				printf("\'%c\' ", exp_out[i][j]);
			}
			puts("");
			*/
			return 0;
		}
		/*
		else {
			printf("CORRECT:  ");
			printf("out: \"%s\" exp: \"%s\"\n", output[i], exp_out[i]);
		}
		*/
		
	}
	//puts("");
	return 1;
}

/*
 * Testing function for various implementations of camelCaser.
 *
 * @param  camelCaser   A pointer to the target camelCaser function.
 * @param  destroy      A pointer to the function that destroys camelCaser
 * output.
 * @return              Correctness of the program (0 for wrong, 1 for correct).
 */
int test_camelCaser(char **(*camelCaser)(const char *),
                    void (*destroy)(char **)) {
    // TODO: Return 1 if the passed in function works properly; 0 if it doesn't.

	char * exp0[]  = {"hello", "world1234", "", NULL};
	char * exp1[]  = {"", "", "", "", "", NULL};
	char * exp2[] = {NULL};
	char * exp3[] = {"", "ahello", "\ahello", "\aHello", "hello", NULL};
	char * exp4[] = {"\ahelloWorldHelloWorld", "", "1234helloworld", NULL};
	char * exp5[] = {"superlongstring1234567890superlongstring1234567890superlongstring1234567890", "helloWorld", NULL};
	char * exp6[] = {"","","","","","","","","","","a","","a",NULL};
	char * exp7[] = {"","1","2","3","4","5","6aaa\5\6\7h3110W0r1d","","8",NULL};
	char * exp8[] = {"1234hello4321World", "1234hello4321world", NULL};
	char * exp9[] = {"\1\2\3\4\5\6\7\e\a\bHelloWorld", NULL};
	char * exp10[] = {"\0011HelloWorld","\002\002WorldHello","3\003Wellohorld","",NULL};
	char * exp11[] = {"\20helloWorld", "worldHello", NULL};

	char ** out0 = camelCaser("Hello . WoRLD	 1234 !!");
	//printf("input: \"%s\"\n", "Hello . WoRLD	 1234 !!");
	if (test_case(out0, exp0, 4)==0) return 0;
	destroy(out0);

	char ** out1 = camelCaser(".....");
	//printf("input: \"%s\"\n", ".....");
	
	if (test_case(out1, exp1, 6)==0) return 0;
	destroy(out1);

	char ** out2 = camelCaser("Hello \0 World. Hi . ."); 
	//printf("input: \"%s\"\n", "Hello \0 World. Hi . .");
	if (test_case(out2, exp2, 1)==0) return 0;
	destroy(out2);

	char ** out3 = camelCaser("\\aHELLO. \aHELLO. \a HELLO. HELLO");
	//printf("input: \"%s\"\n", "\\aHello. \aHello.");
	if (test_case(out3, exp3, 4)==0) return 0;
	destroy(out3);

	char ** out5 = camelCaser("\aHello world HELLO wORlD. \n\t\v\f\r. 1234HELLOWORLD \n.");
	//printf("input: \"%s\"\n", "\aHello world HELLO wORlD. \n\t\v\f\r. 1234HELLOWORLD \n.");
	if (test_case(out5, exp4, 4)==0) return 0;
	destroy(out5);
	
	char ** out6 = camelCaser("");
	//printf("input: \"%s\"\n", "" );
	if (test_case(out6, exp2, 1)==0) return 0;
	destroy(out6);

	char ** out7 = camelCaser("SUPERLONGSTRING1234567890SUPERLONGSTRING1234567890SUPERLONGSTRING1234567890 . helLO woRLD !");
	//printf("input: \"%s\"\n", "SUPERLONGSTRING1234567890SUPERLONGSTRING1234567890SUPERLONGSTRING1234567890 . helLO woRLD !");
	if (test_case(out7, exp5, 3)==0) return 0;
	destroy(out7);

	char ** out8 = camelCaser("!@#$%^\"&*(a,{a.");
	//printf("input: \"%s\"\n", "!@#$%^\"&*(a,{a.");
	if (test_case(out8, exp6, 12)==0) return 0;
	destroy(out8);

	char ** out9 = camelCaser(",1/2;3\'4(5]6aaa\5\6\7h3110 w0r1d,{8}9");
	//printf("input: \"%s\"\n", ",1/2;3\'4(5]6aaa\5\6\7h3110 w0r1d,{8}9");
	if (test_case(out9, exp7, 10)==0) return 0;
	destroy(out9);

	char ** out10 = camelCaser("1234hello 4321world. 1234HELLO4321WORLD.");
	//printf("input: \"%s\"\n", "1234hello 4321world. 1234HELLO4321WORLD.");
	if (test_case(out10, exp8, 3)==0) return 0;
	destroy(out10);

	char ** out11 = camelCaser("\1\2\3\4\5\6\7\e\r\t\a\f\v\b\nhello world.");
	//printf("input: \"%s\"\n", "\1\2\3\4\5\6\7\e\r\t\a\f\v\b\nhello world.");
	if (test_case(out11, exp9, 2)==0) return 0;
	destroy(out11);

	char ** out12 = camelCaser("\1 1hello world. \2\t\2WORLD\vHELLO@ 3\n \3 wELLOHORLD@@");
	//printf("input: \"%s\"\n", "\1 1hello world. \2\t\2WORLD\vHELLO@ 3\n \3 wELLOHORLD@@");
	if (test_case(out12, exp10, 5)==0) return 0;
	destroy(out12);

	char ** out13 = camelCaser("\20hello world. \40 world hello!");
	//printf("input: \"%s\"\n", "\20hello world. \40 world hello!");
	if (test_case(out13, exp11, 3)==0) return 0;
	destroy(out13);

	return 1;
}
