/*  tape.c - emulates a turing tape.
    Copyright (C) 2018 Renato Alencar <renatoalencar.73@gmail.com>

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

#ifdef __MACH__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include "tape.h"

/* Create a new tape
 *
 * A tape holds a buffer and keeps track of its position
 */
CakiTape *caki_tape_new(void *buffer, size_t length)
{
  CakiTape *result = (CakiTape *) calloc(1, sizeof(CakiTape));

  result->buffer = buffer;
  result->position = 0;
  result->length = length;

  return result;
}

/*
 * Return the actual character on the tape and forward one position
 * Return END_OF_TAPE if the tape has ended.
 */
char caki_tape_forward(CakiTape *tape)
{
  if (tape->position == tape->length)
    return END_OF_TAPE;
  
  return ((char *) tape->buffer)[tape->position++];
}

/* Return the actual character on the tape and backard one position
 * Return END_OF_TAPE if it is in the beginning of the tape.
 */
char caki_tape_backward(CakiTape *tape)
{
  if (tape->position == 0)
    return END_OF_TAPE;
  
  return ((char *) tape->buffer)[tape->position--];
}

/* Consume the tape until finds a specific char */
void caki_tape_consume_until(CakiTape *tape, char stop)
{
  while(caki_tape_forward(tape) != stop);
}

/* Free the memory used by the tape struct */
void caki_tape_free(CakiTape *tape) {
  free((void *) tape);
}