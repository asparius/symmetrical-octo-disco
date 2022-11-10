#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h> // termios, TCSANOW, ECHO, ICANON
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#define PY_SSIZE_T_CLEAN
#include </usr/include/python3.10/Python.h>

const char * sysname = "shellax";
int module_installed = 0;
enum return_codes {
	SUCCESS = 0,
	EXIT = 1,
	UNKNOWN = 2,
};

struct command_t {
	char *name;
	bool background;
	bool auto_complete;
	int arg_count;
	char **args;
	char *redirects[3]; // in/out redirection
	struct command_t *next; // for piping
};




/**
 * Prints a command struct
 * @param struct command_t *
 */
void print_command(struct command_t * command)
{
	int i=0;
	printf("Command: <%s>\n", command->name);
	printf("\tIs Background: %s\n", command->background?"yes":"no");
	printf("\tNeeds Auto-complete: %s\n", command->auto_complete?"yes":"no");
	printf("\tRedirects:\n");
	for (i=0;i<3;i++)
		printf("\t\t%d: %s\n", i, command->redirects[i]?command->redirects[i]:"N/A");
	printf("\tArguments (%d):\n", command->arg_count);
	for (i=0;i<command->arg_count;++i)
		printf("\t\tArg %d: %s\n", i, command->args[i]);
	if (command->next)
	{
		printf("\tPiped to:\n");
		print_command(command->next);
	}


}


int wiseman(struct command_t * command){


	if(command->args[0] == NULL){

		
		printf("Invalid input\n");
		printf("Wiseman is busy, enter a interval\n");
		return SUCCESS;
	
	}
	else{
		
		char *interval = command->args[0];
		char min[120] = "*/";
		strcat(min,interval);
		char *rem = " * * * * fortune -a | espeak\n";
		strcat(min,rem);
		
		FILE *fp;
		fp = fopen("a.txt", "w+");
        	fputs(min, fp);
		fclose(fp);
		system("crontab a.txt");
		remove("a.txt");				
	
	


		return SUCCESS;


	}	


}

int chatroom(struct command_t * command){

	char * room = command->args[0];
	char * user = command -> args[1];
	char roompath[512] = "/tmp/";
	char userpath[512];
	int fd;
	DIR *d;
	struct dirent *dir;
	
	int fifo_counter = 0;

			
	strcat(roompath,room);
	d = opendir(roompath);	
	strcat(roompath,"/");
	struct stat st = {0};
	
    	if (stat(roompath, &st) == -1) {
      
          mkdir(roompath, 0700);
      	}
	strcpy(userpath,roompath);
	strcat(userpath,user);
	if(mkfifo(userpath,0777) == -1){
		if (errno != EEXIST) {
            		printf("Could not create fifo file\n");
            		return 1;
        		}
	
	}
	char inp_name[128] = "[";
	strcat(inp_name,room);
	strcat(inp_name,"] ");
	strcat(inp_name,user);
	strcat(inp_name,": ");
	printf("Welcome to %s!\n",room);	
	
	
	if (fork() == 0){
		fd = open(userpath, O_RDONLY);	
		while(1){
			
			char buffer[128];
			
			


		
		
	 		
			int res = read(fd,buffer,128);
			if(res == -1 || res == 0){
				//printf("%d\n",errno);

			}
			else{	
				printf("\r%s",buffer);
				
				printf("%s",inp_name);	
				fflush(stdout);
			}
			
		}
	}
	else
	{       
		char inp[128];

		char inp_name[256] = "[";
		strcat(inp_name,room);
		strcat(inp_name,"] ");
		strcat(inp_name,user);
		strcat(inp_name,": ");
		strcpy(inp,inp_name);
		printf("%s",inp);
			
		fflush(stdout);

		while(1){
			char inp_name[256] = "[";
			strcat(inp_name,room);
			strcat(inp_name,"] ");
			strcat(inp_name,user);
			strcat(inp_name,": ");
		
	
			
			char buffer[128];	
			//char *buffers = calloc(sizeof(char), 128);
			fgets(buffer,128,stdin);
			
			strncat(inp_name,buffer,128);
				
			d = opendir(roompath);
			
			if(d){
				
				while((dir = readdir(d)) != NULL){

					if((strcmp(dir->d_name,user) != 0) && (strcmp(dir->d_name,".") != 0) && (strcmp(dir->d_name,"..")!= 0)) {
					if(fork() == 0){
						char otherbuf[128];
						
						strcpy(otherbuf,roompath);
						strcat(otherbuf,dir->d_name);
						fd = open(otherbuf,O_WRONLY);
					
						write(fd,inp_name,strlen(inp_name ) + 1);
						close(fd);
						exit(0);
						}
					
			}

			}
		
				
			
					
		}
		 
		 printf("%s",inp);
		 fflush(stdout);
	
		}
	}
	return SUCCESS;



} 


