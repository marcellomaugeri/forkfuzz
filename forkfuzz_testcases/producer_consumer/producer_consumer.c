#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

#define MSG_SIZE 1024

// Function to reverse a string
void reverse(char *str, int length) {
    int start = 0;
    int end = length -1;
    while (start < end) {
        char temp = *(str+start);
        *(str+start) = *(str+end);
        *(str+end) = temp;
        start++;
        end--;
    }
}

struct msgbuf {
    long mtype;
    char mtext[MSG_SIZE];
};

int main(int argc, char *argv[]) {
    int msgid, pid;
    struct msgbuf msg;

    if (argc < 2) {
        printf("Usage: %s [file] \n", argv[0]);
        exit(1);
    }

    //Open the input file
    FILE *fp;
    fp = fopen(argv[1], "r");

    msgid = msgget(IPC_PRIVATE, 0600 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {  // child process is the consumer
        char current_msg[MSG_SIZE] = "";
        char reversed_msg[MSG_SIZE] = "";
        while (1) {
            //Receive the message
            if (msgrcv(msgid, &msg, MSG_SIZE, 1, 0) == -1) {
                perror("msgrcv");
                exit(1);
            }
            //Copy the message to current_msg
            strcpy(current_msg, msg.mtext);
            strcpy(reversed_msg, msg.mtext);
            //Reverse the message
            reverse(reversed_msg, strlen(reversed_msg));
            //print the reversed message
            printf("(%s,%s)\n", current_msg, reversed_msg);
            //abort the program if the message is a palindrome
            if(strcmp(current_msg, reversed_msg) == 0){
                raise(SIGSEGV);
            }
            //quit if the message is "quit"
            if (strcmp(current_msg, "quit") == 0) {
                printf("Consumer: quit\n");
                exit(0);
            }
        }
    } else {  // parent process is the producer
        int i;
        //Read the file line by line send the messages to the consumer
        char buf[MSG_SIZE];
        while (fgets(buf, MSG_SIZE, fp) != NULL) {
            char *token = strtok(buf, " ");  
            while (token != NULL) {
                msg.mtype = 1;
                strcpy(msg.mtext, token);
                if (msgsnd(msgid, &msg, strlen(msg.mtext) + 1, 0) == -1) {
                    perror("msgsnd");
                    exit(1);
                }
                token = strtok(NULL, " ");
            }
        }
        //Send the message "quit" to the consumer
        strcpy(msg.mtext, "quit");
        if (msgsnd(msgid, &msg, strlen(msg.mtext) + 1, 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
    }

    // wait for child process to exit
    if (waitpid(pid, NULL, 0) == -1) {
        perror("waitpid");
        exit(1);
    }
    printf("Producer: quit\n");
    // destroy message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    exit(0);
}
