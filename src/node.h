/*  node.h - tree struct functions definitions.
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

#ifndef NODE_H
#define NODE_H

#include "caki.h"

/* Allocate a new Node a return */
CakiNode *caki_node_new();

/* Insert a new Node into other */
void caki_node_insert(CakiNode *, CakiNode *);

/* Free all the space of Node and childs */
void caki_node_free(CakiNode *);

#endif /* !NODE_H */
