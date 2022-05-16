#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<glob.h>
#include<sys/wait.h>

#define DELIMS " \t\n"

struct cmd_st{
		glob_t globres;
};

static void prompt(void){
		fprintf(stdout, "myShell v0.0.1$ ");
}

static void parse(char *msg, struct cmd_st *cmd){
		char *tok;
		int cnt = 0;
		while(1){
			tok = strsep(&msg, DELIMS);
			if (tok == nullptr){
					break;
			}
			if (tok[0] == '\0'){
					continue;
			}
			glob(tok, GLOB_NOCHECK | (GLOB_APPEND * cnt), nullptr, &cmd->globres);
			cnt = 1;
		}
}

int main(){
		pid_t pid;
		char *msg = nullptr;
		size_t msgSize = 0;
		struct cmd_st cmd;
		while(1){		
			prompt();
			if (getline(&msg, &msgSize, stdin) < 0){
					break;
			}
			parse(msg, &cmd);
			if (0){
					// inner command
			}else{
					pid = fork();
					if (pid < 0){
						perror("fork error");
						exit(1);
					}else if (pid  == 0){
							execvp(cmd.globres.gl_pathv[0], cmd.globres.gl_pathv);
							perror("execvp error");
							exit(0);
					}else{
							wait(nullptr);
					}
			}
		}
}
