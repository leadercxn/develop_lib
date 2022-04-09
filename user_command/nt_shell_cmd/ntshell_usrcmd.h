/**
 * @file ntshell_usrcmd.h
 * @author Shinichiro Nakamura
 * @brief ナチュラルタイニーシェルタスクコマンドの定義。
 * @details
 */

/*
 * ===============================================================
 *  Natural Tiny Shell - User Command Module
 * ===============================================================
 * Copyright (c) 2010-2011 Shinichiro Nakamura
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * ===============================================================
 */

#ifndef NTSHELL_USRCMD_H
#define NTSHELL_USRCMD_H

#include "board_config.h"

typedef int (* ntshell_call_func)(int argc, char **argv);

/**
 * @brief コマンドテーブル。
 */
typedef struct {
    char *cmd;  /**< コマンド名。 */
    char *desc; /**< コマンド説明。 */
    ntshell_call_func func; /**< コールバック関数。 */
} cmd_table_t;


usart_object_t get_usart_object(void);
int ntshell_register( char *cmd,  char *desc, ntshell_call_func call);
int ntshell_usrcmd_execute(const char *text, void *extobj);

#endif

