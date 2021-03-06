/*  caki.c - caki main routines.
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
#include <string.h>

#include "caki.h"
#include "node.h"
#include "parser.h"
#include "tape.h"

CakiNode *caki_parse(void *buffer, size_t length)
{
	CakiNode *out;
	CakiTape *tape;

	if (buffer == NULL)
		return NULL;

	tape = caki_tape_new(buffer, length);

	out = __caki_parse(tape);

	caki_tape_free(tape);

	return out;
}

float caki_get_float(char *code, CakiNode *root)
{
	CakiNode *n;

	n = caki_get_node(code, root);

	if (n == NULL || n->v_ptr == NULL)
		return 0.0;

	if (n->type == CAKI_TYPE_FLOAT)
		return *((float *) n->v_ptr);

	return 0.0;
}

int caki_get_int(char *code, CakiNode *root)
{
	CakiNode *n;

	n = caki_get_node(code, root);

	if (n == NULL || n->v_ptr == NULL)
		return 0;

	if (n->type == CAKI_TYPE_INT)
		return *((int *) n->v_ptr);

	return 0;
}

void *caki_get_ptr(char *code, CakiNode *root)
{
	CakiNode *n;

	n = caki_get_node(code, root);

	if (n != NULL)
		return n->v_ptr;

	return NULL;
}

CakiNode *caki_get_node(char *code, CakiNode *root)
{
	CakiNode *node;
	char node_name[256];
	int i;

	/* Find the name of the node; until find a dot (.) or the end
	 * of the string. The dot indicates that it's trying to find
	 * a subnode. 
	 */
	for (i = 0; code[i] != '\0' && code[i] != '.' && i < 256; i++)
		node_name[i] = code[i];
	node_name[i] = '\0';

	/* Search for node with the name given */
	for (node = root; node != NULL; node = node->next) {
		if (strcmp(node->name, node_name) == 0)
			break;
	}

	/* Node not found */
	if (node == NULL)
		return NULL;

	/* When a dot is found search for a subnode */
	if (code[i] == '.' && node->type == CAKI_TYPE_NODE)
		return caki_get_node(&code[i+1], (CakiNode *) node->v_ptr);
	else
		return node;
}

char *caki_get_str(char *code, CakiNode *root)
{
	return (char *) caki_get_ptr(code, root);
}

void caki_free(CakiNode *root)
{
	if (root == NULL)
		return;

	caki_node_free(root);
}
