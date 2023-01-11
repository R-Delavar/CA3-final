#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <conio.h>
typedef struct user user;
/*the below structure saves the user's information*/
struct user{
    char* userName;
    char* password;
    int numberofPosts;
    user* next;
};
typedef struct message message;
/*the below structure saves the massage's information*/
struct message{
    char* content;
    char* username;
    char* password;
    int likes;
    int postID;
    message* next;
};
typedef struct likes likes;
/*the below structures saves the people who have liked the post*/
struct likes{
    char* username;
    int postID;
    likes* next;
};
int newUser(char name[], char password[],user* dummyUser){
    /*memory is allocated to save a new user. if not successful, the program returns 1*/
    user *newUser = (user *) malloc(sizeof(user)); 
    if( newUser == NULL){
        free(newUser);
        return 1;
    }
    /*the information of the new user is saved*/
    newUser->userName = name;       
    newUser->password = password;
    newUser->numberofPosts=0;
    newUser->next = NULL;
    /*the new user is added to the liked list after the dummyUser and before the last user added*/
    newUser->next=dummyUser->next;
    dummyUser->next=newUser;
    return 0;
}
void printPosts(user* users,message* dummyMessage){
    /*a file named posts is created in order to save the posts*/
    FILE* posts;
    posts=fopen("posts.txt","w");
    /*a temp user and temp message are created to iterate and save the info*/
    user* tempUser=users->next;
    message* tempMessage=dummyMessage->next;
    /*the loop searches the users, then saves the username for each one*/
    while(tempUser!=NULL){
        char* username=tempUser->userName;
        /*the loop searches the messages to find those written by a specific username, then prints the post, username and the number of likes*/
        while(tempMessage!=NULL){
            if(!strcmp(tempMessage->username,username)){
                int numberOfLikes=tempMessage->likes;
                char* content=tempMessage->content;
                fprintf(posts,"%s ",content);
                fprintf(posts,"%s ",username);
                fprintf(posts,"%d \n",numberOfLikes);
            }
            tempMessage=tempMessage->next;
        }
        /*temp message reaches NULL at the end of the loop so it has to point at the first node after dummy node again*/
        tempMessage=dummyMessage->next;
        tempUser=tempUser->next;
    }
}
void printUsers(user* dummyUser){
    /*a file named accounts is created in order to save the accounts information */
    FILE* accounts;
    accounts =fopen("accounts.txt","w");
    user* tempUser=dummyUser->next;
  while(tempUser != NULL){
        int numberofPosts=(tempUser->numberofPosts);
        char* name = tempUser->userName;
        char* password= tempUser->password;
        fprintf(accounts,"%s ",name);
        fprintf(accounts,"%s ",password);
        fprintf(accounts,"%d\n",numberofPosts);
        tempUser=tempUser->next;
    }
    /*the file will close and the information will be saved*/
    fclose(accounts);
  
}
user* searchUser(user* dummyUser,char name[],char password[],int state){
    /*dummy user is the first user of the list which is null*/
    int result=9;
    int result2=9;
    /*the loop below checks each node to find the wanted user*/
    while(dummyUser!=NULL){
        /*for each node, the username and password of the node and the entered username and password are checked to see if they match*/
            result=strcmp((dummyUser->userName),name);
            if(result==0){
                if(state==2){
                    result2=strcmp((dummyUser->password),password);
                    /*if they matched, the user will be returned*/
                    if(result2==0){
                        return dummyUser;
                    }
                    else{
                        /*if the password doesn't match the username, NULL will be returned*/
                        printf("password doesn't match the username");
                        return NULL;
                    }
                }
                if(state==1){
                    return dummyUser;
                }
            }
            dummyUser=dummyUser->next;       
    }
    /*if the username wasn't found, the function returns NULL and prints an error message*/
    if (result!=0){
        return  NULL;
    }
}
char* commandReader(){
    size_t size = 2; // 2 bytes can store a 1-character string
    char *input = malloc( size );
    size_t i = 0;
    int c;
    /**
    * we want to break on EOF and *any* whitespace
    * character
    */
    while( (c = getchar()) != EOF && !isspace( c ) )
    {
        if ( i == size )
        {
            /**
            * Double the size of the buffer; again, no need for a cast
            *
            * Since realloc will return NULL if it can't extend the
            * buffer, we want to assign the result to a temporary;
            * otherwise we risk overwriting the pointer value in name
            * with a NULL pointer and losing our access to the memory
            * that's been previously allocated.
            */
            char *tmp = realloc( input, size * 2 ); 
            if ( tmp )
                {
                input = tmp;
                size *= 2;
                }
            else
            {
                /**
                * We were unable to extend the buffer; we can treat this
                * as a fatal error and exit immediately,
                * The previous buffer is still allocated at this point. 
                */
                fputs( "Failed to extend name buffer, exiting...", stderr );
                free( input ); // clean up what we've already allocated
                exit( 0 );
            } 
        }
        input[i++] = c;
    }
    input[i] = '\0';
    return input;
}
int commandFinder(char *command){
    int result=-1;
    char commands[9][10]={
        "signup","login","post","like","logout","info","find_user","delete","exit"
    };
    for(int i=0;i<=8;i++){
        result=strcmp(command,commands[i]);
        if(result==0){
            return (i+1);
        }
    }
    return 0;
}
int messageSaver(message* dummyMessage,char* username,int numberofMessages,char content[],user* dummyUser){
    /*a temp user is created to iterate the list*/
    user* tempUser=dummyUser->next;
    message *newMessage = (message *) malloc(sizeof(message)); 
    /*if the memory allocation for the new message fails, the program returns 0*/
    if( newMessage == NULL){
        return 0;
    }
    /*the information of the new message is saved and the number of the posts the user has entered is increased by 1*/
    newMessage->content=content;
    newMessage->username=username;
    newMessage->postID=numberofMessages;
    newMessage->likes=0;
    newMessage->next=dummyMessage->next;
    dummyMessage->next=newMessage;
    while(tempUser!=NULL){
        if(!strcmp(tempUser->userName,username)){
            tempUser->numberofPosts=(tempUser->numberofPosts)+1;
        }
        tempUser=tempUser->next;
    }
    /*if the whole operation was successful, the function returns 1*/
    return 1;
}
char* postReader(){
    size_t size = 2; // 2 bytes can store a 1-character string
    char *input = malloc( size ); // no need for a cast
    size_t i = 0;
    int c;
    /**
    * we want to break on EOF and *any* whitespace
    * character
    */
    while( (c = getchar()) != EOF&& !iscntrl(c))
    {
        if ( i == size )
        {
            /**
            * Double the size of the buffer; again, no need for a cast
            *
            * Since realloc will return NULL if it can't extend the
            * buffer, we want to assign the result to a temporary;
            * otherwise we risk overwriting the pointer value in name
            * with a NULL pointer and losing our access to the memory
            * that's been previously allocated.
            */
            char *tmp = realloc( input, size * 2 ); 
            if ( tmp )
                {
                input = tmp;
                size *= 2;
                }
            else
            {
                /**
                * We were unable to extend the buffer; we can treat this
                * as a fatal error and exit immediately*/
                fputs( "Failed to extend name buffer, exiting...", stderr );
                free( input ); // clean up what we've already allocated
                exit( 0 );
            } 
        }
        input[i++] = c;
    }
    input[i] = '\0';
    return input;
}
int postLiking(char* name, int postID,message* dummyMessage,likes* dummyLikes,char* savedName){
    /*dummyMessage is the first message node*/
    likes* placeHolder=dummyLikes->next;
    /*the loop below searches all the nodes of likes to see if the user has liked the post before or not; if yes, it returns 0*/
    while(placeHolder!=NULL){
                    if(placeHolder->username==savedName){
                        if(placeHolder->postID==postID){
                            return 0;
                        }
                    }
                    placeHolder=placeHolder->next;
                }
    while(dummyMessage!=NULL){
        if(!strcmp(dummyMessage->username,name)){
            if(dummyMessage->postID==postID){
                /*memory is allocated to save a new like, if it fails, porgram returns2*/
                likes* newLike=malloc(sizeof(likes));
                if(newLike==NULL){
                    return 2;
                }
                else{
                    /*the information of the new like node is entered and the likes if the post is increased by one; if successful, the program returns 1*/
                    newLike->username=savedName;
                    newLike->postID=postID;
                    newLike->next=dummyLikes->next;
                    dummyLikes->next=newLike;
                    dummyMessage->likes+=1;
                    return 1;
                }
            }
        }
    dummyMessage=dummyMessage->next;    
    }
    printf("post does not exist\n");
}
void postSearcher(message* dummyMessage,char* username, char* password){
    int counter=0;
    message* tempMessage=dummyMessage->next;
    /*the loop below searches all the message to find the post with the wanted username and prints them*/
    while(tempMessage!=NULL){
        if(!strcmp(tempMessage->username,username)){
            printf("post: %s \n postID:%d\n likes:%d\n\n",tempMessage->content,tempMessage->postID,tempMessage->likes);
            counter++;
        }
        tempMessage=tempMessage->next;
    }
    if(counter==0){
        printf("no messages have been posted yet!\n");
    }
}
int deletePost(message* dummyMessage,int postID, char* name1){
    message* tempMessage;
    tempMessage=dummyMessage;
    /*the loop below searches all the messages to find the message with the username of the user and the wanted postID*/
    /*using dummyMessage and tempMessage, the address of the node before the wanted node and the wnated node are saved*/
    /*this way, when the message is found, the address of the next message node will be passed to the previous messaage node*/
    /*then the memory will be freed and this way, a post will be deleted*/
    while(dummyMessage!=NULL){
        if(!strcmp(dummyMessage->username,name1)){
            if(dummyMessage->postID==postID){
                free(dummyMessage->content);
                tempMessage->next=dummyMessage->next;
                /*if the whole procedure was successful, the program returns 1*/
                return 1;
            }
        }
        tempMessage=dummyMessage;
        dummyMessage=dummyMessage->next;    
    }
    /*if the message wasn't found, the program returns 0*/
    return 0;
}
int main(){
    /*for each structure, a dummy one is defined for easier adding/searching*/
    user *dummyUser = (user *) malloc(sizeof(user));
    message* dummyMessage= (message *)malloc(sizeof(message));
    likes* dummyLikes=(likes *)malloc(sizeof(likes));
    dummyLikes->next=NULL;
    dummyUser->next=NULL;
    dummyMessage->next=NULL;
    char* command;
    char* name;
    char* password;
    char* message;
    char* savedName;
    char* savedPassword;
    int postID;
    int status=-1;
    int functionCheck=-1;
    int flag=1;
    int login=0;
    int numberofMessages=1;
    user* doer;
    user* finder;
    user* postowner;
    printf("Hello! You have to sign up/log in to enter the program!\n");
    while(flag){
        command=commandReader();
        status=commandFinder(command);
        if(!status){
            /*if the entered command doesn't exist, commandFinder returns 0 and the input buffer is flushed*/
            printf("no such command exists\n");
            while ((getchar()) != '\n');            
        }
        if (status){
            switch(status){
                case 1:{
                    /*the first command is signup, if the user has already logged in program shows an error and exits, else the account making procedure begins*/
                    if(!login){
                        name=commandReader();
                        if(atoi(name)){
                            printf("username must contain chars!\n");
                            break;
                        }
                        /*next function checks if the entered username already exists or not*/
                        user* finder=searchUser(dummyUser->next,name,password,1);
                        /*if the entered username doesn't exists, a new account will be created with the entered username and password*/
                        if(finder==NULL){
                            password=commandReader();
                            functionCheck=newUser(name,password,dummyUser);
                            /*if the account was built successfully, newUser returns 0 and a message saying the operation was successful appears*/
                            if(functionCheck==0){
                                printf("account made successfully! now log in to use other features!\n");
                            }
                            else
                            /*if memory allocation fails, function returns 1 and the program says the memory allocation failed */
                            {
                                printf("couldn't allocate suffiecent memory! please try again..\n");
                            }
                        }
                        else{
                        printf("username already exists, please try again and choose another name\n");
                        while ((getchar()) != '\n');            
                        }
                        status=-1;
                        functionCheck=-1;
                    }
                    else{
                        printf("you have to log out first!\n");
                    }
                    break;
                }
                case 2:{
                    /*the second command is login, if the user has logged in they have to log out first*/
                    if(!login){
                        name=commandReader();
                        if(atoi(name)){
                            printf("username must contain chars!\n");
                            break;
                        }
                        password=commandReader();
                        user* finder= (user* )malloc(sizeof(user));
                        /*the next function searches to see if an account exists with the entered username and passwords */
                        finder=searchUser(dummyUser->next,name,password,2);
                        /*if it exists, the account will be returned as a pointer and a message of opereation happened successfully appears*/
                        if(finder!=NULL){
                            printf("successfully logged in!\n");
                            login=1;
                            savedName=name;
                            savedPassword=password;
                            doer=finder;
                        }
                        else{
                            printf("could not log in\n");
                        }
                    }
                    else{
                        printf("you have to log out first!\n");
                    }
                    break;
                }
                case 3:{
                    /*the third operation is to post a message. the user has to login first, so if they haven't a message saying they have to login first apears and the program exits the command*/
                    if(login){
                        message=postReader();
                        /*the next function saves the message and increases the number of messages posted by user by 1*/
                        functionCheck=messageSaver(dummyMessage,savedName,numberofMessages,message,dummyUser);
                        /*if the message was saved successfully, the function returns 1, else it returns 0 and for both cases the program shows messages to inform the user*/
                        if (functionCheck==1){
                            printf("your message was saved successfully!\n");
                            numberofMessages++;
                        }
                        else if(functionCheck==0){
                            printf("memory allocation failed,couldn't save your message.\n");
                            functionCheck=-1;
                        }
                    }
                    else{
                        printf("you have to log in first!\n");
                    }
                    break;
                }
                case 4:{
                    /*the fourth command is to like a post. for which, user has to login; so if they haven't, an error message appears and the program exits the command*/
                    if(login){
                        int ID_ErrorCheck=-1;
                        name=commandReader();
                        if(atoi(name)){
                            printf("username must contain chars!\n");
                            break;
                        }
                        ID_ErrorCheck=atoi(name);
                        /*atoi is used to see if the user has entered a correct username. it returns 0 if the user enters character*/
                        if(ID_ErrorCheck){
                            printf("please enter the username correctly.\n");
                        }
                        /*atoi is used to see if the user enters a correct post ID. if yes, since the postIDs start from 1, the value must be greater than 0*/
                        ID_ErrorCheck=atoi(commandReader());
                        if(ID_ErrorCheck){
                            postID= ID_ErrorCheck;
                        }
                        else{
                            printf("wrong postID entered! exiting the command!\n");
                            break;
                        }
                        /*the next function, if successful, adds the user as one of the likers of the post, so they can't like the post again, and increases the likes of the post by 1*/
                        functionCheck=postLiking(name,postID,dummyMessage->next,dummyLikes,savedName);
                        /*the function returns 0 if the user has liked the post before and the program shows appropriate message*/
                        if(functionCheck==0){
                            printf("you have liked this post before!\n");
                        }
                        /*the function returns 1 if the post was liked successfully*/
                        else if(functionCheck==1){
                            printf("liked the post successfully!\n");
                        }
                        /*the function returns 2 if the memory allocation for saving the user as likers failed*/
                        else{
                            printf("could not allocate sufficient memory!\n");
                        }
                    }
                    else{
                        printf("you have to log in first!\n");
                    }
                    break;
                }
                case 5:{
                    /*the fifth command is logout, id the user hasn't logged in first the program shows an error and exits the command*/
                    if(login){
                        /*if the user has logged in, the value of login changes to 0 so other commands needing the user to be logged in won't work and the user successfully logs out*/
                        login=0;
                        printf("logged out successfully!\n");
                    }
                    else{
                        printf("you haven't logged in yet; exiting the command\n");
                    }
                    break;
                }
                case 6:{
                    /*the sixth command is to show users information. if they haven't logged in yet, the program exits the command and show an error*/
                    if(login){
                        /*the username and password are shown*/
                        printf("username:%s \n password:%s \n",doer->userName,doer->password);
                        /*the function below searches and finds all the posts which the user has written*/
                        postSearcher(dummyMessage,savedName,savedPassword);
                    }
                    else{
                        printf("you have to log in first!\n");
                    }
                    break;
                }
                case 7:{
                    /*the seventh command is to find a user, for which the user has to login first and if they haven't the progtram shows an error and exits*/
                    if(login){
                        name=commandReader();
                        if(atoi(name)){
                            printf("username must contain chars!\n");
                            break;
                        }
                        /*if the user enters their own ID, the program shows their posts*/
                        if(!strcmp(name,savedName)){
                            postSearcher(dummyMessage,savedName,savedPassword);
                            break;
                        }
                        /*the function below searches for the user and shows their posts, likes for each one, and post ID*/
                        user* temp=searchUser(dummyUser->next,name,password,1);
                        postSearcher(dummyMessage,temp->userName,temp->password);
                    }
                    else{
                        printf("you have to log in first!\n");
                    }
                    break;
                }
                case 8:{
                    /*the eighth command is to delete a post for which the user has to login first and if they haven't, an error message shows and the program exits the command*/
                    if(login){
                        int ID_ErrorCheck=atoi(commandReader());
                        /*if the postID entered wasn't an integer, the code below shows an error message and exits the command*/
                        if(ID_ErrorCheck){
                            postID= ID_ErrorCheck;
                        }
                        else{
                            printf("wrong postID entered! exiting the command!\n");
                            break;
                        }
                        /*the function below searches for the post and if it finds the post successfully deletes it*/
                        functionCheck=deletePost(dummyMessage->next,postID,savedName);
                        /*the function returns 1 if the post was deleted successfully and the program shows a message*/
                        if(functionCheck==1){
                            printf("deleted the post successfully!\n");
                        }
                        /*if the postID either doessn't exist or doesn't belong to the user an error message shows and the program exits the command*/
                        else if(functionCheck==0){
                            printf("couldn't find the post.\n");
                        }
                    }
                    else{
                        printf("you have to login first!\n");
                    }
                        break;
                }
                case 9:{
                    /*the ninth command is to close the app, so the value causing the program to loop becomes zero, and the accounts and the messages are saved in a file*/
                    flag=0;
                    printUsers(dummyUser);
                    printPosts(dummyUser,dummyMessage);
                    break;
                }
            }
        }
        
    }
    return 0;
}