int process_command(struct command_t *command);

int module(int rootpid ){

	
	char call[1000];
	char number[100];
        strcpy(call,"sudo insmod mymodule2.ko rootpid=");
	sprintf(number,"%d",rootpid);
	strcat(call,number);
	
	if(module_installed == 0){
		system(call);
		module_installed = 1;
		

	}
	
	int fd = open("pids.txt",O_WRONLY | O_CREAT| O_APPEND);
	
	dup2(fd,1);
	close(fd);
	system("sudo dmesg");
	
		

	
		
	chmod("pids.txt",S_IRUSR | S_IWUSR);
	
	
	
	
	return SUCCESS;
}


void line_printer(char *filename)
{
	char call[1000];
	if (fork() == 0)
	{
		strcpy(call, "cat pids.txt | grep MyModule -n | tail -1 > ");
		strcat(call, filename);

		system(call);
		exit(1);
	}
	else
	{
		wait(0);
		FILE *f = fopen(filename, "r");
		char buffer[1000];
		char line[50];
		int des_line;
		while (fgets(buffer, 1000, f))
		{

			char *occ = strchr(buffer, ':');

			strncat(line, buffer, occ - buffer);

			des_line = atoi(line);
		}
		int line_counter = 0;
		f = fopen("pids.txt", "r");
		FILE *f2 = fopen("despids.txt", "w");
		char *sqbr;

		while (fgets(buffer, 1000, f))
		{

			line_counter++;
			if (line_counter > des_line)
			{
				sqbr = strchr(buffer, ']');

				fputs(sqbr + 2, f2);
			}
		}
	}
}

void fn_caller(char* flag,char *filename){

		wchar_t *program = Py_DecodeLocale("Dummy",NULL);
		if (program == NULL){
			fprintf(stderr,"Fatal Error: Cannot decode name\n");
			exit(1);

			}
		Py_SetProgramName(program);
		Py_Initialize();

		PyRun_SimpleString("import os\n");;
		char buffer[100];
		char flagbuff[10];
		strcpy(buffer,"os.system('python3 viz.py ");
		strcpy(flagbuff,flag);
		strcat(buffer,flag);
		strcat(buffer, " ");
		strcat(buffer,filename);
		strcat(buffer,"')\n");
		PyRun_SimpleString(buffer);

		if(Py_FinalizeEx() < 0){
			exit(120);

		}
		PyMem_RawFree(program);

}


int graphviz(struct command_t *command){
	
	char filename[1000];
	char flag[100];

	if(strcmp(command->args[0],"-e") && strcmp(command->args[0],"-y")){
		printf("Invalid flag!\n");
		return 1;
	}
	
	


	strcpy(filename,command->args[1]);
	strcpy(flag,command->args[0]);
	
	if(fork() == 0){
		fn_caller(flag,filename);
	}
	else{
		wait(0);
		remove("process.gv");
		rename("process.gv.png",command->args[2]);
	}	
	return SUCCESS;


}
int free_command(struct command_t *command);
int parse_command(char *buf, struct command_t *command);
int psvis(struct command_t *command){
		

	if(fork() ==0){
		module(atoi(command->args[0]));
		exit(1);
	}
	else{
		wait(0);
		
				
	}

	if(fork()==0){

		line_printer("line.txt");
		
		exit(1);
	}
	else{

		wait(0);

	}

	if(fork()==0){
		
		struct command_t *com=malloc(sizeof(struct command_t));
		memset(com, 0, sizeof(struct command_t)); // set all bytes to 0
		com->name = "graphviz";
		com->args=(char **)malloc(sizeof(char *));
		for(int arg_index = 0; arg_index < 3;arg_index++){
		

		
			com->args[arg_index]=(char *)malloc(100);
		}
		strncpy(com->args[0],"-e",strlen("-e"));

		strncpy(com->args[1], "despids.txt",strlen("despids.txt"));
		strncpy(com->args[2], command->args[1],strlen(command->args[1]));
		com->args[2] = command->args[1];	
		
		process_command(com);
		
		for(int arg_index = 0; arg_index < 3;arg_index++){
			free(com->args[arg_index]);
		}
		free(com->args);
		free(com); 
		exit(1);
			
	
	}
	else{

		wait(0);
		
		remove("pids.txt");
		remove("line.txt");
		remove("despids.txt");

	}
		
		
	return SUCCESS;

}

