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



int main() {
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
        }
    }
 
  
    return -1;
}