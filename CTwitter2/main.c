#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include<string.h>
#include<windows.h>
#include"slre.h"
#include "cJSON.h"
#include"slre.h"
#define MAX 100000
int m_index=0;
int initialize(int);
char *send_data(char *);
void SignUp();
void Login();
struct sockaddr_in server;
void menu(char *);
void timeline(char *);
void search(char *);
void tweet_profile(char *);
void personal_area(char *);
void logout(char *);
void send_tweet(char *);
void refresh(char *);

int main() {
    int can_initialize=initialize(12345);
    while  (can_initialize==0) {
        printf("Welcome!\n1.Login\n");
        printf("2.Don't have an account?Sign Up\n");
        printf("3.Exit\n");
        int choice;
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                Login(NULL);
                break;
            case 2:
                SignUp(NULL);
                break;
            case 3:
                exit(0);
            default:
                exit(0);
        }


    }
    return 0;
}
int initialize(int port){
    WSADATA wsaData;
    int wsaStartUp=WSAStartup(MAKEWORD(2,2),&wsaData);
    if(wsaStartUp != 0){
        printf("Error\n");
        return -1;
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);
    return 0;
}
char *send_data(char *data){

    char *buffer = malloc(1000);
    memset(buffer,0,1000);

    int client_socket = socket(AF_INET,SOCK_STREAM,0);
    if(client_socket == INVALID_SOCKET){
        return buffer;
    }

    int can_connect = connect(client_socket,(struct sockaddr*)&server,sizeof(server));

    if(can_connect != 0){
        return buffer;
    }
    send(client_socket,data,strlen(data),0);
    recv(client_socket,buffer,999,0);
    closesocket(client_socket);
    return buffer;
}
void SignUp() {
    char username[100],password[100],password1[100];
    while (1) {
        printf("Choose your Username:\n");
        scanf("%s", username);
        printf("Choose your Password:\n");
        scanf("%s", password);
        printf("Verify Password:\n");
        scanf("%s", password1);
        if(strcmp(password,password1)!=0){
            printf("ERROR:PASSWORDS DON'T MATCH.TRY AGAIN\n");
            continue;
        }
        else{
            break;
        }
    }
    char signupdata[115]={0};
    strcat(signupdata,"signup ");
    strcat(signupdata,username);
    strcat(signupdata,", ");
    strcat(signupdata,password);
    strcat(signupdata,"\n");
    printf("%s",signupdata);
    char *feedback1=send_data(signupdata);
    printf("%s\n",feedback1);

    if (feedback1[27]=='T'){
        printf("This username is not allowed.Try another one\n");
    }
    else{
        printf("Hi %s.You signed up successfully.\nNow login to get started.\n",username);
        Login();
    }
}
void Login() {
    char username[100], password[100];
    printf("Enter Username:\n");
    scanf("%s", username);
    printf("Enter Password:\n");
    scanf("%s", password);
    char logindata[115] = {0};
    printf("%s\n", logindata);
    strcat(logindata, "login ");
    strcat(logindata, username);
    strcat(logindata, ", ");
    strcat(logindata, password);
    strcat(logindata, "\n");
    printf("%s", logindata);
    char *feedback = send_data(logindata);
    printf("%s", feedback);
    if ((feedback[9]=='E')&&(feedback[29]=='e')){
        printf("You are already logged in.Disconnect the server and Login again\n");
        exit(0);
    } else if ((feedback[9]=='E')&&(feedback[29]=='i')) {
        printf("This username is not valid \n");
    }else if((feedback[9]=='E')&&(feedback[29]=='c')) {
        printf("Wrong Password.\nTRY AGAIN\n");
        Login();
    }
        else {
            int num_caps = 2;
            char caps[num_caps][100000];
            char regex[] = "{\"type\":\"([A-Za-z]+)\",\"message\":\"([A-Za-z0-9\-\_ ]+)\"}";
            slre_match(regex, feedback, 100000, caps, num_caps, 0);
            /*char error[100] = {0};
            strcat(error, "Error");
            char already_loggedin[100] = {0};
            strcat(already_loggedin, "The user ");
            strcat(already_loggedin, username);
            strcat(already_loggedin, "is already logged in");
            char invalid_username[100] = {0};
            strcat(invalid_username, "This username is not valid");
            if (strcmp(caps[0], error) == 0) {
            if (strcmp(caps[1], already_loggedin) == 0) {

            } else if (strcmp(caps[1], invalid_username)) {
            printf("%s Try logging in again", caps[1]);
            Login();
            } else {
            printf("%s Try logging in again.\n", caps[1]);
            Login();
            }
            }*/
            printf("%s\n",caps[1]);
            menu(caps[1]);
        }
    }

