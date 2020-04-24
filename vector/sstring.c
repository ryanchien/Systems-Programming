/**
 * Vector Lab
 * CS 241 - Spring 2019
 */
 
#include "sstring.h"
#include "vector.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <assert.h>
#include <string.h>

struct sstring {
    // Anything you want
	vector * v;
	size_t len;
};

/*
//TODO: comment out helper func before committing
vector * sstring_vec(sstring *this) {
	assert(this);
	return this->v;
}

//TODO: comment out helper func before committing
size_t sstring_len(sstring *this) {
	assert(this);
	return this->len;
}
*/

sstring *cstr_to_sstring(const char *input) {
    // your code goes here
	if (input == NULL)
		return NULL;
	sstring * s = malloc(sizeof(sstring));
	s->v = vector_create(string_copy_constructor, string_destructor, string_default_constructor);
	const char * str_ptr = input;
	char * ch_buff = malloc(sizeof(char)+1);
	ch_buff[0] = ' ';
	ch_buff[1] = '\0';
	while (*str_ptr) {
		ch_buff[0] = *str_ptr;
		vector_push_back(s->v, (void*)ch_buff);
		str_ptr++;
	}
	s->len = strlen(input);
	free(ch_buff);
    return s;
}

char *sstring_to_cstr(sstring *input) {
    // your code goes here
	assert(input);
	if (input->len == 0)
		return "";
	char * str = malloc(input->len + 1);
	strcpy(str, "");
	for (size_t i=0; i<input->len; i++) {
		char * ch_ptr = vector_get(input->v, i);
		str = strcat(str, ch_ptr);
	}
    return str;
}

int sstring_append(sstring *this, sstring *addition) {
    // your code goes here
	assert(this);
	assert(addition);
	for (size_t i=0; i<addition->len; i++) {
		vector_push_back(this->v, vector_get(addition->v, i)); 
		this->len++;
	}
    return this->len;
}

vector *sstring_split(sstring *this, char delimiter) {
    // your code goes here 
	assert(this);
	assert(delimiter != '\0');
	//create vector of strings
	vector * str_v = vector_create(string_copy_constructor, string_destructor, string_default_constructor);

	//count number of delimiters in sstring
	size_t count = 1;
	for (size_t i=0; i<this->len; i++) {
		char * ch_ptr = vector_get(this->v, i);
		if (*ch_ptr == delimiter)
			count++;
	}

	if (count == 1)
		vector_push_back(str_v, sstring_to_cstr(this));
	else {
		vector_resize(str_v, count);
		size_t v_idx = 0;
		for (size_t i=0; i<this->len; i++) {
			assert(v_idx < count);
			char * ch_ptr = vector_get(this->v, i);
			if (*ch_ptr != delimiter) {
				char * curr_str = vector_get(str_v, v_idx);
				char * new_str = malloc(strlen(curr_str)+2);
				strcpy(new_str, curr_str);
				new_str = strcat(new_str, ch_ptr);
				vector_set(str_v, v_idx, new_str);
				free(new_str);
			}
			else {
				v_idx++;
			}
		}
	}

    return str_v;
}

int sstring_substitute(sstring *this, size_t offset, char *target,
                       char *substitution) {
    // your code goes here
	assert(this);
	assert(offset < this->len);
	assert(target);
	assert(strcmp(target, "")!=0);
	assert(substitution);

	// find target
	int t_idx = -1;
	size_t t_len = strlen(target);
	for (size_t i=offset; i<this->len; i++) {
		char * ch_ptr = vector_get(this->v, i);
		if (*ch_ptr == *target && (i+t_len)<=this->len) {
			char * slice = sstring_slice(this, i, i+t_len);
			if (strcmp(slice, target)==0)
				t_idx = i;
			free(slice);
		}
	}

	//target not found
	if (t_idx == -1)
    	return -1;
	//erase target
	for (size_t i=0; i<t_len; i++) {
		vector_erase(this->v, t_idx);
		this->len--;
	}
	//insert substitution
	size_t s_len = strlen(substitution);
	char * s_ptr = substitution;
	char * ch_buff = malloc(sizeof(char)+1);
	ch_buff[0] = ' ';
	ch_buff[1] = '\0';
	for (size_t i=0; i<s_len; i++) {	
		ch_buff[0] = s_ptr[i];
		if (t_idx+i == this->len)//can't insert after last index ->pushback instead
			vector_push_back(this->v, ch_buff);
		else
			vector_insert(this->v, t_idx+i, ch_buff);
		this->len++;
	}
	free(ch_buff);
	return 0;
}

char *sstring_slice(sstring *this, int start, int end) {
    // your code goes here
	assert(this);
	assert(start >= 0 && start <= end && end <= (int)this->len);
	char * str = malloc(end - start + 1);
	strcpy(str, "");
	for (int i=start; i<end; i++) {
		char * substr = vector_get(this->v, (size_t)i);
		str = strcat(str, substr);
	}
    return str;
}

void sstring_destroy(sstring *this) {
    // your code goes here
	assert(this);
	vector_destroy(this->v);
	free(this);
	this = NULL;
}
