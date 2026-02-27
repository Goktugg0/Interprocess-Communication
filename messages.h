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

#ifndef MESSAGES_H
#define MESSAGES_H

// define the data structures for your messages here

// Message from Client to Router-Dealer (Req_queue)
typedef struct {
	int job, data, service;
} ClientRequest;

// Message from Router-Dealer to Workers (S1_queue and S2_queue)
typedef struct {
	int job, data;
} WorkerRequest;

// Message from Workers to Router-Dealer (Res_queue)
typedef struct {
	int job, data;
} WorkerResponse;

#endif
