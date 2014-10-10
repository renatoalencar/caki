/*  node.c - tree struct routines.
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
#include "caki.h"

CakiNode *caki_node_new()
{
	return (CakiNode *) calloc(1, sizeof(CakiNode));
}

void caki_node_insert(CakiNode *root, CakiNode *new)
{

	if (root->next == NULL && root->end == NULL) {
		root->next = new;
		root->end = new;
	}
	else {
		root->end->next = new;
		root->end = new;
	}
}

void caki_node_free(CakiNode *root)
{
	if (root->next != NULL)
		caki_node_free(root->next);
	if (root->type == TNODE)
		caki_node_free((CakiNode *) root->v_ptr);
	else
		free(root->v_ptr);
	free(root->name);
	free(root);
}
