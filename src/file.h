#ifndef _NOTEPAD_FILE_H
#define _NOTEPAD_FILE_H

#include "std.h"

void file_init(void);
void file_open(LPCWSTR path);
void file_close(void);

LPCWSTR file_get_name(void);
uint64_t file_get_line_count(void);
uint64_t file_get_line_length(uint64_t line_idx);
uint64_t file_get_line_span(uint64_t line_idx, uint64_t char_offset, uint64_t char_count, void *buffer);

#endif /* _NOTEPAD_FILE_H */
