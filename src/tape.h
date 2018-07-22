/*  tape.h - tape definitions.
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

#ifndef TAPE_H
#define TAPE_H

#define END_OF_TAPE -1

/* This struct is used to keep track of the actual position of the buffer
 * and forward and backward it. It also keeps track of the buffer length.
 *
 * Do not create a new one by your self, use caki_tape_new instead.
 */
struct _cakitape {
  void *buffer;
  size_t position;
  size_t length;
};

typedef struct _cakitape CakiTape;

/* Creates a new tape allocated on the memory */
CakiTape *caki_tape_new(void *buffer, size_t length);

/* Forward one byte on the tape, returns END_OF_TAPE if the tape has ended. */
char caki_tape_forward(CakiTape *tape);

/* Backward one byte on the tape, returns END_OF_TAPE if it is on the beginning of the tape */
char caki_tape_backward(CakiTape *tape);

/* Forward the tape until find a stop char */
void caki_tape_consume_until(CakiTape *tape, char stop);

/* Free the memory used by the tape */
void caki_tape_free(CakiTape *tape);

#endif /* !TAPE_H */