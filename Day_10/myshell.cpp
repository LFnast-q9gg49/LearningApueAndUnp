#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include <glob.h>
#include <wait.h>

const char* delim = " \n\t";

struct cmd_st{
	glob_t globres;
};

static void prompt(void ){
	fprintf(stdout, "mysh-0.1$ ");
}

static void parse(char *line, struct cmd_st *res){
	char *tok;
	int i = 0;
	while(1){
		tok = strsep(&line, delim);
		if (tok == nullptr){
			break;
		}
		if (tok[0] == '\0'){
			continue;
		}
		glob(tok, GLOB_NOCHECK|GLOB_APPEND*i, nullptr, &res->globres);
		i = 1;
	}
}

int main(){
	static char *linebuf = nullptr;
	static size_t linebuf_size = 0;
	struct cmd_st cmd;
	pid_t pid;
	while(1){
		prompt();// 
		
		if(getline(&linebuf, &linebuf_size, stdin)<0){
			break;
		}

		parse(linebuf, &cmd);

		if (0){
			// inner command
			/* do sth */
		}else{
			// outer command
			pid = fork();

			if (pid < 0){
				perror("fork");
				exit(1);
			}else if (pid == 0){
				execvp(cmd.globres.gl_pathv[0], cmd.globres.gl_pathv);
				perror("exec");
				exit(1);
			}else {
				wait(nullptr);
			}
		}
		
	}
	exit(0);
}
