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

#ifndef __MACH__
#include <malloc.h>
#endif

#include <stdlib.h>

#include "parser.h"
#include "tokenizer.h"
#include "caki.h"

/* Parse the file pointer input */
CakiNode *__caki_parse(FILE *input)
{
	CakiNode *root, *new;
	CakiToken *tk, *node;
	
	root = caki_node_new(); // Create a new node
	tk = caki_tokenize(input); // Generate tokens
	node = tk;
	
	while (node)
		caki_node_insert(root, caki_parse_statement(&node));

	caki_token_free(tk);

	new = root->next;
	free(root);
	return new;
}

/* Parse a statement, a pair <key> <value>; */
CakiNode *caki_parse_statement(CakiToken **tk)
{
	CakiNode *new = NULL;

	if (caki_parser_accept(*tk, T_IDENTIFIER)) {
		/* It's a indentifier */
		new = caki_node_new();
		new->name = (*tk)->content;
	
		*tk = (*tk)->next;
		if (*tk == NULL) {
			printf("Premature end of file\n");
			exit(1);
		}
		

		/* Parse the value */

		if (caki_parser_accept(*tk, T_INTEGER)) {
			/* It's a integer */
			int *integer;

			integer = (int *) malloc(sizeof(int));
			*integer = atoi((*tk)->content);
			new->type = CAKI_TYPE_INT;
			new->v_ptr = (void *) integer;
		}
		else if (caki_parser_accept(*tk, T_FLOAT)) {
			/* It's a float */
			float *ft;

			ft = (float *) malloc(sizeof(float));
			*ft = (float) atof((*tk)->content);
			new->type = CAKI_TYPE_FLOAT;
			new->v_ptr = (void *) ft;
		}
		else if (caki_parser_accept(*tk, T_STRING)) {
			/* It's a string */
			new->type = CAKI_TYPE_STR;
			new->v_ptr = (void *) (*tk)->content;
		}
		else if (caki_parser_accept(*tk, T_LBRACE)) {
			/* It's a left brace ({).
			 * It indicates the begin of a subnode.
			*/
			new->type = CAKI_TYPE_NODE;
			new->v_ptr = __caki_parse_node(tk);
			return new;
		}
		else {
			/* ERROR - NOT EXPECTED */
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

/* Parse a node, <key> { ... } */
CakiNode *__caki_parse_node(CakiToken **tks)
{
	CakiNode *root;
	
	root = caki_node_new();
	
	if (caki_parser_accept(*tks, T_LBRACE)) {
		/* Parse the subnode */

		CakiNode *tmp;

		*tks = (*tks)->next;

		if (*tks == NULL) {
			printf("Premature end of file\n");
			exit(4);
		}
		
		/* Parse a statement until find a right brace */
		while ((*tks)->type != T_RBRACE)
			caki_node_insert(root, caki_parse_statement(tks));

		*tks = (*tks)->next;
		tmp = root->next;
		free(root);
		return tmp;
	}
	else {
		printf("Expected Left Brace (line %d, column %d): %s\n",\
			(*tks)->line, (*tks)->column, caki_get_token_name((*tks)->type));
		exit(5);
	}
}


/* Expects a CakiTokenType `t' or report error else */
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

/* Returns if the token is of the `type' given */
int caki_parser_accept(CakiToken *t, CakiTokenType type)
{
	return (t->type == type);
}

