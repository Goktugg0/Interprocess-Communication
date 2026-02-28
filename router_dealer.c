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
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>    
#include <unistd.h>    // for execlp
#include <mqueue.h>    // for mq


#include "settings.h"  
#include "messages.h"

char client2dealer_name[30];
char dealer2worker1_name[30];
char dealer2worker2_name[30];
char worker2dealer_name[30];

int main (int argc, char * argv[])
{
  if (argc != 1)
  {
    fprintf (stderr, "%s: invalid arguments\n", argv[0]);
  }
  
  // TODO:
    //  * create the message queues (see message_queue_test() in
    //    interprocess_basic.c)
    pid_t processID;
    mqd_t mq_req;
    mqd_t mq_s1;
    mqd_t mq_s2;
    mqd_t mq_rsp;

    struct mq_attr attr_c_req, attr_w_req, attr_w_rsp;


    snprintf(client2dealer_name, 30, "/Req_queue_Group101_%d", getpid());
    snprintf(dealer2worker1_name, 30, "/S1_queue_Group101_%d", getpid());
    snprintf(dealer2worker2_name, 30, "/S2_queue_GRoup101_%d", getpid());
    snprintf(worker2dealer_name, 30, "/Rsp_queue_Group101_%d", getpid());

   
    attr_c_req.mq_maxmsg = MQ_MAX_MESSAGES;
    attr_c_req.mq_msgsize = sizeof(ClientRequest);

    attr_w_req.mq_maxmsg = MQ_MAX_MESSAGES;
    attr_w_req.mq_msgsize = sizeof(WorkerRequest);

    attr_w_rsp.mq_maxmsg = MQ_MAX_MESSAGES;
    attr_w_rsp.mq_msgsize = sizeof(WorkerResponse);

    mq_req = mq_open(client2dealer_name, O_CREAT | O_RDONLY | O_EXCL | O_NONBLOCK, 0600, &attr_c_req);
    mq_rsp = mq_open(worker2dealer_name, O_CREAT | O_RDONLY | O_EXCL | O_NONBLOCK, 0600, &attr_w_rsp);
    mq_s1 = mq_open(dealer2worker1_name, O_CREAT | O_WRONLY | O_EXCL | O_NONBLOCK, 0600, &attr_w_req);
    mq_s2 = mq_open(dealer2worker2_name, O_CREAT | O_WRONLY | O_EXCL | O_NONBLOCK, 0600, &attr_w_req);
  
    if(mq_req == -1 || mq_s1 == -1 || mq_s2 == -1 || mq_rsp == -1)
    {
      perror("Message queue creation failed!");
      exit(1);
    }

    //response -1 ise error ile exit arasinda mq close response icin 

    //  * create the child processes (see process_test() and
    //    message_queue_test())
    pid_t client_pid = fork();
    if (client_pid == 0){
      execlp("./client", "client_child", client2dealer_name, NULL);
      perror("Failed to start client!");
      exit(1);
    }

    pid_t worker1_pid[N_SERV1];
    for (int i = 0; i < N_SERV1; i++) {
      worker1_pid[i] = fork();
      if (worker1_pid[i] == 0){
        execlp("./worker_s1", "worker_s1_child", worker2dealer_name, NULL);
        perror("Failed to start worker S1!");
        exit(1);
      }
    } 

    pid_t worker2_pid[N_SERV1];
    for (int i = 0; i < N_SERV2; i++) {
      worker2_pid[i] = fork();
      if (worker2_pid[i] == 0){
        execlp("./worker_s2", "worker_s2_child", worker2dealer_name, NULL);
        perror("Failed to start worker S2!");
        exit(1);
      }
    } 

    //  * read requests from the Req queue and transfer them to the workers
    //    with the Sx queues


    //  * read answers from workers in the Rep queue and print them
    //  * wait until the client has been stopped (see process_test())
    //  * clean up the message queues (see message_queue_test())

    // Important notice: make sure that the names of the message queues
    // contain your goup number (to ensure uniqueness during testing)
  
  return (0);
}
