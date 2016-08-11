#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int search_key(int startpos, char *key, char *jsondoc, int doclen, char*valuestring);
char *json_path(char *path, char *jsondoc);