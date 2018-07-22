/*  tokenizer.h - tokenizer definitions.
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

#include "tape.h"

#ifndef TOKENIZER_H
#define TOKENIZER_H

#define IS_IDENTIFIER(c) (c == '_' || isalnum(c))

typedef enum {
	T_COMMENT,	/* A comment */
	T_IDENTIFIER,	/* A indentifier */
	T_LBRACE,	/* A left brace (begins a sunode) */
	T_RBRACE,	/* A right brace (ends a subnode) */
	T_STRING,	/* A string (") */
	T_NUMBER,	/* A number (Not used yet) */
	T_FLOAT,	/* A float number (Has a dot) */
	T_INTEGER,	/* A integer */
	T_TERM		/* A line terminator (;) */
} CakiTokenType;

struct _cakitoken {
	char *content;			/* Token's content */
	CakiTokenType type;		/* Token's type */
	struct _cakitoken *next;	/* The next token */
	struct _cakitoken *end;		/* The last element of the list */
	int line, column;		/* The line and column of the token */
};

typedef struct _cakitoken CakiToken;

/* Allocate a new token */
CakiToken *caki_token_new();

/* Free memory space of a token list */
void caki_token_free(CakiToken *);

/* Insert a new token in a token list */
int caki_token_insert(CakiToken *, CakiToken *);

/* Generate tokens from a file pointer */
CakiToken *caki_tokenize(CakiTape *);

/* Read a numeric token */
CakiToken *caki_token_number(CakiTape *, int *, int *);

/* Tokenizes a identifier */
CakiToken *caki_token_identifier(CakiTape *, int *, int *);

/* Tokenizes a string */
char *caki_token_string(CakiTape *, int *, int *);

/* Returns the token's name */
char *caki_get_token_name(CakiTokenType );


#endif /* !TOKENIZER_H */
