#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>



char** tokenator9000(char* text,int *tokens)
{

    int t=strlen(text);
    text[t-1]='\0';
    char** product=malloc(sizeof(char*));
    int start=0;
    int i=0;
    while(1){
        if(text[i]=='\0'){
            if(i==0){
                product[*tokens]="\0";
                break;
            }
            else if(text[i-1]==' '){
                if(*tokens==0){
                    product[*tokens]="\0";
                    break;
                }
                else{
                    product=realloc(product,(*tokens+1) * sizeof(char*));
                    product[*tokens]=NULL;
                    break;
                }
            }
            else{
                product=realloc(product,(*tokens+2) * sizeof(char*));
                int space=i-start;
                product[*tokens]=malloc(space+1);
                product[*tokens][space]='\0';
                int j;
                int k=0;
                for(j=start;j<i;j++){
                    product[*tokens][k]=text[j];
                    k++;
                }
                product[*tokens+1]=NULL;
                *tokens=*tokens+1;
                break;

            }
        }
        else{
            if(text[i]==' '){
                if(text[i-1]==' '){
                    start=i+1;
                }
                else if(i==0){
                    start=i+1;
                }
                else{
                product=realloc(product,(*tokens+1) * sizeof(char*));
                int space=i-start;
                product[*tokens]=malloc(space+1);
                product[*tokens][space]='\0';
                int j;
                int k=0;
                for(j=start;j<i;j++){
                    product[*tokens][k]=text[j];
                    k++;
                }
                start=i+1;
                *tokens=*tokens+1;
                }
            }
        }
        i=i+1;
    }
    return product;
}


int main()
{
    char request[250];
    while(0==0){
        char dir[250];
        getcwd(dir,sizeof(dir));
        printf("User@%s$ ",dir);
        fgets(request,sizeof(request),stdin);
        int amount=0;
        char** processed;
        processed=tokenator9000(request,&amount);
        if(amount==0){
            continue;
        }
        else if(strcmp(processed[0],"exit")==0){
            break;
        }
        else if(strcmp(processed[0],"cd")==0){
            int res=chdir(processed[1]);
            if(res==-1){
                perror("An error occured");
            }
            continue;
        }
        else if(strcmp(processed[0],"rm")==0){
            int result=remove(processed[1]);
            if(result==-1){
                perror("An error occured");
            }
            continue;
        }
        else if(strcmp(processed[0],"head")==0){
            if(amount==1){
                char temp[8192];
                int i;
                for(i=0;i<10;i++){
                    fgets(temp,sizeof(temp),stdin);
                    printf("%s",temp);
                }
            }
            else if(strcmp(processed[1],"-n")==0 && amount==4){
                int fd=open(processed[3],O_RDONLY);
                if(fd==-1){
                    perror("An error occured");
                    continue;
                }
                int n=atoi(processed[2]);
                char buf[8192];
                int s=read(fd,buf,8192);
                if(s==-1){
                    perror("An error occured");
                    continue;
                }
              	close(fd);
                int lines=0;
                int i=0;
                while(lines<n && buf[i]!='\0'){
                    if(buf[i]=='\n'){
                        lines++;
                    }
                    i++;
                }
                int j;
                for(j=0;j<i;j++){
                    printf("%c",buf[j]);
                }
            }
            else if(strcmp(processed[1],"-n")==0 && amount==3){
                char temp[8192];
                int i;
                int n=atoi(processed[2]);
                for(i=0;i<n;i++){
                    fgets(temp,sizeof(temp),stdin);
                    printf("%s",temp);
                }
            }
            else{
                int fd=open(processed[1],O_RDONLY);
                if(fd==-1){
                    perror("An error occured");
                    continue;
                }
                char buf[8192];
                int s=read(fd,buf,8192);
                if(s==-1){
                    perror("An error occured");
                    continue;
                }
              	close(fd);
                int lines=0;
                int i=0;
                while(lines<10 && buf[i]!='\0'){
                    if(buf[i]=='\n'){
                        lines++;
                    }
                    i++;
                }
                int j;
                for(j=0;j<i;j++){
                    printf("%c",buf[j]);
                }
            }
            continue;
        }
        else if(strcmp(processed[0],"help")==0){
            printf("\n");
            printf("                    (.'.'.'.'.'.)                           \n");
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            printf("\n");
            printf("\n");
            printf("                  Microshell Project                        \n");
            printf("               Created by Dawid Piskadlo                    \n");
            printf("\n");
            printf("                        MMXIX                               \n");
            printf("\n");
            printf("     Simplified implementation of a UNIX shell              \n");
            printf("\n");
            printf("                  Built-in commands:                        \n");
            printf("                        exit                                \n");
            printf("                         cd                                 \n");
            printf("                         rm                                 \n");
            printf("                        head                                \n");
            printf("\n");
            printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
            printf("                    (.'.'.'.'.'.)                           \n");
            printf("\n");
            continue;
        }
        else{
            int pid=fork();
            if(pid>=0){
                if(pid==0){
                   execvp(processed[0],processed);
                   perror("An error occured");
                   exit(0);
                }
                else{
                    wait(0);
                }
            }
        }
        free(processed);
    }
    return 0;
}