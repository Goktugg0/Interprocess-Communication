/* 
 * Operating Systems  (2INCO)  Practical Assignment
 * Interprocess Communication
 *
 * Goktug Cakmakli - 2108488
 * Arjin Calis - 2129213
 * Kateřina Chroncová - 2143194
 *
 * Grading:
 * Your work will be evaluated based on the following criteria:
 * - Satisfaction of all the specifications
 * - Correctness of the program
 * - Coding style
 * - Report quality
 * - Deadlock analysis
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>      // for perror()
#include <unistd.h>     // for getpid()
#include <mqueue.h>     // for mq-stuff
#include <time.h>       // for time()

#include "messages.h"
#include "request.h"

static void rsleep (int t);


int main (int argc, char * argv[])
{

    printf("Working(Client)...\n");

    // when you run a program (via the command line or via execlp) it can receive a number of arguments
    // the first argument is typically the name of the program
    // argc stores the number of arguments that the program received
    printf("Number of arguments: %d\n", argc);
    printf("First param (ServiceID): %s\n", argv[0]);
    printf("Second param(Data): %s\n", argv[1]);

    // TODO:
    // (see message_queue_test() in interprocess_basic.c)
    //  * open the message queue (whose name is provided in the
    //    arguments)

    // If someone tries to run client directly, it catches and exits without crashing
    if (argc != 2) {
        fprintf(stderr, "Client: Invalid arguments. Expected 1, got %d\n", argc - 1);
        exit(1);
    }

    // Create the channel to send
    // We use O_WRONLY because we want to send
    // We don't need the third and fourth parameter because these are only needed for channel creation
    mqd_t req_queue = mq_open(argv[1], O_WRONLY);
    if (req_queue == (mqd_t)-1) {
        perror("Client: mq_open failed");
        exit(1);
    }

    // Receive the request ID from the arguement that is provided in the request.
    char *req_name = argv[1];
    
    //  * repeatingly:
    //      - get the next job request 
    //      - send the request to the Req message queue
    //    until there are no more requests to send
    int jobID;
    int data;
    int serviceID;
    ClientRequest req_msg;

    // Keep checking whether there is a message in the request list
    while (getNextRequest(&jobID, &data, &serviceID) != NO_REQ) {
        // Check whether the request is without any errors
        if (getNextRequest(&jobID, &data, &serviceID) != NO_ERR) {
            perror("Receiving Request failed (client)!!");
            break;
        }

        // Assign the variables to the data structure
        req_msg.job = jobID;
        req_msg.data = data;
        req_msg.service = serviceID;

        // Send the message as shown in the interprocess basics
        int result = mq_send(req_queue, (char*)&req_msg, sizeof(ClientRequest), 0);
        if(result == -1)
        {
            perror("Sending message failed (client)!");
            break; // If the sending queue is broken already just break and don't try again.
        }
    }

    //  * close the message queue
    // When there is no resources or the message queue cannot be closed exit safely
    if (mq_close(req_queue) == -1) {
        perror("Client: mq_close failed");
        exit(1);
    }
    return (0);
}
