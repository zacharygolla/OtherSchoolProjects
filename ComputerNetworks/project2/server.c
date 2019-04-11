#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <string.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXADDR 16
#define MAXREAD 1024
#define MAXWRITE 1024
#define CONNECTIONS 5
#define MAXBUFF 1024

//log the command from the agent to the server
void logAction(char sck_msg[MAXREAD], char sck_addr[MAXADDR], FILE *file) {
    //character array to hold message
    char buffer[MAXBUFF];
    //set current time to current time
    time_t current_time = time(0);
    
    //put the time into a string and then print to file
    strftime(buffer, MAXBUFF, "%Y-%n-%d %H:%M:%S.000", localtime(&current_time));
    fprintf(file, "\"%s\": Responded to agent \"%s\" with \"%s\"\r\n", buffer, sck_msg, sck_addr);
}

void logReply(char sck_addr[MAXADDR], char reply[], FILE *file){
    //character array to hold message
    char buffer[MAXBUFF];
    //set time to now
    time_t current_time = time (0);
    
    //put time ito a string and send it to file
    strftime(buffer, MAXBUFF, "%Y-%m-%d %H:%M:%S.000", localtime(&current_time));
    if(reply[0] == 0){
        fprintf(file, "\"%s\": No response is supplied to agent \"%s\" (agent not active)\r\n", buffer, sck_addr);
    }else{
        fprintf(file, "\"%s\": Responded to agent \"%s\" with \"%s\"\r\n", buffer, sck_addr, reply);
    }
}

int main(int argc, char *argv[]) {
    int srvr_sckt, cli_sckt, cli_len, opt = 1, cli_num = 0;
    
    //address for the server socket and agent/client socket
    struct sockaddr_in serv_addr, cli_addr;
    
    time_t connect_times[CONNECTIONS] = {};
    char connect_addr[CONNECTIONS][MAXADDR] = {};
    
    //open file to send contents
    FILE *file, *read_file;
    file = fopen("log.txt", "w");
    int port = atoi(argv[1]);
    
    //run socket function (create server socket)
    srvr_sckt = socket(AF_INET, SOCK_STREAM, 0);
    
    //value is -1 return error else say socket created successfully
    if(srvr_sckt < 0) {
        perror("Socket not created\n");
        exit(-1);
    }
    else {
        fprintf(stdout, "Server socket created\n");
    }
    
    //need to reuse addr and avoid binding issue when canceling the run
    if(setsockopt(srvr_sckt, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Issue when reusing socket\n");
        close(srvr_sckt);
        exit(-1);
    }
    
    //create server address and set port to argument given
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    
    //bind the socket to the server address(if result is -1 return error) else successful bound
    if(bind(srvr_sckt, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error while binding\n");
        close(srvr_sckt);
        exit(-1);
    }
    else {
        fprintf(stdout, "Successfully Bound\n");
    }
    
    //listen for agents to send command to server
    fprintf(stdout, "Server Listening for Agents...\n");
    listen(srvr_sckt, 10);
    cli_len = sizeof(cli_addr);
    
    //create while loop with arguement run to keep server infinitely open
    while (1) {
        //accept a connection from an agent
        cli_sckt = accept(srvr_sckt, (struct sockaddr *) &cli_addr, &cli_len);
        
        //if value is -1, return error on accept
        if (cli_sckt < 0) {
            perror("ERROR on accept\n");
        }
        
        //character arrays for agent socket address and action given
        char sck_addr[MAXADDR];
        char sck_actn[MAXREAD];
        
        inet_ntop(AF_INET, &cli_addr.sin_addr, sck_addr, MAXREAD);
        read(cli_sckt, sck_actn, MAXREAD);
        
        //get the client number
        int i;
        bool found = false;
        for(i = 0; i < CONNECTIONS; ++i) {
            if(!strcmp(connect_addr[i], sck_addr)) {
                cli_num = i;
                found = true;
            }
        }
        
        if(!found) {
            cli_num = -1;
        }
        
        //log the action
        logAction(sck_actn, sck_addr, file);
        
        /*Join function
        if the cli number exists then the client already exists
        otherwise check if the server is full
        if not full, write to server and log the reply*/
        if(!strcmp(sck_actn, "#JOIN")) {
            if(cli_num < 0) {
                bool filled = true;
                int i;
                for(i = 0; i < CONNECTIONS; ++i) {
                    if(connect_addr[i][0] == 0) {
                        sprintf(connect_addr[i], "%s", sck_addr);
                        connect_times[i] = time(NULL);
                        filled = false;
                        break;
                    }
                }
                if(filled) {
                        write(cli_sckt, "$SERVER FULL", 12);
                        logReply(sck_addr, "$SERVER FULL", file);
                }
                else {
                        write(cli_sckt, "$OK", MAXWRITE);
                        logReply(sck_addr, "$OK", file);
                }
            }
            else {
                write(cli_sckt, "$ALREADY MEMBER", 15);
                logReply(sck_addr, "$ALREADY MEMBER", file);
            }
        }
        /*leave function
         if cli number does not exists, then the client is already a member
         else remove client from the server
         */
        else if(!strcmp(sck_actn, "#LEAVE")) {
            if(cli_num < 0) {
                write(cli_sckt, "$NOT MEMBER", 11);
            }
            else {
                memset(connect_addr[cli_num], 0, MAXADDR);
                write(cli_sckt, "$OK", 3);
            }
        }
        /*list function
         looked for all connected addresses, give their connection time and
         the ip address associated in the correct format asked for
         */
        else if(!strcmp(sck_actn, "#LIST")) {
            if(cli_num >= 0) {
                //log it
                logReply(sck_addr, "list shown", file);
                
                char list[MAXWRITE] = {};
                
                int i;
                for(i = 0; i < CONNECTIONS; ++i) {
                    if(connect_addr[i][0] != 0) {
                        sprintf(list, "<%s, %ld> \n", connect_addr[i], time(NULL) - connect_times[i]);
                        write(cli_sckt, list, MAXWRITE);
                    }
                }
            }
            else {
                //log it
                logReply(sck_addr, "\0", file);
            }
        }
        /*log function
         log function outputs to the log file all logs done so far
         */
        else if(!strcmp(sck_actn, "#LOG")) {
            if(cli_num >= 0) {
                logReply(sck_addr, "log.txt written", file);
                fflush(file);
                
                read_file = fopen("log.txt", "r");
                
                char log[MAXWRITE];
                while(fgets(log, MAXWRITE - 1, read_file))
                    write(cli_sckt, log, strlen(log));
                
                fclose(read_file);
            }else{
                logReply(sck_addr, "\0", file);
            }
        }
        else {
            logReply(sck_addr, "\0", file);
        }
        fflush(file);
        close(cli_sckt);
    } /* end of while */
}