int re_handler(struct command_t *command);

int pipe_handler(struct command_t *command) {
    int fd[2];
    int intfd;    
    int com_number = 0;
    int ret;	
    struct command_t *currCommand = command;
    if(command->next){
   	 while (currCommand != NULL) {
        
        	if (pipe(fd) == -1) {
            		perror("pipe failed!");
        		}
        

        
        	pid_t pid = fork();

        	if (pid == -1) {
            		perror("fork failed");
            		return UNKNOWN;
        	}
        	else if (pid == 0) {
            
            	if (com_number != 0) {
                // stdin
                	dup2(intfd, 0);
            	}
            
            	if (currCommand->next != NULL) {
                	// stdout
               	 	dup2(fd[1], 1);
            	}
            
            	re_handler(currCommand);
            	exit(0);
        	}
        	else {
            		wait(0);
            		intfd = fd[0];
            		close(fd[1]);
            
		}
        
        currCommand = currCommand->next;
        com_number++;

    		}

		return SUCCESS;
    	

	}
}







int free_command(struct command_t *command)
{
	if (command->arg_count)
	{
		for (int i=0; i<command->arg_count; ++i)
			free(command->args[i]);
		free(command->args);
	}
	for (int i=0;i<3;++i)
		if (command->redirects[i])
			free(command->redirects[i]);
	if (command->next)
	{
		free_command(command->next);
		command->next=NULL;
	}
	free(command->name);
	free(command);
	return 0;
}









int show_prompt()
{
	char cwd[1024], hostname[1024];
    gethostname(hostname, sizeof(hostname));
	getcwd(cwd, sizeof(cwd));
	printf("%s@%s:%s %s$ ", getenv("USER"), hostname, cwd, sysname);
	return 0;
}
/**
 * Parse a command string into a command struct
 * @param  buf     [description]
 * @param  command [description]
 * @return         0
 */
int parse_command(char *buf, struct command_t *command)
{
	const char *splitters=" \t"; // split at whitespace
	int index, len;
	len=strlen(buf);
	while (len>0 && strchr(splitters, buf[0])!=NULL) // trim left whitespace
	{
		buf++;
		len--;
	}
	while (len>0 && strchr(splitters, buf[len-1])!=NULL)
		buf[--len]=0; // trim right whitespace

	if (len>0 && buf[len-1]=='?') // auto-complete
		command->auto_complete=true;
	if (len>0 && buf[len-1]=='&') // background
		command->background=true;

	char *pch = strtok(buf, splitters);
	command->name=(char *)malloc(strlen(pch)+1);
	if (pch==NULL)
		command->name[0]=0;
	else
		strcpy(command->name, pch);

	command->args=(char **)malloc(sizeof(char *));

	int redirect_index;
	int arg_index=0;
	char temp_buf[1024], *arg;
	while (1)
	{
		// tokenize input on splitters
		pch = strtok(NULL, splitters);
		if (!pch) break;
		arg=temp_buf;
		strcpy(arg, pch);
		len=strlen(arg);

		if (len==0) continue; // empty arg, go for next
		while (len>0 && strchr(splitters, arg[0])!=NULL) // trim left whitespace
		{
			arg++;
			len--;
		}
		while (len>0 && strchr(splitters, arg[len-1])!=NULL) arg[--len]=0; // trim right whitespace
		if (len==0) continue; // empty arg, go for next

		// piping to another command
		if (strcmp(arg, "|")==0)
		{
			struct command_t *c=malloc(sizeof(struct command_t));
			int l=strlen(pch);
			pch[l]=splitters[0]; // restore strtok termination
			index=1;
			while (pch[index]==' ' || pch[index]=='\t') index++; // skip whitespaces

			parse_command(pch+index, c);
			pch[l]=0; // put back strtok termination
			command->next=c;
			continue;
		}

		// background process
		if (strcmp(arg, "&")==0)
			continue; // handled before

		// handle input redirection
		redirect_index=-1;
		if (arg[0]=='<')
			redirect_index=0;
		if (arg[0]=='>')
		{
			if (len>1 && arg[1]=='>')
			{
				redirect_index=2;
				arg++;
				len--;
			}
			else redirect_index=1;
		}
		if (redirect_index != -1)
		{
			command->redirects[redirect_index]=malloc(len);
			strcpy(command->redirects[redirect_index], arg+1);
			continue;
		}

		// normal arguments
		if (len>2 && ((arg[0]=='"' && arg[len-1]=='"')
			|| (arg[0]=='\'' && arg[len-1]=='\''))) // quote wrapped arg
		{
			arg[--len]=0;
			arg++;
		}
		command->args=(char **)realloc(command->args, sizeof(char *)*(arg_index+1));
		command->args[arg_index]=(char *)malloc(len+1);
		strcpy(command->args[arg_index++], arg);
	}
	command->arg_count=arg_index;
	return 0;
}

