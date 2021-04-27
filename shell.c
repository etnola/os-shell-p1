// https://stackoverflow.com/questions/60218137/tokenizing-a-string-and-return-it-as-an-array
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[] = "> ";
char delimiters[] = " \t\r\n";
extern char **environ;
void sighandler(int);
pid_t pid;
char s[100];
char *ptr;

// https://indradhanush.github.io/blog/writing-a-unix-shell-part-3/
void sighandler(int signum){
  printf("caught signal %d,coming out...\n", signum);
  kill(pid,SIGKILL);
 
}


int main() {

  int i, k, j;
  bool background;
    // Stores the string typed into the command line.
  char command_line[MAX_COMMAND_LINE_LEN];
  char cmd_bak[MAX_COMMAND_LINE_LEN];
  
    // Stores the tokenized command line input.
  char *arguments[MAX_COMMAND_LINE_ARGS];
    	
  while (true) {
      
    do{ 
            // Print the shell prompt.
      printf("%s", prompt);
      fflush(stdout);

            // Read input from stdin and store it in command_line. If there's an
            // error, exit immediately. (If you want to learn more about this line,
            // you can Google "man fgets")
        
      if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
        fprintf(stderr, "fgets error");
          exit(0);
          }
 
      }while(command_line[0] == 0x0A);  // while just ENTER pressed

      
        // If the user input was EOF (ctrl+d), exit the shell.
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }
    // bit.ly/3t2UqyA
        arguments[0] = strtok(command_line,delimiters);
  
        i=0;
        while(arguments[i] != NULL){
          arguments[++i] = strtok(NULL,delimiters);
        }
          arguments[i] = NULL;
      
         // https://stackoverflow.com/a/13098645
        if (strcmp(arguments[0],"cd") == 0){
          
          chdir(arguments[1]);
        } 
        else if (strcmp(arguments[0],"pwd") == 0){
          printf("%s\n", getcwd(s, 100)); 
        }
        else if (strcmp(arguments[0],"echo") == 0){
          k=1;
    
          while(arguments[k] != NULL){
            ptr = arguments[k];
            if(strchr(ptr,'$') != NULL){
              memmove(ptr, ptr+1, strlen(ptr));
              arguments[k] = getenv(ptr);
            }
            printf("%s ",arguments[k]);
            k++; 
          }
          printf("\n");
        }
        else if (strcmp(arguments[0],"exit") == 0){
          exit(0);
        }
        else if (strcmp(arguments[0],"env") == 0){
          if (arguments[1] == NULL){
            for (i = 0; environ[i] != NULL; i++){
                printf("\n%s", environ[i]);
             }
            printf("\n");
          } else {
              printf("%s\n",getenv(arguments[1]));
            
          }
        }
        else if(strcmp(arguments[0],"setenv") == 0){
            const char *str_arr[2];
            char *str = arguments[1];
            i=0;
            str_arr[i] = strtok(str, "=");
            while(str_arr[i] != NULL) {
              str_arr[++i] = strtok(NULL,"=");
            }
            setenv(str_arr[0],str_arr[1],1);
          
        }
        //https://stackoverflow.com/a/26817195
        else{
          if (strcmp(arguments[i-1],"&") == 0){
           arguments[i-1] = NULL;
           background = true;//flag
            
          }
          pid = fork();
          if (pid<0){
            perror("fork failed");
            exit(0);
          }
          else if(pid == 0){ 
            execvp(arguments[0], arguments);
            printf("execvp() failed: No such file or directory");
            exit(0);
          }
          else{
            // 4. The parent process should wait for the child to complete unless its a background process
            if (background  != true){
              wait(NULL);
            }
            
          }
        } 
        /*    
        char *token = strtok(command_line,delimiters);
        char *temp;
        int x = 0;
        temp = strtok(NULL,delimiters);
        while (temp != NULL){
          arguments[x] = temp;
          x+=1;
          temp = strtok(NULL,delimiters);
        }
        
        int pid = fork();
        if (pid == 0){
 
          if (execve(token, arguments, environ) < 0 ){
            perror("error");
            exit(1); 
          }
          exit(0); 
        }
        else if (pid < 0){
          perror("error");
          exit(1);
        }  else {
    
          wait(NULL); 
        }*/
    }
 
  
    return -1;
}