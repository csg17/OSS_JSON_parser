#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TOKEN_COUNT 50

typedef enum
{
    UNDEFINED = 0,
    OBJECT = 1,
    ARRAY = 2,
    STRING = 3,
    PRIMITIVE = 4
} TYPE_T;

typedef struct token
{
    TYPE_T type;
    int start;
    int end;
    int size;
    struct token *value;
    struct token *next; // point next node
} TOKEN_T;

int getFileSize(char *filename);
void JsonParser(char *allContent, int contentSize, TOKEN_T *list, int base);
void Pushtoken(TOKEN_T *head, TOKEN_T *data);
void printToken(TOKEN_T *head, char* allContent);
int numOfToken = 0;

int main(int argc, char **argv)
{
    int sizeOfFile = -1;
    char buffer[64];
    
    FILE *fp = NULL;
    //tokenList의 첫 node는 next를 제외하고 모두 null입니다.
    TOKEN_T *tokenList = malloc(sizeof(TOKEN_T));
    
    tokenList->value = NULL;
    tokenList->next = NULL;
    
    if (argc < 2)
    {
        printf("usage: ./out <filename>\n");
        return -1;
    }
    
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("fail to open file %s\n", argv[1]);
        return -1;
    }
    
    sizeOfFile = getFileSize(argv[1]);
    printf("size of file %d \n", sizeOfFile);
    char allContent[sizeOfFile];
    
    while (fscanf(fp, "%s", buffer) != EOF)
    {
        strcat(allContent, buffer);
        strcat(allContent, " ");
    }
    
    JsonParser(allContent, sizeOfFile, tokenList, 0);
    printf("Parsor done!\n");
    
    // printToken(tokenList, allContent);
    
    
    return 0;
}

//get length of file content
int getFileSize(char *filename)
{
    FILE *fp = fopen(filename, "r");
    int len = -1;
    if (fp == NULL)
    {
        printf("fail to open file\n");
    }
    else
    {
        fseek(fp, 0, SEEK_END);
        len = (int)ftell(fp);
    }
    
    return len;
}

void Pushtoken(TOKEN_T *head, TOKEN_T *data)
{
    TOKEN_T *tail = head;
    //fine end of list
    while (tail->next)
        tail = tail->next;
    
    //data is key
    if (data->size == 1){
        tail->next = data;
    }
    //data is value
    
    else if (data->size == 0){
        switch (data->type){
            case OBJECT:{
                tail->value = data;
                break;
            }
                
            case ARRAY:{
                //find end of array
                while(tail->value) tail = tail->value;
                tail->value = data;
                break;
            }
                
            case STRING:{
                tail->value = data;
                break;
            }
            default:{
                printf("What is the token's type?\n");
            }
        }
    }
    else
        printf("fail to add token in list\n");
}
void printToken(TOKEN_T *head, char* allContent){
    TOKEN_T *temp = head;
    
    do{
        if(temp->value == NULL) continue;
        else if(temp->value->type == OBJECT){
            printToken(temp->value, allContent);
        }
        else if(temp->value->type == ARRAY){
            while(temp->value){
                printf("\n");
                temp = temp->value;
            }
        }
        else {
            printf("STRING : %d\n", temp->value->start);
        }
        temp = temp->next;
    }while(temp);
}


