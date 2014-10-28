/*  parser.h - parser function definitions.
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

#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"

/* Parse a file pointer, save it into a Node and return it */
CakiNode *__caki_parse(FILE *);

/* Parse a statement - <key> <value>; */
CakiNode *caki_parse_statement(CakiToken **);

/* Parse a node - <key> { ... } */
CakiNode *__caki_parse_node(CakiToken **);

/* Return if token is of the type given */
int caki_parser_accept(CakiToken *, CakiTokenType);

/* Raise error if toke isn't of the type given */
void caki_parser_expect(CakiToken **, CakiTokenType);

#endif /* !PARSER_H */
