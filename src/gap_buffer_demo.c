/*
 * This software is provided 'as-is', without any express or implied warranty. 
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Do what you wish, with this code.
 *
 * This is a simple demo of the gap buffer - a *very* basic demonstration
 *
 * Author: Jones
 */

#include <stdlib.h>
#include <stdio.h>
#include "lib/gap_buffer.h"

int main()
{
    gap_T gb;

    gb = gap_buffer_new();

    gap_buffer_put_str(gb, "helwd");            gap_buffer_print(gb);
    gap_buffer_move_cursor(gb, -1);
    gap_buffer_put_str(gb, "orl");              gap_buffer_print(gb);
    gap_buffer_move_cursor(gb, -7);

    // this does the same thing as gap_buffer_put while in replace mode
    gap_buffer_replace(gb, 'H');                gap_buffer_print(gb);
    gap_buffer_move_cursor(gb, 2);
    gap_buffer_put_str(gb, "lo!#o ");           gap_buffer_print(gb);
    gap_buffer_delete(gb);                      gap_buffer_print(gb);
    gap_buffer_delete(gb);                      gap_buffer_print(gb);
    gap_buffer_delete(gb);                      gap_buffer_print(gb);
    gap_buffer_delete(gb);                      gap_buffer_print(gb);
    gap_buffer_put(gb, ' ');                    gap_buffer_print(gb);

    /* demonstrate replace mode */
    gap_buffer_set_mode(gb, REPLACE_MODE);

    // these replace instead of insert now!
    gap_buffer_put(gb, 'W');                    gap_buffer_print(gb);

    /* now puts are back to insert mode! */
    gap_buffer_set_mode(gb, INSERT_MODE);

    gap_buffer_move_cursor(gb, 3);
    gap_buffer_put(gb, '!');                    gap_buffer_print(gb);

    // this does the same thing as an insert mode put
    gap_buffer_insert(gb, '!');                 gap_buffer_print(gb);
    gap_buffer_put_str(gb, " Bye!");            gap_buffer_print(gb);
    gap_buffer_put_str(gb, " World!");          gap_buffer_print(gb);
    gap_buffer_move_cursor(gb, -7);             gap_buffer_print(gb);
    gap_buffer_delete(gb);                      gap_buffer_print(gb);
    gap_buffer_move_cursor(gb, DIST_END(gb));   gap_buffer_print(gb);
    gap_buffer_put(gb, '!');                    gap_buffer_print(gb);
    gap_buffer_move_cursor(gb, DIST_START(gb));
    gap_buffer_move_gap(gb);
    gap_buffer_print(gb);

    gap_buffer_destroy(gb);

    return 0;
}

