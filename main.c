#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
    UNDEFINED = 0,
    OBJECT = 1,
    ARRAY = 2,
    STRING = 3,
    PRIMITIVE = 4
} TYPE_T;

typedef struct token{
    TYPE_T type;
    int start;
    int end;
    int size;
    char keyVal[100000];
    struct token *value;
    struct token *next; // point next node
} TOKEN_T;

#define MAX 100
int getFileSize(char *filename);
void JsonParser(char *allContent, int contentSize, TOKEN_T *list, int base);
void Pushtoken(TOKEN_T *head, TOKEN_T *data);
void printToken(TOKEN_T *head, char* allContent);
int numOfToken = 0;
int tf, sz, total[MAX], i=0, aa=0;

int main(int argc, char **argv)
{
    int sizeOfFile = -1;
    char buffer[64];
    
    FILE *fp = NULL;
    //tokenList의 첫 node는 next를 제외하고 모두 null입니다.
    TOKEN_T *tokenList = malloc(sizeof(TOKEN_T));
    
    tokenList->value = NULL;
    tokenList->next = NULL;
    
    if (argc < 2){
        printf("usage: ./main <filename>\n");
        return -1;
    }
    
    fp = fopen(argv[1], "r");
    if (fp == NULL){
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
    
    JsonParser(allContent, sizeOfFile, tokenList, 1);
    printf("Parsor done!\n");
    
    // printToken(tokenList, allContent);
    
    
    return 0;
}

//get length of file content
int getFileSize(char *filename){
    FILE *fp = fopen(filename, "r");
    int len = -1;
    if (fp == NULL){
        printf("fail to open file\n");
    } else{
        fseek(fp, 0, SEEK_END);
        len = (int)ftell(fp);
    }
    
    return len;
}

void Pushtoken(TOKEN_T *head, TOKEN_T *data){
    TOKEN_T *tail = head;
    
    //fine end of list
    while (tail->next){
        tail = tail->next;
    }
    
    //data is key
    if (data->size == 1){
        tail->next = data;
    }
    
    //data is value
    else if (data->size == 0 || data->size > 1){
        switch (data->type){
            case OBJECT:{
                tail->next = data;
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
            case PRIMITIVE:{
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
    printf("in print token\n");
    
    TOKEN_T *temp = head;
    
    while(temp != NULL){
        
        if(temp->value->type == OBJECT){
            printToken(temp->value, allContent);
        }
        else if(temp->value->type == ARRAY){
            int kWordLen = temp->end - temp->start;
            char kbuf[kWordLen];
            strncpy(kbuf, allContent + temp->start, kWordLen);
            kbuf[kWordLen] = '\0';
            printf("%s : ", kbuf);
            
            while(temp->value){
                printf("ARRAY elements : %d", temp->value->start);
                temp = temp->value;
            }
        }
        else if(temp->value->type == STRING){
            int kWordLen = temp->end - temp->start;
            char kbuf[kWordLen];
            strncpy(kbuf, allContent + temp->start, kWordLen);
            kbuf[kWordLen] = '\0';
            printf("%s : ", kbuf);
            
            int vWordLen = temp->value->end - temp->value->start;
            char buf[vWordLen];
            strncpy(buf, allContent + temp->value->start, vWordLen);
            buf[vWordLen] = '\0';
            printf("%s\n", buf);
        }
        else if(temp->value->type == PRIMITIVE){
            int kWordLen = temp->end - temp->start;
            char kbuf[kWordLen];
            strncpy(kbuf, allContent + temp->start, kWordLen);
            kbuf[kWordLen] = '\0';
            printf("%s : ", kbuf);
            
            int vWordLen = temp->value->end - temp->value->start;
            char buf[vWordLen];
            strncpy(buf, allContent + temp->value->start, vWordLen);
            buf[vWordLen] = '\0';
            printf("%s\n", buf);
            }
        
        temp = temp->next;
        
    };
}

void JsonParser(char *allContent, int contentSize, TOKEN_T *list, int base)
{
    int cur = base;
    int flag = 0;
    //TOKEN_T *t_token = malloc(sizeof(TOKEN_T));
    //int numOfToken = 0;
    // 이거 있으면 ARRAY에 원소 2개 이상일 때 동작 안함.
    // if(allContent[cur] != '{'){
    //     return ;
    // }
    int exception=0;
    cur++;
    //int i=0;
    
    while(cur < contentSize){
        TOKEN_T *t_token = malloc(sizeof(TOKEN_T));
        switch (allContent[cur]) { //doc[pos]
                
            //string
            case '"' : {
                int prev=0;
                int prev1=0;
                int z;
                int flag=1;
                char *begin = allContent + cur + 1;
                char *end = strchr(begin, '"');
                char temp[contentSize];
                
                
                
                if(end == NULL) break;
                
                int wordLen = end - begin;
                
                strncpy(temp, begin, wordLen);
                temp[wordLen] = '\0';
                numOfToken++;
                //
                for( z=0;cur-z>0;z++){
                    
                     if(allContent[cur-1-z]=='"'){
                     prev = cur-z-1;
                    break;}
                }
                if(prev!=0){
                char *begin1 = allContent + prev;
                char *end1 =  allContent + cur;
                int length3 = end1 - begin1;

                char temp5[length3];
                strncpy(temp5,begin1,length3);
                temp5[length3] = '\0';
                
                if(strchr(temp5,':')!=NULL){
                flag = 0;
                if(strchr(temp5,'{')!=NULL)
                flag =1;
                }
                else
                flag =1;
                }
                int prev2=0;
                 for( int q=0;cur-q>0;q++){
                    
                     if(allContent[cur-1-q]=='"'){
                     prev2 = cur-q-1;
                    break;}
                }
                if(prev2!=0){
                char *begin2 = allContent + prev2;
                char *end2 =  allContent + cur;
                int length4 = end2 - begin2;

                char temp6[length4];
                strncpy(temp6,begin2,length4);
                temp6[length4] = '\0';

                if(strchr(temp6,',')!=NULL)
                flag =1;
            
                }

                if( tf==1 ) {
                   //printf("tf: %d, sz: %d \n", tf,sz );
                    if(sz==0) { 
                        tf=0;
                    }
                    else{
                    //total[i] = wordLen;
                    //printf("3");
                    aa=5;
                    flag = 0;
                    sz--; i++;
                    }
                }
                                  
                   
                //print info of each token
                printf("[%2d] %s (size=%d, %lu~%lu, String)\n", numOfToken, temp,flag, strlen(allContent) - strlen(begin), strlen(allContent) - strlen(end));
                
                cur = cur + wordLen + 1;
                
                t_token->type = STRING;
                t_token->start = strlen(allContent) - strlen(begin);
                t_token->end = strlen(allContent) - strlen(end);
                t_token->size = flag;
                t_token->value = NULL;
                t_token->next = NULL;
                strcpy(t_token->keyVal, temp);
                
                Pushtoken(list, t_token);
                break;
            }
                //array
            case '[' : {
                char temp[contentSize]; // 문자열 토큰
                char arrStr[contentSize]; // 문자열 배열 토큰
                char *beginArr, *endArr;
                TOKEN_T *tokenListObject = malloc(sizeof(TOKEN_T));
                int wordLen;
                tf = 1;
                int countArr;
                
                if( allContent[cur] == '[' ){
                    beginArr = allContent + cur;
                    endArr = strchr(beginArr, ']');
                    wordLen = endArr - beginArr + 1;
                    strncpy(arrStr, beginArr, wordLen);
                    arrStr[wordLen] = '\0';
                    numOfToken++;
                    countArr=0;
                    int ifObject=0;
                    for(int i=0; i<wordLen; i++){
                        if(allContent[cur+i]=='{'){
                            ifObject++;
                        }
                        if(allContent[cur+i]==',')
                            countArr++;
                    }
                    
                    countArr++;
                    sz = countArr;
                    
                    t_token->type = ARRAY;
                    t_token->start = strlen(allContent) - strlen(beginArr);
                    t_token->end = strlen(allContent) - strlen(endArr);
                    t_token->size = flag;
                    t_token->value = NULL;
                    t_token->next = NULL;
                    strcpy(t_token->keyVal, arrStr);
                    Pushtoken(list, t_token);
                    
                    if(ifObject!=0)
                        printf("[%2d] %s (size=%d, %lu~%lu, ARRAY)\n", numOfToken, arrStr,ifObject, strlen(allContent) - strlen(beginArr), strlen(allContent) - strlen(endArr));
                    else
                        printf("[%2d] %s (size=%d, %lu~%lu, ARRAY)\n", numOfToken, arrStr,countArr, strlen(allContent) - strlen(beginArr), strlen(allContent) - strlen(endArr));
                    
                }
                
                JsonParser(allContent, wordLen + cur, list, cur);
                
                cur += wordLen-1;
                
                break;
            }
                //object
            case '{' : {
                char temp[contentSize]; // 객체 저장
                char *begin = allContent + cur;
                char *cp = begin;
                int depth = 0;
                char *object=begin;
                char *end = NULL;
                do{
                    if(*cp == '{')
                        depth++;
                    else if(*cp == '}') depth--;
                    cp++;
                }while(depth);
                
                end = cp;
                int wordLen = end - begin;
                
                
                strncpy(temp, begin, wordLen);
                temp[wordLen] = '\0';
                
                
                if(strchr(temp+1,'{')!=NULL){
                    object++;
                    while(*object!='{'){
                        object++;
                    }
                    
                    while(*object !='}'){
                        if(*object==',')
                            exception++;
                        object++;
                    }
                }
                
                
                int flag=0;
                for(int i=0; i<wordLen; i++){
                    if(allContent[cur+i]==',')
                        flag++;
                }
                flag++;
                numOfToken++;
                TOKEN_T *dummy = malloc(sizeof(TOKEN_T));
                dummy->type = STRING;
                dummy->value = NULL;
                dummy->next = t_token;
                
                t_token->type = OBJECT;
                t_token->start = strlen(allContent) - strlen(begin);
                t_token->end = strlen(allContent) - strlen(end);
                t_token->size = flag;
                t_token->value = NULL;
                t_token->next = NULL;
                strcpy(t_token->keyVal, temp);

                Pushtoken(list, dummy);
                printf("[%2d] %s (size=%d, %lu~%lu, OBJECT)\n", numOfToken, temp, flag-exception, strlen(allContent) - strlen(begin), strlen(allContent) - strlen(end));
                
                JsonParser(allContent, wordLen + cur, dummy, cur);
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
                
                t_token->type = PRIMITIVE;
                t_token->start = strlen(allContent) - strlen(begin);
                t_token->end = strlen(allContent) - strlen(end);
                t_token->size = flag;
                t_token->value = NULL;
                t_token->next = NULL;
                strcpy(t_token->keyVal, buffer);
                
                Pushtoken(list, t_token);
                printf("[%2d] %s (size=0, %lu~%lu, PRIMITIVE)\n", numOfToken, buffer, strlen(allContent) - strlen(begin), strlen(allContent) - strlen(end));
                cur = cur + stringLength + 1;
                break;
            }
            default:
                break;
        }
        cur++;
    }
}