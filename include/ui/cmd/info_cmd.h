#ifndef  __INFO_CMD_H__
#define  __INFO_CMD_H__

void cmd_info_r();
void cmd_info_b();
void cmd_p(char*);
void cmd_b(char*);
void cmd_d(char*);
void cmd_x(int, char*);
void cmd_w(char*);
void cmd_dw(char*);
//for cmd_r
void init_bp_pool();
void wp_pool_init();

#endif /*  __INFO_CMD_H__ */