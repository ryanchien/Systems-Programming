/**
 * Vector Lab
 * CS 241 - Spring 2019
 */
 
#include "sstring.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    // TODO create some tests
	char * str = "Hello! World!!!";
	sstring * sstr = cstr_to_sstring(str);
	assert(sstring_len(sstr) == strlen(str));
	
	vector * v = sstring_vec(sstr);
	assert(vector_size(v) == strlen(str));
	assert(**(char**)(vector_at(v, 0)) == 'H');
	assert(**(char**)(vector_at(v, 1)) == 'e');
	assert(**(char**)(vector_at(v, 2)) == 'l');
	assert(**(char**)(vector_at(v, 3)) == 'l');
	assert(**(char**)(vector_at(v, 4)) == 'o');
	assert(**(char**)(vector_at(v, 5)) == '!');
	assert(**(char**)(vector_at(v, 6)) == ' ');
	assert(**(char**)(vector_at(v, 7)) == 'W');
	assert(**(char**)(vector_at(v, 8)) == 'o');
	assert(**(char**)(vector_at(v, 9)) == 'r');
	assert(**(char**)(vector_at(v, 10)) == 'l');
	assert(**(char**)(vector_at(v, 11)) == 'd');
	assert(**(char**)(vector_at(v, 12)) == '!');
	assert(**(char**)(vector_at(v, 13)) == '!');
	assert(**(char**)(vector_at(v, 14)) == '!');

	char * str1 = sstring_to_cstr(sstr);
	assert(strcmp(str, str1) == 0);

	char * str2 = " Hi..";
	sstring *sstr1 = cstr_to_sstring(str2);
	vector * v0 = sstring_vec(sstr1);
	//printf("v size: %lu, expected size: %lu\n", vector_size(v0), strlen(str2));
	assert(vector_size(v0) == strlen(str2));
	assert(**(char**)(vector_at(v0, 0)) == ' ');
	assert(**(char**)(vector_at(v0, 1)) == 'H');
	assert(**(char**)(vector_at(v0, 2)) == 'i');
	assert(**(char**)(vector_at(v0, 3)) == '.');
	assert(**(char**)(vector_at(v0, 4)) == '.');

	int i = sstring_append(sstr, sstr1);
	assert(i == (int)strlen(str) + (int)strlen(str2));
	assert(i == (int)sstring_len(sstr));

	vector *v1 = sstring_vec(sstr);
	assert((int)vector_size(v1) == i);
	assert(**(char**)(vector_at(v1, 0)) == 'H');
	assert(**(char**)(vector_at(v1, 1)) == 'e');
	assert(**(char**)(vector_at(v1, 2)) == 'l');
	assert(**(char**)(vector_at(v1, 3)) == 'l');
	assert(**(char**)(vector_at(v1, 4)) == 'o');
	assert(**(char**)(vector_at(v1, 5)) == '!');
	assert(**(char**)(vector_at(v1, 6)) == ' ');
	assert(**(char**)(vector_at(v1, 7)) == 'W');
	assert(**(char**)(vector_at(v1, 8)) == 'o');
	assert(**(char**)(vector_at(v1, 9)) == 'r');
	assert(**(char**)(vector_at(v1, 10)) == 'l');
	assert(**(char**)(vector_at(v1, 11)) == 'd');
	assert(**(char**)(vector_at(v1, 12)) == '!');
	assert(**(char**)(vector_at(v1, 13)) == '!');
	assert(**(char**)(vector_at(v1, 14)) == '!');
	assert(**(char**)(vector_at(v1, 15)) == ' ');
	assert(**(char**)(vector_at(v1, 16)) == 'H');
	assert(**(char**)(vector_at(v1, 17)) == 'i');
	assert(**(char**)(vector_at(v1, 18)) == '.');
	assert(**(char**)(vector_at(v1, 19)) == '.');

	vector * split = sstring_split(sstr, '!');
	assert(vector_size(split) == 5);
	//printf("str: \"%s\"\n", (char*)(vector_get(split, 1)));
	assert(strcmp((char*)(vector_get(split, 0)), "Hello") == 0);
	assert(strcmp((char*)(vector_get(split, 1)), " World") == 0);
	assert(strcmp((char*)(vector_get(split, 2)), "") == 0);
	assert(strcmp((char*)(vector_get(split, 3)), "") == 0);
	assert(strcmp((char*)(vector_get(split, 4)), " Hi..") == 0);

	vector * split1 = sstring_split(sstr, ' ');
	assert(vector_size(split1) == 3);
	//printf("str: \"%s\"\n", (char*)(vector_get(split, 1)));
	assert(strcmp((char*)(vector_get(split1, 0)), "Hello!") == 0);
	assert(strcmp((char*)(vector_get(split1, 1)), "World!!!") == 0);
	assert(strcmp((char*)(vector_get(split1, 2)), "Hi..") == 0);

	sstring_destroy(sstr);
	sstring_destroy(sstr1);
	vector_destroy(split);
	vector_destroy(split1);
	free(str1);


	sstring * sstr2 = cstr_to_sstring("012345678");
	char * slice = sstring_slice(sstr2, 0, 9);
	char * slice1 = sstring_slice(sstr2, 2, 5);
	assert(strcmp(slice, "012345678")==0);
	assert(strcmp(slice1, "234")==0);

	sstring_destroy(sstr2);
	free(slice);
	free(slice1);

	sstring * sstr3 = cstr_to_sstring("0123456789");
	int subs_out = sstring_substitute(sstr3, 0, "abc", "cba");
	assert(subs_out==-1);
	subs_out = sstring_substitute(sstr3, 3, "123", "321");
	assert(subs_out==-1);
	subs_out = sstring_substitute(sstr3, 4, "456", "445566");
	assert(subs_out==0);
	assert(sstring_len(sstr3) == 13);
	char * subst = sstring_to_cstr(sstr3);
	assert(strcmp(subst, "0123445566789")==0);
	
	subs_out = sstring_substitute(sstr3, 5, "445566", "456");
	assert(subs_out==-1);
	subs_out = sstring_substitute(sstr3, 4, "445566", "456");
	assert(subs_out==0);
	assert(sstring_len(sstr3) == 10);
	char * subst1 = sstring_to_cstr(sstr3);
	assert(strcmp(subst1, "0123456789")==0);

	sstring_destroy(sstr3);
	free(subst);
	free(subst1);
	
	sstring *replace_me = cstr_to_sstring("This is a {} day, {}!");
	int sub = sstring_substitute(replace_me, 18, "{}", "friend");
	assert(sub == 0);
	assert(sstring_len(replace_me) == 25);
	char * rep = sstring_to_cstr(replace_me);
	assert(strcmp(rep, "This is a {} day, friend!")==0);
	sub = sstring_substitute(replace_me, 0, "{}", "good");
	assert(sub == 0);
	assert(sstring_len(replace_me) == 27);
	char * rep1 = sstring_to_cstr(replace_me);
	assert(strcmp(rep1, "This is a good day, friend!")==0);
	sub = sstring_substitute(replace_me, 0, "friend!", "friend!!!");
	assert(sub == 0);
	char * rep2 = sstring_to_cstr(replace_me);
	assert(strcmp(rep2, "This is a good day, friend!!!")==0);
	sstring_destroy(replace_me);
	free(rep);
	free(rep1);
	free(rep2);

	sstring * hello = cstr_to_sstring("HELLIO!");
	int h = sstring_substitute(hello, 0, "I", "");
	assert(h == 0);
	assert(sstring_len(hello) == 6);
	char * chello = sstring_to_cstr(hello);
	assert(strcmp(chello, "HELLO!")==0);

	sstring * world = cstr_to_sstring(" WORLD!");
	int helloworld = sstring_append(hello, world);
	assert(helloworld == 13);
	char * hw = sstring_to_cstr(hello);
	assert(strcmp(hw, "HELLO! WORLD!")==0);
	sstring_destroy(hello);
	sstring_destroy(world);
	free(chello);
	free(hw);

	sstring * please = cstr_to_sstring("Please\twork...");
	int p = sstring_substitute(please, 0, "\t", "\n ");
	assert(p == 0);
	char * pp = sstring_to_cstr(please);
	assert(strcmp(pp, "Please\n work...")==0);
	p = sstring_substitute(please, 0, "Please", "1Please ");
	assert(p == 0);
	char * ppp = sstring_to_cstr(please);
	assert(strcmp(ppp, "1Please \n work...")==0);
	sstring_destroy(please);
	free(pp);
	free(ppp);

	puts("SUCCESS");
    return 0;
}
