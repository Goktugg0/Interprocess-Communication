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
#include "service1.h"

static void rsleep (int t);

int main (int argc, char * argv[])
{
    // TODO:
    // (see message_queue_test() in interprocess_basic.c)
    //  * open the two message queues (whose names are provided in the
    //    arguments)
    //  * repeatedly:
    //      - read from the S1 message queue the new job to do
    //      - wait a random amount of time (e.g. rsleep(10000);)
    //      - do the job 
    //      - write the results to the Rsp message queue
    //    until there are no more tasks to do
    //  * close the message queues
    
    // --------------------------------------
    // --------------------------------------
    // --------------------------------------

    printf("Working(Worker service 1)...\n");

    // when you run a program (via the command line or via execlp) it can receive a number of arguments
    // the first argument is typically the name of the program
    // argc stores the number of arguments that the program received
    printf("Number of arguments: %d\n", argc);
    printf("First param (): %s\n", argv[0]);
    printf("Second param(): %s\n", argv[1]);
    printf("Third param(): %s\n", argv[1]);

    // command-line arguments are available in argv[0] .. argv[argc-1]
    // argv[0] always contains the name of the program
    // check if the user has started this program with valid arguments
    if (argc != 3)
    {
        fprintf (stderr, "%s: %d arguments:\n", argv[0], argc);
        for (int i = 1; i < argc; i++)
            {
            fprintf (stderr, " '%s'\n", argv[i]);
            }
        exit (1);
    }
    // else: parse the arguments...

    const char *s1_name = argv[1];
    const char *rsp_name = argv[2];
    mqd_t mq_s1;
    mqd_t mq_rsp;
    WorkerRequest req;
    WorkerResponse rsp;

    mq_s1 = mq_open(s1_name, O_RDONLY);
    if (mq_s1 == (mqd_t)-1) {
        perror("Worker 1: mq_open for sending failed");
        exit(1);
    }

    mq_rsp = mq_open(rsp_name, O_WRONLY);
    if (mq_rsp == (mqd_t)-1) {
        perror("Worker 1: mq_open for response failed");
        exit(1);
    }

    //  * repeatedly:
    //      - read from the S1 message queue the new job to do
    //      - wait a random amount of time (e.g. rsleep(10000);)
    //      - do the job 
    //      - write the results to the Rsp message queue
    while (req.job >= 0)
    {

        // Send the message
        int result_recieve = mq_send(mq_s1, (char*)&req, sizeof(req), 0);
        if(result_recieve == -1)
        {
            perror("Recieving message failed (worker 1)!");
            break; // If the sending queue is broken already just break and don't try again.
        }

        rsleep(10000);

        rsp.job  = req.job;
        rsp.data = service(req.data);

        // Send the message
        int result_send = mq_send(mq_rsp, (char*)&rsp, sizeof(rsp), 0);
        if(result_send == -1)
        {
            perror("Sending message failed (worker 1)!");
            break; // If the sending queue is broken already just break and don't try again.
        }
    }
    
    mq_close(mq_s1);
    mq_close(mq_rsp);

    return(0);
}

/*
 * rsleep(int t)
 *
 * The calling thread will be suspended for a random amount of time
 * between 0 and t microseconds
 * At the first call, the random generator is seeded with the current time
 */
static void rsleep (int t)
{
    static bool first_call = true;
    
    if (first_call == true)
    {
        srandom (time (NULL) % getpid ());
        first_call = false;
    }
    usleep (random() % t);
}