void prompt_backspace()
{
	putchar(8); // go back 1
	putchar(' '); // write empty over
	putchar(8); // go back 1 again
}
/**
 * Prompt a command from the user
 * @param  buf      [description]
 * @param  buf_size [description]
 * @return          [description]
 */
int prompt(struct command_t *command)
{
	int index=0;
	char c;
	char buf[4096];
	static char oldbuf[4096];

    // tcgetattr gets the parameters of the current terminal
    // STDIN_FILENO will tell tcgetattr that it should write the settings
    // of stdin to oldt
    static struct termios backup_termios, new_termios;
    tcgetattr(STDIN_FILENO, &backup_termios);
    new_termios = backup_termios;
    // ICANON normally takes care that one line at a time will be processed
    // that means it will return if it sees a "\n" or an EOF or an EOL
    new_termios.c_lflag &= ~(ICANON | ECHO); // Also disable automatic echo. We manually echo each char.
    // Those new settings will be set to STDIN
    // TCSANOW tells tcsetattr to change attributes immediately.
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);


    //FIXME: backspace is applied before printing chars
	show_prompt();
	int multicode_state=0;
	buf[0]=0;
  	while (1)
  	{
		c=getchar();
		// printf("Keycode: %u\n", c); // DEBUG: uncomment for debugging

		if (c==9) // handle tab
		{
			buf[index++]='?'; // autocomplete
			break;
		}

		if (c==127) // handle backspace
		{
			if (index>0)
			{
				prompt_backspace();
				index--;
			}
			continue;
		}
		if (c==27 && multicode_state==0) // handle multi-code keys
		{
			multicode_state=1;
			continue;
		}
		if (c==91 && multicode_state==1)
		{
			multicode_state=2;
			continue;
		}
		if (c==65 && multicode_state==2) // up arrow
		{
			int i;
			while (index>0)
			{
				prompt_backspace();
				index--;
			}
			for (i=0;oldbuf[i];++i)
			{
				putchar(oldbuf[i]);
				buf[i]=oldbuf[i];
			}
			index=i;
			continue;
		}
		else
			multicode_state=0;

		putchar(c); // echo the character
		buf[index++]=c;
		if (index>=sizeof(buf)-1) break;
		if (c=='\n') // enter key
			break;
		if (c==4) // Ctrl+D
			return EXIT;
  	}
  	if (index>0 && buf[index-1]=='\n') // trim newline from the end
  		index--;
  	buf[index++]=0; // null terminate string

  	strcpy(oldbuf, buf);

  	parse_command(buf, command);

  	// print_command(command); // DEBUG: uncomment for debugging

    // restore the old settings
    tcsetattr(STDIN_FILENO, TCSANOW, &backup_termios);
  	return SUCCESS;
}





