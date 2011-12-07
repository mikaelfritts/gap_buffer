/*
 * This software is provided 'as-is', without any express or implied warranty. 
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Do what you wish, with this code.
 *
 * This is a simple gap buffer implementation.
 *
 * Author: Jones
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gap_buffer.h"

struct gap_buffer
{
    char * buffer;
    int start;
    int end;
    int gap_start;
    int gap_end;
    int cursor;
    int mode;
};

gap_T gap_buffer_new()
{
    gap_T g = malloc(sizeof(struct gap_buffer));

    g->buffer = malloc(sizeof(char) * 10);

    g->start = 0;
    g->end = 9;
    g->gap_start = 0;
    g->gap_end = 9;
    g->cursor = 0;
    g->mode = INSERT_MODE;

    return g;
}

void gap_buffer_move_gap(gap_T g)
{
    // do nothing if the gap is on the cursor already
    if (g->cursor == g->gap_start)
        return;

    // the first position in the buffer that needs to be moved
    char *src;

    // the length within the buffer that needs to be moved
    int length;

    // need the gap size in recalculating some positions
    int gap_size = g->gap_end - g->gap_start;

    // CASE 1: the gap is going to move to the left
    if (g->cursor <= g->gap_start)
    { 
        // when moving gap left, the source must be the cursor position
        src = g->buffer + g->cursor;

        // copy the data in between cursor and the start of the gap
        length = g->gap_start - g->cursor;

        memmove(g->buffer + g->cursor + gap_size+1, src, sizeof(char) * length);

        // reset the gap end and start
        g->gap_end -= length;
        g->gap_start -= length;
    } 
    // CASE 2: the gap is moving to the right
    else if (g->cursor >= g->gap_end)
    {
        // the data to copy begins at the first char after the end of the gap
        src = g->buffer + g->gap_end + 1;

        // the length of the data to copy is from the gap end to the cursor
        length = g->cursor - g->gap_end;

        memmove(g->buffer + g->gap_start, src, sizeof(char) * length);

        // reset gap positions - in this case, we also need to move cursor
        g->gap_end += length;
        g->gap_start += length;
        g->cursor = g->gap_start;
    } 
    // Cursor position should never be inside or outside the gap
    else 
        printf("Cursor is inside or outside the gap! Gap move cancelled.\n");
}

void gap_buffer_resize_buffer(gap_T g)
{   
    // double buffer size - buffer begins with 0, so actual size is g->end + 1
    int new_size = (g->end + 1) * 2; 

    // length of characters after the gap to move to end of resized buf
    int length = g->end - g->gap_end;

    char * buffer = realloc(g->buffer, sizeof(char) * new_size);

    g->buffer = buffer;

    char *src = g->buffer + g->gap_end + 1;

    char *dest = g->buffer + new_size - length;

    memmove(dest, src, sizeof(char) * length);

    g->end = new_size - 1;
    g->gap_end = g->end - length;
}

/*
 * One might be tempted to simply decrement or increment the cursor field 
 * (or add or subtract from it) elsewhere, to move the cursor.   
 *
 * While it may be more efficient to do so, caution is needed.  
 * This function will compensate any cursor movement which may encounter the
 * gap and/or the buffer end or start.  It is recommended that this function
 * be used for any and all cursor movement operations.
 */
void gap_buffer_move_cursor(gap_T g, signed int distance)
{
    // store the original cursor position, before the move
    int old_position = g->cursor;

    int gap_size = g->gap_end - g->gap_start;

    g->cursor += distance;

    // CASE 1: the cursor crossed gap threshold, while moving left
    if (old_position > g->gap_end && g->cursor < g->gap_end)
        g->cursor -= gap_size;
    // CASE 2: the cursor crossed gap threshold, while moving right
    else if (old_position <= g->gap_start && g->cursor > g->gap_start)
        g->cursor += gap_size;

    // make sure cursor didnt move beyond buffer size (not counting gap-size)
    if (g->cursor < 0 || g->cursor > g->end)
    {
        // TODO: Error handling here - for now, return cursor to old position
        g->cursor = old_position;   
    }
}

