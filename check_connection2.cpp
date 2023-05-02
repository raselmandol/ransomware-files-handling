//checking incoming and outgoing connection
//if _pclose/_popen fails use just pclose,popen
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE* pipe = popen("netstat -anob", "r"); //opening a pipe to run the command
    if (!pipe) {
        printf("command execution failed!\n");
        return 1;
    }

    char buffer[256];
    while (!feof(pipe)) {
        if (fgets(buffer, 256, pipe) != NULL) {
            printf("%s", buffer);
        }
    }
    //closing the pipe
    pclose(pipe); 

    return 0;
}