void menu(char *Token){
    printf("Choose where to go:\n");
    printf("1.Timeline\n2.Search\n3.Tweet Profile\n4.Personal area\n5.Log out\n");
    int choice1;
    scanf("%d",&choice1);
    switch (choice1) {
        case 1:
            timeline(Token);
            break;
        case 2:
            search(Token);
            break;
        case 3:
            tweet_profile(Token);
            break;
        case 4:
            personal_area(Token);
            break;
        case 5:
            logout(Token);
            break;
    }

}
void timeline(char *Token){
    int choice2=0;
    while(choice2!=3) {
        printf("1.Send Tweet\n2.Refresh\n3.Back to Main menu\n");
        scanf("%d",&choice2);
        switch (choice2) {
            case 1:
                send_tweet(Token);
            case 2:
                refresh(Token);
            case 3:
                break;

        }
    }
    menu(Token);
}
void search(char *Token){
    char search_data[200]={0};
    printf("Search a username:\n");
    char search[20]="search ";
    char searcheduser[200]={0};
    scanf("%s",searcheduser);
    strcat(search_data,search);
    strcat(search_data,Token);
    strcat(search_data,", ");
    strcat(search_data,searcheduser);
    strcat(search_data,"\n");
    char *feedback4= send_data(search_data);
    cJSON *root = cJSON_Parse(feedback4);
   /* cJSON *type = cJSON_GetObjectItem(root, "type");
    if(type->valuestring[0]=='P'){
            printf("%s\n", type->valuestring);
            printf("%s\n",searcheduser);
    }*/
    cJSON *message= cJSON_GetObjectItem(root, "message");
    for (int i = 0; i <cJSON_GetArraySize(message);++i) {
        cJSON *subitem = cJSON_GetArrayItem(message,i);
        cJSON *username = cJSON_GetObjectItem(subitem,"username");
        cJSON *bio = cJSON_GetObjectItem(subitem,"bio");
        cJSON *numberOfFollowers = cJSON_GetObjectItem(subitem,"numberOfFollowers");
        cJSON *numberOfFollowings = cJSON_GetObjectItem(subitem,"numberOfFollowings");
        cJSON *followStatus = cJSON_GetObjectItem(subitem,"followStatus");
        printf("%s   t    \n",subitem[i].valuestring);
        printf("%d   t    \n",subitem[i].valueint);
    }

 // {"type":"Profile","message":{"username":"j","bio":"","numberOfFollowers":0,"numberOfFollowings":0,"followStatus":"NotFollowed","allTweets":[{"id":27,"author":"j","content":"sdf sdf  sdf sf","comments":{},"likes":0},{"id":28,"author":"j","content":"hey trump s sd w a d 24 990","comments":{},"likes":0}]}}

}
void tweet_profile(char *Token){
}
void personal_area(char *Token){
    int choice4=0;
    printf("1.set bio\n2.change password\n");
    scanf("%d",&choice4);
    //if()

}
void logout(char *Token){
    int verify=0;
printf("Are you sure you want to log out of your account?\n");
printf("1.Yes\n");
printf("2.No\n");
scanf("%d",&verify);
if (verify==1){
    char logout[200]={0};
    strcat(logout,"logout ");
    strcat(logout,Token);
    strcat(logout,"\n");
    char *feed=send_data(logout);
    printf("You logged out of your account.\n");
    printf("Do you want to close the program?\n1.Yes\n2.No,continue with Twitter\n");
    int status=0;
    scanf("%d",&status);
    if (status==1){
        exit(0);
    }
    else{

    }
}
}
void send_tweet(char *Token){
    printf("Write your tweet and press Enter:\n");
    char tweet[400];
    char tweet_data[500] = "send tweet ";
    scanf("%*c%[^\n]s", tweet);
    printf("%s\n", tweet);
    printf("%s\n", Token);
    strcat(tweet_data, Token);
    strcat(tweet_data, ", ");
    strcat(tweet_data, tweet);
    strcat(tweet_data, "\n");
    printf("%s", tweet_data);
    send_data(tweet_data);
    printf("YOUR TWEET HAS BEEN SENT SUCCESSFULLY\n");
    timeline(Token);
}
void refresh(char *Token) {
    char refresh_data[100] = {0};
    char *refresh = "refresh ";
    strcat(refresh_data, refresh);
    strcat(refresh_data, Token);
    strcat(refresh_data, "\n");
    printf("%s", refresh_data);
    char *feedback3 = send_data(refresh_data);
    cJSON *root = cJSON_Parse(feedback3);
    cJSON *username;
    printf("%s", feedback3);

}
