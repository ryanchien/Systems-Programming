/**
 * Extreme Edge Cases Lab
 * CS 241 - Spring 2019
 */
 
#include "camelCaser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

char **camel_caser(const char *input_str) {
	if (!input_str)
    	return NULL;
	//count number of punctuation marks 
	const char * punct_ptr = input_str;
	int punct_count = 0;
	while (*punct_ptr) {
		if (ispunct(*punct_ptr))
			punct_count++;
		punct_ptr++;
	}

	char ** output = malloc((punct_count+1) * sizeof(char*));
	char * input = malloc(strlen(input_str)+1);
	strcpy(input, input_str);
	char * input_ptr = input;

	char * output_s = malloc(strlen(input_str)+1);
	char punct[] = "!\"#$%&\'()*+,-./:;\?@[\\]^_`{|}~";
	//use buffer to concat char to string
	char char_buff[2];
	char_buff[0] = 'a';
	char_buff[1] = '\0';
	char * token;

	int i = 0;
	while ((token = strsep(&input_ptr, punct)) != NULL) {
		//make sure not to allocate memory for null pointer
		if (i == punct_count)
			break;
		int next_word = 0; //first letter in a word (if not start) should be uppercased
		int first_word = 1; //first letter of first word in a sentence should be lowercased
		int word_found = 0;
		strcpy(output_s, "");
		char * tkn_ptr = token;
		if (strcmp(tkn_ptr,"")!=0) {
			while (*tkn_ptr) {
				if (isspace(*tkn_ptr)) {
					if (word_found == 1) {
						next_word = 1;
						first_word = 0;
					}
				}
				else if (isalpha(*tkn_ptr)) {
					if (first_word==1)
						*tkn_ptr = tolower(*tkn_ptr);
					else if (next_word==1)
						*tkn_ptr = toupper(*tkn_ptr);
					else
						*tkn_ptr = tolower(*tkn_ptr);
					char_buff[0] = *tkn_ptr;
					strcat(output_s, char_buff);
					first_word = 0;
					next_word = 0;
					word_found = 1;
				}
				else {
					char_buff[0] = *tkn_ptr;
					strcat(output_s, char_buff);
					word_found = 1;
				}
				tkn_ptr++;
			}
		}
		output[i] = malloc(strlen(input_str)+1);
		strcpy(output[i], output_s);
		i++;
	}
		
	output[punct_count] = NULL;
	free(input);
	input = NULL;
	free(output_s);
	output_s = NULL;
	return output;
}

void destroy(char **result) {
	char ** ptr = result;
	while (*ptr) {
		free(*ptr);
		*ptr = NULL;
		ptr++;
	}
	free(result);
	result = NULL;
    return;
}
