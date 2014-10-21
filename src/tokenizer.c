/*  tokenizer.c - generate tokens from a file pointer.
    Copyright (C) 2014 Renato Alencar <renatoalencar.73@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "caki.h"
#include "tokenizer.h"

#define IGNORE_LINE(s) while(getc(s) != '\n');
#define _ungetc(s) fseek(s, -1, SEEK_CUR)
#define update_position(c, l, cl)  if (c == '\n') { l++; cl = 0; } else cl++;

CakiToken *caki_token_new()
{
	return (CakiToken *) calloc(1, sizeof(CakiToken));
}

void caki_token_free(CakiToken *t)
{
	
	if (t->content != NULL && t->type != T_STRING &&
		 t->type != T_IDENTIFIER)
		free(t->content);

	if (t->next != NULL)
		caki_token_free(t->next);
	free(t);
}

/* Insert a token `new' at end of `head' */
int caki_token_insert(CakiToken *head, CakiToken *new) 
{

	if (head->next == NULL && head->end == NULL) 
		head->next = head->end = new;
	else 
		head->end->next = head->end = new;

	return 0;
}

/* Generate tokens of `stream' */
CakiToken *caki_tokenize(FILE *stream)
{
	char c;
	CakiToken *tokens, *tk;
	int line = 0, column = 0;

	tokens = caki_token_new();

	while ((c = getc(stream)) != EOF) {
		switch (c) {
		case '#':
			/* Ignore comments */
			IGNORE_LINE(stream);
			line++;
			column = 0;
			break;
		case '{':
			tk = caki_token_new();
			tk->type = T_LBRACE;
			tk->line = line;
			tk->column = column;
			caki_token_insert(tokens, tk);
			break;
		case '}':
			tk = caki_token_new();
			tk->type = T_RBRACE;
			tk->line = line;
			tk->column = column;
			caki_token_insert(tokens, tk);
			break;
		case '"':
		{
			int l = line, cl = column;

			tk = caki_token_new();
			tk->type = T_STRING;
			tk->content = caki_token_string(stream, &line, &column);
			tk->line = l;
			tk->column = cl;
			caki_token_insert(tokens, tk);
			break;
		}
		case ';':
			tk = caki_token_new();
			tk->type = T_TERM;
			tk->line = line;
			tk->column = column;
			caki_token_insert(tokens, tk);
			break;
		case '\n':
			line++;
			column = 0;
			break;
		default:
			if (isalpha(c) || c == '_') {
				_ungetc(stream);
				tk = caki_token_identifier(stream, &line,
					 &column);
				caki_token_insert(tokens, tk);
			}
			else if (isdigit(c)) {
				_ungetc(stream);
				tk = caki_token_number(stream, &line, &column);
				caki_token_insert(tokens, tk);
			}
			else if (isspace(c))
				break;
			else {
				printf("Illegal character "
					"(line %d, column %d): '%c'\n",\
					 line, column, c);
				caki_token_free(tokens);
				exit(1);
			}
		}

		if (c != '\n')
			column++;
	}

	tk = tokens->next;
	free(tokens);
	return tk;
}

CakiToken *caki_token_number(FILE *s, int *line, int *column)
{
	CakiToken *out;	/* Token that will be returned */
	char *buff,	/* Temp buffer to store the number */
		c;
	int i, err, /* Error flag */
		l = *line, cl = *column;
	CakiTokenType type = T_INTEGER; /* Type of token, changes to T_FLOAT if we found a dot (.) */

	buff = malloc(32);
	for (i = 0, err = 0; (c = getc(s)) != EOF && (isdigit(c) || c == '.') 
		&& i < 32; i++) {
		if (c == '.') {
			if (type == T_FLOAT) 
				err = 1;
			type = T_FLOAT;
		}
		update_position(c, (*line), (*column));
		buff[i] = c;
	}
	buff[i] = '\0';

	if (err) {
		printf("Unknown number (line %d, column %d): %s\n",\
			*line, *column, buff);
		exit(8);
	}

	_ungetc(s);
	(*column)--;

	out = caki_token_new();
	out->type = type;
	out->content = buff;
	out->line = l;
	out->column = cl;
	return out;
}

CakiToken *caki_token_identifier(FILE *s, int *line, int *column)
{
	char *buff, c;
	int i, l = *line, cl = *column;
	CakiToken *out;

	buff = malloc(256);

	for (i = 0; (c = getc(s)) != EOF && IS_IDENTIFIER(c) && i < 255; i++) {
		update_position(c, (*line), (*column));
		buff[i] = c;
	}
	buff[i] = '\0';

	out = caki_token_new();
	out->type = T_IDENTIFIER;
	out->line = l;
	out->column = cl;
	out->content = buff;
	return out;
}

char caki_read_escape_char(FILE *s, int *line, int *column)
{
	char d = 0, c;

	update_position((c = getc(s)), (*line), (*column));
				
	if (c >= 'a' && c <= 'f')
		d = c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		d = c - 'A' + 10;
	else if (isdigit(c))
		d = c - '0';
	else {
		printf("Unexpected (line "
			"%d, column %d):  0x%c\n",\
			 *line, *column, c);
		exit(7);
	}
	update_position((c = getc(s)), (*line), (*column));
	if (c >= 'a' && c <= 'f')
		d = d*16 + c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		d = d*16 + c - 'A' + 10;
	else if (isdigit(c))
		d = d*16 + c - '0';
	else {
		printf("Unexpected (line "
			"%d, column %d):  0x%c\n",\
			*line, *column, c);
		exit(7);
	}

	return d;
}

char *caki_token_string(FILE *s, int *line, int *column)
{
	char *buff, c;
	int i;

	buff = malloc(1024);

	for (i = 0; (c = getc(s)) != EOF && c != '"' && i < 1023; i++)
	{
		update_position(c, (*line), (*column));

		if (c == '\\') {
			update_position((c = getc(s)), (*line), (*column));

			switch (c) {
			case '\\':
				buff[i] = '\\';
				break;
			case 'n':
				buff[i] = '\n';
				break;
			case 'r':
				buff[i] = '\r';
				break;
			case 't':
				buff[i] = '\t';
				break;
			case 'a':
				buff[i] = '\a';
				break;
			case 'x':
				buff[i] = caki_read_escape_char(s, line, column);
				break;
			default:
				buff[i] = '\\';
				buff[++i] = c;
			}
		}
		else
			buff[i] = c;
	}
	buff[i] = '\0';

	return buff;
}

char *caki_get_token_name(CakiTokenType t)
{
	char *names[] = {
		"Comment",
		"Identifier",
		"Left brace",
		"Right brace",
		"String",
		"Number",
		"Float",
		"Integer",
		"Semicollon (;)"};

	return names[t];
} 
