# Readme
1. 在该目录中添加命令，支持nt-shell/letter-shell。
2. 对于nt-shell,需要在对应项目定义一个cmd_table,cmd_table中把对应函数都填写好，然后调用CMD_FUNCTION_REGISTER把cmd_table中的所有函数都注册进去，从而自定义命令即初始化完成
3. 对于letter-shell，只需要在cmd_list.c中实现自定义命令，然后使用宏SHELL_EXPORT_CMD把命令导出，该命令即可以被使用