//checking incoming and outgoing connection
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  //running netstat -anob setting command
    FILE* pipe = _popen("netstat -anob", "r"); //opening a pipe to run the command
    if (!pipe) {
        printf("command execution failed!\n");
        return 1;
    }

    //buffer-->set
    char buffer[256];
    while (!feof(pipe)) {
        if (fgets(buffer, 256, pipe) != NULL) {
            printf("%s", buffer);
        }
    }
    //closing the pipe
    _pclose(pipe); 

    return 0;
}