void gap_buffer_print(gap_T g)
{
    int i = 0; 

    printf(" cursor:%*d",3, g->cursor);
    //printf(" start:%*d",3, g->start);
    printf(" gap_start: %*d", 3, g->gap_start);
    printf(" gap_end:%*d",3, g->gap_end);

    printf("  [");

    i = 0;
    while (i <= g->end)
    {
        if (i > g->gap_start && i < g->gap_end)
            printf("_");
        else if (i == g->gap_start)
            printf("|");
        else if (i == g->gap_end)
            printf("|");
        else 
        {
            if (g->buffer[i] != '\0')
                printf("%c", g->buffer[i]);
            else
                printf("*");
        }
        i++;
    }

    printf("]");
    printf("\n"); 
}

void gap_buffer_put(gap_T g, char ch) 
{
    if (g->mode == REPLACE_MODE)
        gap_buffer_replace(g, ch);
    else
        gap_buffer_insert(g, ch);
}

void gap_buffer_insert(gap_T g, char ch)
{
    // inserts must always happen at the gap start - move the gap if needed
    if (g->cursor != g->gap_start)
        gap_buffer_move_gap(g);

    // the gap may be 0 length.  If so, resize the buffer to create space 
    if (g->gap_end == g->gap_start)
        gap_buffer_resize_buffer(g);

    // double check that resize was succesfull 
    if (g->gap_start >= g->gap_end)
    {
        printf("Error: 'put' failed, no room in buffer\n");
        return;
    }

    // finally, save the char into the buffer, and increment cursor and gap st
    g->buffer[g->cursor] = ch;
    g->gap_start++;
    gap_buffer_move_cursor(g, 1);
}

void gap_buffer_replace(gap_T g, char ch) 
{
    if (g->cursor == g->gap_start)
    {
        // Case 1: If bumping into buffer end, just call insert instead
        if (g->gap_end == g->end)
        {
            gap_buffer_insert(g, ch);
            return;
        }
        
        // Case 2: If bumping into gap, jump it to replace the char after it
        gap_buffer_move_cursor(g, 1);
    }

    // overwrite the char into the buffer, and increment cursor and gap st
    g->buffer[g->cursor] = ch;
    gap_buffer_move_cursor(g, 1);
}

void gap_buffer_delete(gap_T g)
{
    // does nothing if the cursor is on the gap start
    gap_buffer_move_gap(g);

    if (g->gap_start != 0) 
    {
        g->gap_start--;
        gap_buffer_move_cursor(g, -1);
    }
}

void gap_buffer_put_str(gap_T g, char * str)
{
    gap_buffer_move_gap(g);

    int length = strlen(str);

    while (length > (g->gap_end - g->gap_start))
        gap_buffer_resize_buffer(g);

    do
    { 
        gap_buffer_put(g, *str++); 
    }
    while (--length > 0);
}

void gap_buffer_set_mode(gap_T g, int mode)
{
    if (mode == REPLACE_MODE)
        g->mode = REPLACE_MODE;
    else
        g->mode = INSERT_MODE;
}

int gap_buffer_distance_to_end(gap_T g)
{
    int distance = 0;

    if (g->cursor <= g->gap_start)
        distance = (g->end - g->cursor) - (g->gap_end - g->gap_start);

    else if (g->cursor > g->gap_end)
        distance = g->end - g->cursor;

    return distance;     
}

int gap_buffer_distance_to_start(gap_T g)
{
    int distance = 0;

    if (g->cursor > g->gap_end)
        distance = (g->start - g->cursor) + (g->gap_end - g->gap_start);

    else if (g->cursor <= g->gap_start)
        distance = g->start - g->cursor;

    return distance;     
}

void gap_buffer_destroy(gap_T g)
{
    free(g->buffer);
    free(g);
}

