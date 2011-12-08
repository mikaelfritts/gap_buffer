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

#ifndef DRJ_GAP_BUFFER_H__
#define DRJ_GAP_BUFFER_H__

#define DEBUG 1

#define INSERT_MODE 0
#define REPLACE_MODE 1

#define DIST_END(p) gap_buffer_distance_to_end(p)
#define DIST_START(p) gap_buffer_distance_to_start(p)

#ifdef DEBUG
    #define INITIAL_SIZE 10 
    #define GROW_SIZE 10
#else
    #define INITIAL_SIZE 1024
    #define GROW_SIZE 1024
#endif

typedef struct gap_buffer *gap_T;

/*
 * Creates a new gap buffer.  Use this first.
 *
 * Usage:  gap_T gb = gap_buffer_new();
 */
gap_T gap_buffer_new();

/*
 * Moves the gap in the buffer to the position of the cursor.  
 *
 * Characters surrounding the new gap position will be displaced around the 
 * gap.  If the start of the gap is already at the cursor position, the 
 * function does * nothing, and returns.  This function is usually called 
 * automatically when doing insert, delete, or replace operations.
 */
void gap_buffer_move_gap(gap_T);

/*
 *  Resizes the buffer. 
 *
 *  Usually when there is no space left in the gap, and an insert is attempted. 
 *  You usually don't need to call this directly, it will be called 
 *  automatically when needed.
 */
void gap_buffer_resize_buffer(gap_T);

/*
 * Inserts or replaces a character in the gap at the cursor position.
 * 
 * Param: ch - a character to be inserted into the buffer at cursor position
 *
 * It will call either gap_buffer_insert() or gap_buffer_replace() depending
 * on to what the gap buffer mode variable is set (INSERT_MODE or REPLACE_MODE).
 * The mode is set to INSERT_MODE by default, but can be changed any time.  
 * Once changed, it will be the default mode until manually changed back.
 * Insert mode places characters after the cursor.  Replace mode, as you may 
 * have guessed, overwrites existing characters.
 */
void gap_buffer_put(gap_T, char);

/*
 * Moves the cursor a certain distance in the buffer
 *
 * Params: distance  -  the spaces to move cursor, relative from current pos.
 *
 * A negative distance will move the cursor to the left.  A positive distance
 * will move it right.  The distance moved will *not* count the space in the
 * gap.  The gap will be jumped automatically.  The gap will not be moved to
 * the cursor position, unless an insert, replace or delete operation is 
 * attempted (or is set manually).
 */
void gap_buffer_move_cursor(gap_T, signed int);

/*
 * Awful little function most used for debugging and the demo.  Probably don't
 * want to use it for anything.
 */
void gap_buffer_print(gap_T);

/*
 * Inserts a character at the cursor position.
 *
 * Param: ch  - a single character to be added to the buffer
 *
 * Will resize the buffer automatically, if there is no room for it.  It will
 * move the gap automatically, if it is not at the cursor position.  This can
 * be used to insert even when buffer is in replace mode.
 */
void gap_buffer_insert(gap_T, char);

/*
 * Replaces a character at the cursor position.
 *
 * Param: ch  - a single character to replace an existing character in buffer.
 *
 * Replacing a character will advance the cursor.  If the gap is encountered,
 * it will be skipped over, as if it were not there.  If the cursor runs into
 * the buffer end, the gap will automatically be moved to the cursor, the 
 * buffer will be expanded if necessary, and inserts will occur.  This can be
 * used even if insert mode is set.
 */
void gap_buffer_replace(gap_T, char);

/*
 * Deletes a character at the position in the buffer.
 */ 
void gap_buffer_delete(gap_T);

/*
 * Inserts a string at the cursor position in the buffer
 *
 * Param:  str - a string to be inserted into the buffer
 *
 * Automatically resizes buffer until there is room for the string. 
 */
void gap_buffer_put_str(gap_T, char *);

/*
 * Changes the mode of the buffer to either insert or replace.
 *
 * Param:  mode  - Either INSERT_MODE or REPLACE_MODE
 *
 * In "replace" mode, all calls to gap_buffer_put() will overwrite characters
 * in place.  Default is INSERT_MODE
 */
void gap_buffer_set_mode(gap_T, int);

/*
 * Returns the relative distance to the end of the buffer, from cursor
 * so that one can easily jump to the end without knowing the relative distance
 */
int gap_buffer_distance_to_end(gap_T);

/*
 * Returns the relative distance to the beginning of the buffer, from cursor
 */
int gap_buffer_distance_to_start(gap_T);

/*
 * Call this to free the dynamically allocated buffer struct and char array.
 *
 * This is simply a convenience function, its generally not necessary to call
 * if you're using gap_T as an automatic pointer (I think).
 *
 */
void gap_buffer_destroy(gap_T);

#endif /* DRJ_GAP_BUFFER_H__ */
