#pragma once

#include <stdio.h>

//! @brief opens file to read from and checks if it is open correctly
//!
//! @param [in]  name name of file that we are working with
//! @param [out] code_file var where is ptr to stream
//!
//! @return 0 if all worked correctly and 1 if error
int open_read_file (const char* name, FILE** code_file);

//! @brief opens file to write in and checks if it is open correctly
//!
//! @param [in]  name name of file that we are working with
//! @param [out] code_file var where is ptr to stream
//!
//! @return 0 if all worked correctly and 1 if error
int open_write_file(const char* name, FILE** code_file);

