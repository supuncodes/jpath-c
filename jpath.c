#include "jpath.h"

char *json_path(char *path, char *jsondoc){

    char *outdata = NULL;

    int len_path = strlen(path);
    int len_doc = strlen(jsondoc);

    char bufferpos=-1;
    char buffer[256];
    memset(buffer,0,256);

    int last_json_pos = 0;
    
    int i;
    for (i=0; i < len_path; i++){
        
        switch (path[i]){
            case '.':
            case '[':       
                last_json_pos = search_key(last_json_pos, buffer, jsondoc, len_doc,NULL);
                
                bufferpos = -1;
                memset(buffer,0,256);
                if (last_json_pos == -1)
                    return NULL;
                break;
            case ']':
                memset(buffer,0,256);
                if (last_json_pos == -1)
                    return NULL;
                break;
            default:
                bufferpos++;
                buffer[bufferpos] = path[i];
                break;
        }
        
        if (i == (len_path -1)){
            last_json_pos = search_key(last_json_pos, buffer, jsondoc, len_doc,NULL);

            if (last_json_pos !=-1){
                char *value=malloc(0);
                search_key(last_json_pos, buffer, jsondoc, len_doc, value);
                return value;
            }
        }
            
    }

    return outdata;

}

int search_key(int startpos, char *key, char *jsondoc,int len_doc, char*valuestring){
    int key_len = strlen(key);

    int iscontrolchar =0;

    int bufferpos = -1;
    char buffer[256];
    memset(buffer,0,256);

    int bracketcount=0;
    int isfirstbracket=0;

    int iskey = 1;
    int iskeyvalid = 1;
    
    int i;
    for (i=startpos;i<len_doc;i++){
        
        switch(jsondoc[i]){
            case '[':
            case '{':
                if (i == startpos+1)
                    isfirstbracket =1;
                bracketcount++;
                iscontrolchar =1;
                break;
            case '}':
            case ']':
                bracketcount--;
                iscontrolchar =1;
                break;
            case ' ':
            case '\r':
            case '\n':
            case '\t':
                iscontrolchar = 1;
                break;
            case ':':
                if (bracketcount==1){
                    if (key_len > (bufferpos+1) ) iskeyvalid = 0;
                    iskey = 0;
                }
                iscontrolchar =1;
                break;
            case ',':
                if (bracketcount==0){
                    iskey =1;
                    iskeyvalid =1;
                }
                iscontrolchar =1;
                break;
            case '\'':
            case '\"':
                if (isfirstbracket) iscontrolchar =1;
                else iscontrolchar =2;               
                break;
            default:
                iscontrolchar =0;
        }

        if (valuestring){
            if(bracketcount <0){
                strcpy(valuestring, buffer);
                return -1;
            } else if (iscontrolchar != 2){
                bufferpos++;
                buffer[bufferpos] = jsondoc[i];                
            }
            continue;
        }

        if (!iscontrolchar && iskey){
            bufferpos++;
            buffer[bufferpos] = jsondoc[i];
            if (bufferpos <= key_len){
                if (buffer[bufferpos] != key[bufferpos]){
                    iskeyvalid =0;
                }
            }else iskeyvalid =0;
        }

        if (iskey ==0 && iskeyvalid){
            return i;
        }
        
    }
    return -1;
}
