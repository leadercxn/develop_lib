#ifndef __CMD_FUNCTION
#define __CMD_FUNCTION

#include "ntshell_usrcmd.h"
#include "cmd_list.h"

#define CMD_FUNCTION_REGISTER(cmd, description, handler)     ntshell_register(cmd,  description, handler)

#endif

