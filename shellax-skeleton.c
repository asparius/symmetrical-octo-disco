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
const char * sysname = "shellax";

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
/**
 * Release allocated memory of a command
 * @param  command [description]
 * @return         [description]
 */

void io_handler(struct command_t * command){

	int redirect_index = 0;
	while(redirect_index < 3){

		if(command->redirects[redirect_index] != NULL){
			break;	
		}

		redirect_index++;
	}
	print_command(command);
	printf("%s\n",command->args[command->arg_count -1]);

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

int saitama(struct command_t * command){
	
	int line;
	char c;
	FILE *fptr;
	fptr = fopen("saitama.txt","r");
		
	if (fptr == NULL){

    printf("Cannot open file \n");
    exit(0);

    }
  
    c = fgetc(fptr);
	line = 0;

	while (c != EOF){
        printf ("%c", c);
        c = fgetc(fptr);

		if(c=='\n'){
			line+=1;
		}

		if(line == 9){
			printf("\n⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡤⠀⠄⠀⠀⠀⠀⠀⠈⢷⠒⠊⠉⠉⡍⠉⣽⠀⠀⢀⢸⡉⠉⠹⢉⠉⣹⠃⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀");
			printf("%s", command->args[0]);
			line+=1;
		}
    }

    fclose(fptr);

return SUCCESS;
}

int uniq(struct command_t * command){

	printf("\n");

	int counter;
	int wordCounter = 1;

	if(command->args[0] == NULL){

		printf("Enter inputs next time Sherlock\n");
		return SUCCESS;

	}else if((!(strcmp(command->args[(command->arg_count)-1],"-c"))) && (command->arg_count == 1)){

		printf("Enter inputs next time Sherlock\n");
		return SUCCESS;

	}else if((!(strcmp(command->args[(command->arg_count)-1],"--count"))) && (command->arg_count == 1)){

		printf("Enter inputs next time Sherlock\n");
		return SUCCESS;
		
	}else if(!(strcmp(command->args[(command->arg_count)-1],"-c"))){
		
		wordCounter = 1;

		for(counter = 0; counter < (command->arg_count)-1 ; counter++){

			if(strcmp(command->args[counter], command->args[counter+1])){
				
				printf("%d %s\n",wordCounter, command->args[counter]);
				wordCounter = 1;

			}else{
				
				wordCounter++;
				
				}
		}
		
		return SUCCESS;

	}else if(!(strcmp(command->args[(command->arg_count)-1],"--count"))){
		
		wordCounter = 1;

		for(counter = 0; counter < (command->arg_count)-1 ; counter++){

			if(strcmp(command->args[counter], command->args[counter+1])){
				
				printf("%d %s\n",wordCounter, command->args[counter]);
				wordCounter = 1;

			}else{
				
				wordCounter++;
				
				}
		}
		
		return SUCCESS;

	}else{

		if(command->arg_count == 1){

				printf("%s\n", command->args[counter]);

		}else if(command->arg_count == 2){
			
			if(strcmp(command->args[0], command->args[1])){
				printf("%s\n", command->args[0]);
				printf("%s\n", command->args[1]);
			}else{
				printf("%s\n", command->args[0]);
			}

		}else{
		
		for(counter = 0; counter < (command->arg_count)-2 ; counter++){

			if(strcmp(command->args[counter], command->args[counter+1])){
				
				printf("%s\n", command->args[counter]);

			}

		}

		if(!(strcmp(command->args[counter], command->args[counter+1]))){

				printf("%s\n", command->args[counter]);

		}else{

			printf("%s\n", command->args[counter]);
			printf("%s\n", command->args[counter+1]);

		}

		return SUCCESS;

	}}

	return SUCCESS;
	
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
	printf("Welcome to comp304!\n");	
	
	
	if (fork() == 0){
		while(1){
			
			char buffer[128];
			/*
			fifo_counter = 0;	
			if(d){
				while((dir =readdir(d)) != NULL){
					fifo_counter++;
					
				}

			}
			
			if(fifo_counter - 2  == 1){
				exit(0);
			}
			
			fd = open(userpath, O_RDONLY);	
			read(fd,buffer,128);
			buffer[127] = "\0";
			printf("%s",buffer);
			close(fd);
			*/ 
		}
	}
	else{
		while(1){
			
			char inp_name[128] = "[";
			strcat(inp_name,room);
			strcat(inp_name,"] ");
			strcat(inp_name,user);
			strcat(inp_name,": ");
			
			char buffer[128];

			fd = 0;
			
			fgets(buffer,128,stdin);
			strcat(inp_name,buffer);
			strcat(inp_name,"\n");
			strcat(inp_name,"\0");	
			d = opendir(roompath);
			
			if(d){
				
				while((dir = readdir(d)) != NULL){

					if((strcmp(dir->d_name,user) != 0) && (strcmp(dir->d_name,".") != 0) && (strcmp(dir->d_name,"..")!= 0)) {
						char otherbuf[128];
						
						strcpy(otherbuf,roompath);
						strcat(otherbuf,dir->d_name);
						fd = open(otherbuf,O_WRONLY);
						write(fd,inp_name,strlen(inp_name));
						
					}
				}

			}
			
			}		
		}
	

	return SUCCESS;


} 
int process_command(struct command_t *command);

int pipe_handler(struct command_t *command) {
    int fd[2];
    int intfd;    
    int com_number = 0;

    struct command_t *currCommand = command;

    while (currCommand != NULL) {
        //printf("It is working %d\n", processCounter);
        
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
            
            process_command(currCommand);
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
/**
 * Show the command prompt
 * @return [description]
 */
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

int main()
{
	while (1)
	{
		struct command_t *command=malloc(sizeof(struct command_t));
		memset(command, 0, sizeof(struct command_t)); // set all bytes to 0

		int code;
		code = prompt(command);
		if (code==EXIT) break;
	
		code = pipe_handler(command);
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

	if (strcmp(command->name, "exit")==0)
		return EXIT;

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

		return chatroom(command);

	}

	if(strcmp(command->name,"uniq") ==0){

		return uniq(command);

	}

	if(strcmp(command->name,"saitama") ==0){

		return saitama(command);

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