void JsonParser(char *allContent, int contentSize, TOKEN_T *list, int base)
{
    int cur = base;
    int flag = 0;
    //int numOfToken = 0;
    // 이거 있으면 ARRAY에 원소 2개 이상일 때 동작 안함.
    // if(allContent[cur] != '{'){
    //     return ;
    // }
    
    cur++;
    
    while(cur < contentSize){
        switch (allContent[cur]) { //doc[pos]
                
                //string
            case '"':
            {
                char prev = allContent[cur-2];
                int flag;
                char *begin = allContent + cur + 1;
                char *end = strchr(begin, '"');
                char temp[contentSize];
                
                if(end == NULL) break;
                
                int wordLen = end - begin;
                
                strncpy(temp, begin, wordLen);
                temp[wordLen] = '\0';
                numOfToken++;
                //
                if(prev==':')
                    flag=0;
                else
                    flag=1;
                //print info of each token
                printf("[%2d] %s (size=%d, %lu~%lu)\n", numOfToken, temp,flag, strlen(allContent) - strlen(begin), strlen(allContent) - strlen(end));
                
                cur = cur + wordLen + 1;
                //tokenList에 저장하는거
                TOKEN_T *t_token = malloc(sizeof(TOKEN_T));
                
                // t_token->type = STRING;
                // t_token->start = strlen(allContent) - strlen(begin);
                // t_token->end = strlen(allContent) - strlen(end);
                // t_token->size = flag;
                // t_token->value = NULL;
                // t_token->next = NULL;
                
                // if(flag) flag = 0;
                // else flag = 1;
                
                // Pushtoken(list, t_token);
                
                //token size
            }
                break;
                
                //array
            case '[':
            {
                char temp[contentSize]; // 문자열 토큰
                char arrStr[contentSize]; // 문자열 배열 토큰
                char *beginArr, *endArr;
                TOKEN_T *tokenListObject = malloc(sizeof(TOKEN_T));
                int wordLen;
                
                if( allContent[cur] == '[' ){
                    beginArr = allContent + cur;
                    endArr = strchr(beginArr, ']');
                    wordLen = endArr - beginArr + 1;
                    strncpy(arrStr, beginArr, wordLen);
                    arrStr[wordLen] = '\0';
                    numOfToken++;
                    printf("[%2d] %s (size=1, %lu~%lu, ARRAY)\n", numOfToken, arrStr, strlen(allContent) - strlen(beginArr), strlen(allContent) - strlen(endArr));
                }
                JsonParser(allContent, wordLen + cur, list, cur);
                cur = cur + wordLen + 1;
                
                break;
            }
                
                //object
            case '{':
            {
                char temp[contentSize]; // 객체 저장
                char *begin = allContent + cur;
                char *cp = begin;
                int depth = 0;
                // if( allContent[cur] == '{') { begin = allContent + cur; }
                char *end = NULL;
                do{
                    if(*cp == '{') depth++;
                    else if(*cp == '}') depth--;
                    cp++;
                }while(depth);

                // end = strchr(begin+1, '}');
                end = cp;
                int wordLen = end - begin;


                strncpy(temp, begin, wordLen);
                temp[wordLen] = '\0';
                //
                int flag=0;
                for(int i=0; i<wordLen; i++){
                    if(allContent[cur+i]==',')
                        flag++;
                }
                flag++;
                numOfToken++;
                printf("[%2d] %s (size=%d, %lu~%lu)\n", numOfToken, temp, flag, strlen(allContent) - strlen(begin), strlen(allContent) - strlen(end));
                
                JsonParser(allContent, wordLen + cur, list, cur);
                cur = cur + wordLen + 1;
                
                break;
            }
            case '}':
                return;
                
                //numbers
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case'7': case '8': case '9': case '-':
            {
                char prev = allContent[cur-2];
                char *begin = allContent + cur;
                char *end;
                char *buffer;
                int num = 0;
                
                end = strchr( allContent + cur, ',' );
                if( end  == NULL ) {
                    end = strchr( allContent + cur, '}');
                    if ( end == NULL ) break;
                }
                
                int stringLength = end - begin;
                buffer = malloc(stringLength + 1);
                
                strncpy(buffer, begin, stringLength);
                num = atoi(buffer);
                numOfToken++;
                
                //
                if(prev==':')
                    flag=0;
                else
                    flag=1;
                //
                printf("[%2d] %s (size=%d, %lu~%lu)\n", numOfToken, buffer,flag, strlen(allContent) - strlen(begin), strlen(allContent) - strlen(end));
                cur = cur + stringLength + 1;
                break;
            }
            default:
                break;
        }
        cur++;
    }
}
