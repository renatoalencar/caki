/*  parser.c - parses the input and save it into a tree.
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

#include "node.h"
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <stdlib.h>

#include "parser.h"
#include "tokenizer.h"
#include "caki.h"

CakiNode *__caki_parse(FILE *);
CakiNode *caki_parse_statement(CakiToken **);
CakiNode *__caki_parse_node(CakiToken **);
int caki_parser_accept(CakiToken *, CakiTokenType);
void caki_parser_expect(CakiToken **, CakiTokenType);

CakiNode *__caki_parse(FILE *input)
{
	CakiNode *root, *new;
	CakiToken *tk, *node;
	
	root = caki_node_new();
	tk = caki_tokenize(input);
	node = tk;
	
	while (node)
		caki_node_insert(root, caki_parse_statement(&node));

	caki_token_free(tk);

	new = root->next;
	free(root);
	return new;
}

CakiNode *caki_parse_statement(CakiToken **tk)
{
	CakiNode *new = NULL;

	if (caki_parser_accept(*tk, T_IDENTIFIER)) {
		new = caki_node_new();
		new->name = (*tk)->content;
	
		*tk = (*tk)->next;
		if (*tk == NULL) {
			printf("Premature end of file\n");
			exit(1);
		}
		
		if (caki_parser_accept(*tk, T_INTEGER)) {
			int *integer;
			integer = (int *) malloc(sizeof(int));
			*integer = atoi((*tk)->content);
			new->type = TINT;
			new->v_ptr = (void *) integer;
		}
		else if (caki_parser_accept(*tk, T_FLOAT)) {
			float *ft;
			ft = (float *) malloc(sizeof(float));
			*ft = (float) atof((*tk)->content);
			new->type = TFLOAT;
			new->v_ptr = (void *) ft;
		}
		else if (caki_parser_accept(*tk, T_STRING)) {
			new->type = TSTR;
			new->v_ptr = (void *) (*tk)->content;
		}
		else if (caki_parser_accept(*tk, T_LBRACE)) {
			new->type = TNODE;
			new->v_ptr = __caki_parse_node(tk);
			return new;
		}
		else {
			printf("Unexpected (line %d, column %d): %s!\n",\
				(*tk)->line, (*tk)->column,\
				caki_get_token_name((*tk)->type));
			exit(2);
		}

		caki_parser_expect(tk, T_TERM);
	}
	else {
		printf("Unexpected (line %d, column %d): %s!\n",\
                                (*tk)->line, (*tk)->column,\
				caki_get_token_name((*tk)->type));
		exit(3);
	}
	
	return new;
}

CakiNode *__caki_parse_node(CakiToken **tks)
{
	CakiNode *root, *o;
	
	root = caki_node_new();
	
	if (caki_parser_accept(*tks, T_LBRACE)) {
		*tks = (*tks)->next;
		if (*tks == NULL) {
			printf("Premature end of file\n");
			exit(4);
		}
		
		while ((*tks)->type != T_RBRACE)
			caki_node_insert(root, caki_parse_statement(tks));

		*tks = (*tks)->next;
		o = root->next;
		free(root);
		return root->next;
	}
	else {
		printf("Expected Left Brace (line %d, column %d): %s\n",\
			(*tks)->line, (*tks)->column, caki_get_token_name((*tks)->type));
		exit(5);
	}
}

void caki_parser_expect(CakiToken **tk, CakiTokenType t)
{
	*tk = (*tk)->next;
	
	if (*tk == NULL) {
		printf("Premature end of file\n");
		exit(6);
	}
	
	if ((*tk)->type != t) {
		printf("Expected %s, given %s (line %d, column %d)\n",\
			caki_get_token_name(t), caki_get_token_name((*tk)->type),\
			(*tk)->line, (*tk)->column);
		exit(7);
	}
	
	*tk = (*tk)->next;
}

int caki_parser_accept(CakiToken *t, CakiTokenType type)
{
	return (t->type == type);
}

