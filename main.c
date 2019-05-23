#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TOKEN_COUNT 50

typedef enum{
    UNDEFINED = 0,
    OBJECT = 1,
    ARRAY = 2,
    STRING = 3,
    PRIMITIVE = 4
} TYPE_T;

typedef struct token {
    TYPE_T type;
    int start;
    int end;
    int size;
    int flag; // token size
    struct token* next; // point next node
}TOKEN_T;

int getFileSize(char *filename);
void JsonParser(char *allContent, int contentSize, TOKEN_T *list);

int main(int argc, char **argv) {
    int sizeOfFile = -1;
    char buffer[64];

    FILE *fp = NULL;
    TOKEN_T *tokenList = malloc(sizeof(TOKEN_T) * TOKEN_COUNT);
    
    if(argc < 2){
        printf("usage: ./out <filename>\n");
        return -1;
    }
    
    fp = fopen(argv[1], "r");
    if(fp == NULL){
        printf("fail to open file %s\n", argv[1]);
        return -1;
    }
    
    sizeOfFile = getFileSize(argv[1]);
    char allContent[sizeOfFile];
    
    while(fscanf(fp, "%s", buffer) != EOF) {
        strcat(allContent, buffer);
        strcat(allContent, " ");
    }
    
    JsonParser(allContent, sizeOfFile, tokenList);

    return 0;
}

//get length of file content
int getFileSize(char *filename){
    FILE *fp = fopen(filename, "r");
    int len = -1;
    if(fp == NULL) {
        printf("fail to open file\n");
    } else {
        fseek(fp, 0, SEEK_END); 
        len = (int)ftell(fp); 
    }
    
    return len;
}


void JsonParser(char *allContent, int contentSize, TOKEN_T *list){
    int cur = 1;
    int numOfToken = 0;
    
    if(allContent[cur] != '{'){
        return ;
    }
    
    cur++;
    
    while(cur < contentSize){
        switch (allContent[cur]) { //doc[pos]
            case '"':
            {
                char *begin = allContent + cur + 1;
                char *end = strchr(begin, '"');
                char temp[contentSize];

                if(end == NULL) break;
                
                int wordLen = end - begin;
                
                strncpy(temp, begin, wordLen);
                temp[wordLen] = '\0';
                numOfToken++;
                
                //print info of each token
                printf("[%2d] %s (size=0, %lu~%lu)\n", numOfToken, temp, strlen(allContent) - strlen(begin), strlen(allContent) - strlen(end));

                cur = cur + wordLen + 1;
                //tokenList에 저장하는거
                //token size
            }
                break;
            case '[':
            {
                cur++;
                char temp[contentSize];

                while( allContent[cur] != ']' ){
                    if( allContent[cur] == '"')
                    {
                        char *begin = allContent + cur + 1;
                        char *end = strchr(begin, '"');// '"'로 시작하는 문자열을 end에
                        if( end==NULL ) break;

                        int wordLen = end - begin;

                        strncpy(temp, begin, wordLen);
                        temp[wordLen] = '\0';
                        cur = cur + wordLen + 1;
                        printf("[%2d] %s (size=0, %lu~%lu)\n", numOfToken, temp, strlen(allContent) - strlen(begin), strlen(allContent) - strlen(end));
                    }
                    numOfToken++;
                    //print info of each token
                }
            break;
            }   
            default:
                break;
        }
        cur++;
    }
    
}

