/*
 * string_parser.c
 *
 *  Created on: Nov 25, 2020
 *      Author: gguan, Monil
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_parser.h"

#define _GUN_SOURCE

int count_token (char* buf, const char* delim)
{
	//TODO：
	/*
	*	#1.	Check for NULL string
	*	#2.	iterate through string counting tokens
	*		Cases to watchout for
	*			a.	string start with delimeter
	*			b. 	string end with delimeter
	*			c.	account NULL for the last token
	*	#3. return the number of token (note not number of delimeter)
	*/
	
	if (buf == NULL){
		return 0;
	}
	
	int count = 0;
	
	char *token; char *ptr;
	
	char* copy_buffer = (char *)malloc(sizeof(char) * strlen(buf) + 1);
	strcpy(copy_buffer, buf);	
	
	copy_buffer[strlen(buf)] = '\0';

	token = strtok_r(copy_buffer, delim, &ptr);
	
	while (token != NULL){
		count++;
		token = strtok_r(NULL, delim, &ptr);
	}
	free(copy_buffer);

	return count;
}

command_line str_filler (char* buf, const char* delim)
{
	//TODO：
	/*
	*	#1.	create command_line variable to be filled and returned
	*	#2.	count the number of tokens with count_token function, set num_token. 
    *           one can use strtok_r to remove the \n at the end of the line.
	*	#3. malloc memory for token array inside command_line variable
	*			based on the number of tokens.
	*	#4.	use function strtok_r to find out the tokens 
    *   #5. malloc each index of the array with the length of tokens,
	*			fill command_list array with tokens, and fill last spot with NULL.
	*	#6. return the variable.
	*/
	
	//remove new line
	int j = 0;
	while (buf[j] != '\0'){
		if (buf[j] == '\n'){
			buf[j] = '\0';
			break;
		}
		j++;
	}
	
	command_line cmd_line;
	cmd_line.num_token = count_token(buf, delim);

	cmd_line.command_list = (char**)malloc((cmd_line.num_token + 1) * sizeof(char*));

	char* token;
	char* ptr;
	int i = 0;


	token = strtok_r(buf, delim, &ptr);
	while (token != NULL){
		cmd_line.command_list[i] = (char*)malloc(strlen(token) + 1);
		strcpy(cmd_line.command_list[i], token);
		i++;
		token = strtok_r(NULL, delim, &ptr);
	}

	cmd_line.command_list[i] = NULL;

	return cmd_line;
	


}

void free_command_line(command_line* command)
{
	//TODO：
	/*
	*	#1.	free the array base num_token
	*/

	for (int i = 0; i < command->num_token; i++){
		free(command->command_list[i]);
	}
	free(command->command_list);
	command->command_list = NULL;
	command->num_token = 0;
}
