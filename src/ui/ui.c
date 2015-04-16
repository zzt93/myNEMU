#include "ui/ui.h"
#include "common.h"
#include "utility/input_handler.h"
#include "ui/cmd/run_cmd.h"
#include "ui/cmd/info_cmd.h"

#include <signal.h>
#include <stdlib.h>
#include <string.h>

/* This function will be called when you press <C-c>. And it will return to
 * where you press <C-c>. If you are interesting in how it works, please
 * search for "Unix signal" in the Internet.
 */
static void control_C(int signum) {
	if(nemu_state == RUNNING) {
		nemu_state = INT;
	}
}

void init_signal() {
	/* Register a signal handler. */
	struct sigaction s;
	memset(&s, 0, sizeof(s));
	s.sa_handler = control_C;
	int ret = sigaction(SIGINT, &s, NULL);
	assert(ret == 0);
}

void main_loop() {
	char *cmd;
	while(1) {
        /*
          When the user can input the commands, the program has only three status:
           END, STOP, INT
        */
		cmd = get_input();
		char *p = strtok(cmd, " ");

        cmd = NULL;// the strtok ask for a NULL
		if(p == NULL) { continue; }

        //printf("command is %s\n", p);
        fflush(stdout);
		if(strcmp(p, "c") == 0) { cmd_c(); }
		else if(strcmp(p, "r") == 0) { cmd_r(); }
		else if(strcmp(p, "q") == 0) { return; }

        else if(strcmp(p, "si") == 0) {
            char *num = strtok(cmd, " ");
            if (num == NULL) {
                cmd_si(1);
            } else {
                cmd_si(atoi(num));
            }

        }

        else if(strcmp(p, "info") == 0){
            char *info = strtok(cmd, " ");
            if(strcmp(info, "r") == 0){
                cmd_info_r();
            } else if(strcmp(info, "b") == 0) {
                cmd_info_b();
            } else {
            }
        }

        else if(strcmp(p, "p") == 0){
            char *exp = strtok(cmd, " ");
            if(exp == NULL){
                puts("no expression");
            } else {
                cmd_p(exp);
            }
        }

        else if(strcmp(p, "x") == 0){
            char *n = strtok(cmd, " ");
            if(n == NULL){
                puts("no number and expression");
            } else if (atoi(n) == 0){
                puts("no number or false format");
            } else {
                char *exp = strtok(cmd, " ");
                if (exp == NULL) {
                    puts("no expression");
                } else {
                    cmd_x(atoi(n), exp);
                }

            }
        }

        else if(strcmp(p, "b") == 0){
            /* TODO:  check the separator " *"*/
            char *n = strtok(cmd, " *");
            //printf("n is %s\n", n);

            if (n == NULL) {
                puts("no address following. (line number is not supported for the time being)");
            } else {
                cmd_b(n);
            }

        }
        else if(strcmp(p, "d") == 0){
            char *n = strtok(cmd, " ");
            if (n == NULL) {
                puts("all breakpoint are deleted");
                init_bp_pool();
            } else {
                cmd_d(n);
            }
        } else if (strcmp(p, "w") == 0) {
            char *n = strtok(cmd, " ");
            if (n == NULL) {
                puts("no expression following");
                return;
            } else {
                cmd_w(n);
            }
        } else if (strcmp(p, "dw") == 0) {
            char *n = strtok(cmd, " ");
            if (n == NULL) {
                puts("no number followed");
                return;
            } else {
                cmd_dw(n);
            }
        }
		/* TODO: Add more commands */

		else { printf("Unknown command '%s'\n", p); }
	}
}
