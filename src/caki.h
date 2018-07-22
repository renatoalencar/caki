/*  caki.h - caki main routines definitions.
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

#include <stddef.h>

#ifndef _CAKI_H
#define _CAKI_H

typedef enum {
	CAKI_TYPE_INT,
	CAKI_TYPE_STR,
	CAKI_TYPE_NODE,
	CAKI_TYPE_FLOAT
} CakiNodeType;

/* The Node 
 *
 * Node is a linked list with the name, type and value of
 * itself.
 *
 * Each node is a definition in the file parsed, and when
 * it has a subnode it is stored in `v_ptr', and you can
 * access the node using `caki_get_node' function, or
 * directly, eg.:
 *	node = (CakiNode *) n->v_ptr;
 *
 * Node points to the next node in `next', and ends
 * the list with `next' pointing to `NULL'.
 *
 * `end' points to the end of the list, for quickly
 * insertion to the end of the list.
 */

struct _cakinode {
	char *name;		/* The node name */
	void *v_ptr;		/* The pointer to the node value */
	CakiNodeType type; 	/* The value type */
	struct _cakinode *next;	/* Pointer to next element in the list */
	struct _cakinode *end;	/* Pointer to last element in the list */
};

typedef struct _cakinode CakiNode;



/* Parse the given filename and returns the tree with the values 
 *
 * filename: Recepts the filename with the definitions.
 */
CakiNode *caki_parse(void *buffer, size_t length);



/* Search for the node name given and returns the value as a integer 
 *
 * node_name:	The name of the node that you are searching.
 * root:	The root of definitions, it can be the value returned by
 *		`caki_parse'.
 *
 * The params above are also valid for:
 * 	`caki_get_float', `caki_get_node', `caki_get_ptr', `caki_get_str'.
 */
int caki_get_int(char *node_name, CakiNode *root);

/* Search for the node name given and returns the value as a float */
float caki_get_float(char *node_name, CakiNode *root);

/* Search for the node name given and returns itself */
CakiNode *caki_get_node(char *node_name, CakiNode *root);

/* Search for the node name given and returns the pointer to the value */
void *caki_get_ptr(char *node_name, CakiNode *root);

/* Search for the node name given and returns a string */
char *caki_get_str(char *node_name, CakiNode *root);

/* Free the node and subnodes 
 *
 * root:	The root of definitions, it is recommended to use the
 *		value returned by `caki_parse', as parameter.
 */
void caki_free(CakiNode *root);

#endif /* !_CAKI_H */
