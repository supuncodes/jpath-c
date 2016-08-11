#include <stdio.h>
#include "jpath.h"

char *read_file(char *filename);

void main(){
    char *filecontents = read_file("./nested.json");
    if (filecontents){
        char *jsonvalue = json_path("hello.test", filecontents);
        printf ("Value is : %s\n", jsonvalue);
    }
}

char *read_file(char *filename){
    char * buffer = 0;
    long length;
    FILE * f = fopen (filename, "rb");

    if (f){
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = malloc (length);
        
        if (buffer){
            fread (buffer, 1, length, f);
        }

        fclose (f);
    }
    
    return buffer;
}