int re_handler(struct command_t *command){
	if(command->redirects[0] == NULL && command->redirects[1] == NULL && command->redirects[2] == NULL){
		
		return process_command(command);

	
	}

	else{


		if(command->redirects[0] != NULL && command->redirects[1] != NULL && command->redirects[2] != NULL){
			printf("Invalid Command!\n");
			return SUCCESS;
		}

		else{
		   if(fork() == 0){	
			if(command->redirects[0] != NULL){

				

				int fd = open(command->redirects[0],O_RDONLY);

				dup2(fd,STDIN_FILENO);
				close(fd);
			

			}

			if(command->redirects[1] != NULL){

				

					int fd =open(command->redirects[1],O_CREAT | O_WRONLY,0777);
					dup2(fd,STDOUT_FILENO);
					close(fd);
	
			}

			if(command->redirects[2] != NULL){

				
					
					int fd = open(command->redirects[2],O_CREAT | O_WRONLY | O_APPEND,0777);
					dup2(fd,STDOUT_FILENO);
					close(fd);
	

			}

			


			process_command(command);

			exit(0);
		   
		

		  	 
		   }
		   else{

			   wait(0);
		   }
	   		}	   
		


			}
	return SUCCESS;

	
}
int main()
{
	while (1)
	{
		struct command_t *command=malloc(sizeof(struct command_t));
		memset(command, 0, sizeof(struct command_t)); // set all bytes to 0

		int code;
		code = prompt(command);
		if (code==EXIT) break;
	
		if(command->next){
			code = pipe_handler(command);

		}
		else{
			code = re_handler(command);
	
		}

		if (code==EXIT) break;

		free_command(command);
	}

	printf("\n");
	return 0;
}

int process_command(struct command_t *command)
{
	int r;
	if (strcmp(command->name, "")==0) return SUCCESS;

	if (strcmp(command->name, "exit")==0){
		
		system("sudo rmmod mymodule2.ko");
		module_installed = 0;
		return EXIT;
	}

	if (strcmp(command->name, "cd")==0)
	{
		if (command->arg_count > 0)
		{
			r=chdir(command->args[0]);
			if (r==-1)
				printf("-%s: %s: %s\n", sysname, command->name, strerror(errno));
			return SUCCESS;
		}
	}

	if(strcmp(command->name,"wiseman") == 0){


		return wiseman(command);
	
	}

	if(strcmp(command->name,"chatroom") ==0){

		if(fork()==0){

			chatroom(command);
			exit(0);
		}
		else{
			wait(0);

		}
		return SUCCESS;

	}
	if(strcmp(command->name,"graphviz") == 0){

		if(fork() == 0){
			graphviz(command);
			exit(0);
		}
		else{
			wait(0);
			module_installed = 1;

		}
		return SUCCESS;

	}

	if(strcmp(command->name, "psvis") == 0){
		if(fork() == 0){

			psvis(command);
			exit(0);
		}
		else{

			wait(0);
		}
		return SUCCESS;

	}

	pid_t pid=fork();
	pid_t back;
	if (pid==0) // child
	{
		back = fork();
		if(back == 0){
			
		
			/// This shows how to do exec with environ (but is not available on MacOs)
	    		// extern char** environ; // environment variables
			// execvpe(command->name, command->args, environ); // exec+args+path+environ

			/// This shows how to do exec with auto-path resolve
			// add a NULL argument to the end of args, and the name to the beginning
			// as required by exec

			// increase args size by 2
			command->args=(char **)realloc(
				command->args, sizeof(char *)*(command->arg_count+=2));

			// shift everything forward by 1
			for (int i=command->arg_count-2;i>0;--i)
				command->args[i]=command->args[i-1];

			// set args[0] as a copy of name
			command->args[0]=strdup(command->name);
			// set args[arg_count-1] (last) to NULL
			command->args[command->arg_count-1]=NULL;

		//execvp(command->name, command->args); // exec+args+path
			char* real_path = realpath(command->name,NULL);
			if(real_path == NULL){
				
				char *paths = getenv("PATH");
				char * token = strtok(paths,":");
			
				char tmppath[512];
				while(token){
					strcpy(tmppath,token);
					strcat(tmppath,"/");
					strcat(tmppath, command->name);
					if(!access(tmppath,X_OK)){

						execv(tmppath,command->args);

						}
					token = strtok(NULL,":");
			
			}
			}
			else{
				
				execv(real_path, command->args);

			}


		
		

		
		
			exit(0);
		}
		else{
			if (command->background){
				exit(0);
			}
			else{
				wait(0);
				exit(0);
			}

		}
		return SUCCESS;
		
		/// TODO: do your own exec with path resolving using execv()
	}
	else
	{
		

    // TODO: implement background processes here
    // wait for child process to finish
    		wait(0);
		return SUCCESS;
	}

	// TODO: your implementation here

	printf("-%s: %s: command not found\n", sysname, command->name);
	return UNKNOWN;
}










