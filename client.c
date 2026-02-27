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
    // TODO:
    // (see message_queue_test() in interprocess_basic.c)
    //  * open the message queue (whose name is provided in the
    //    arguments)
    mqd_t req_queue = mq_open(argv[1], O_WRONLY);
    
    //  * repeatingly:
    //      - get the next job request 
    //      - send the request to the Req message queue
    //    until there are no more requests to send


    //  * close the message queue
    // close the message queue
    if (mq_close(req_queue) == -1) {
        perror("Client: mq_close failed");
        exit(1);
    }
    return (0);
}
