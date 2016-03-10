// Name: Timothy Byers
// EE 367 Spring 2016 MP1
// This program simulates server systems 


#include <stdio.h>
#include <stdlib.h>

#define COSTBLOCKED 5  // Cost of getting blocked
#define COST3       1100 // Cost of Warrior 3
#define COST4       1300 // Cost of Warrior 4

typedef struct req{ // A request
   int arrival;     // Arrival time
   int service;     // Service time
   struct req *next;
   } Request;

typedef struct { // Processor state 
   int busy;    // Indicates if processor is busy (busy=1, idle=0)
   int depart;  // Time when processor completes service.
   } Processor;

typedef struct { //
   int occ;         // Occupancy of the request queue 
   int service1;     // Service time of the request (busy=1, idle=0)
   int service2;
   int service3;
   int service4;

   int slot1;	//Request queue positions
   int slot2;
   int slot3;
   int slot4;
} Reqqueue;

typedef struct{
   int occ;
   int slot[9];
   int service[9];
} war4queue;

typedef struct{
   int busy1;
   int busy2;
   int depart1;
   int depart2;
} war4process;

Request * getrequests(void);  // Inputs request data from file 
int warrior3(Request * list); // Returns the # blocked for Warrior 3 
int dblwarrior(Request * list); // Returns the # blocked for Double Warrior 3
int warrior4(Request * list); // Returns the # blocked for Warrior 4 

void main()
{
Request * reqqueue;
int blocked3;   // Blocking for Warrior 3
int blockeddbl; // Blocking for Double Warrior 3
int blocked4;   // Blocking for Warrior 4

reqqueue = getrequests();
if (reqqueue == NULL) return;

blocked3 = warrior3(reqqueue);
printf("Warrior 3:\n");
printf("   -> Daily number blocked = %d\n",blocked3);
printf("\n");
printf("   -> Annual number blocked = %d\n",blocked3 * 365);
printf("\n");


blockeddbl = dblwarrior(reqqueue);
printf("Double Warrior:\n");
printf("   -> Daily number blocked = %d\n",blockeddbl);
printf("\n");
printf("   -> Annual number blocked = %d\n",blockeddbl * 365);
printf("\n");


blocked4 = warrior4(reqqueue);
printf("Warrior 4:\n");
printf("   -> Daily number blocked = %d\n",blocked4);
printf("\n");
printf("   -> Annual number blocked = %d\n",blocked4 * 365);
printf("\n");


}

// Returns the number of blocked requests for Double Warrior 3 
// *** Implement this function *** 
int dblwarrior(Request * list)
{
	// Server 1 and server 2 processors
	Processor proc0, proc1;

	// Request list
	Request *reqHead;

	// Next request
	Request *nextReq;
	
	// Queue
	Reqqueue reqq0, reqq1;

	int req_num, ctime, blocked;
	req_num = blocked = 0;
	
	nextReq = list;
	
	// Initialize processors
	proc0.busy = proc1.busy = 0;
	// Initialize request queue
	reqq0.occ = reqq1.occ = 0;
	reqq0.slot1 = reqq0.slot2 = reqq0.slot3 = reqq0.slot4 = 0;
	reqq1.slot1 = reqq1.slot2 = reqq1.slot3 = reqq1.slot4 = 0;
	
	for(ctime = 0; nextReq != NULL; ctime++){
		if(reqq0.slot4 == 0)
			reqq0.occ = 0;
		if(reqq1.slot4 == 0)
			reqq1.occ = 0;

		//Start of time slot

		//if we have an arrival put in reqqueue
		if(nextReq->arrival == ctime){
			if(req_num % 2 == 0){
				if(reqq0.occ == 1)
					blocked++;
				else{
					if(reqq0.slot1 == 0){
						reqq0.slot1 = 1;
						reqq0.service1 = nextReq->service;
					}
					else if(reqq0.slot2 == 0){
						reqq0.slot2 = 1;
						reqq0.service2 = nextReq->service;
					}
					else if(reqq0.slot3 == 0){
						reqq0.slot3 = 1;
						reqq0.service3 = nextReq->service;
					}
					else if(reqq0.slot4 == 0){
						reqq0.slot4 = 1;
						reqq0.service4 = nextReq->service;
						reqq0.occ = 1;
					}
				}
			}
			else{
				if(reqq1.occ == 1)
					blocked++;
				else{
					if(reqq1.slot1 == 0){
						reqq1.slot1 = 1;
						reqq1.service1 = nextReq->service;
					}
					else if(reqq1.slot2 == 0){
						reqq1.slot2 = 1;
						reqq1.service2 = nextReq->service;
					}
					else if(reqq1.slot3 == 0){
						reqq1.slot3 = 1;
						reqq1.service3 = nextReq->service;
					}
					else if(reqq1.slot4 == 0){
						reqq1.slot4 = 1;
						reqq1.service4 = nextReq->service;
						reqq1.occ = 1;
					}
				}
			}
			//Point to next arrival
			nextReq = nextReq->next;
			req_num++;
		}

		//transfer request from queue to idle proc.
		if(proc0.busy == 0 && reqq0.slot1 > 0){
			proc0.depart = ctime + reqq0.service1 -1;
			proc0.busy = 1;
			reqq0.slot1 = reqq0.slot2;
			reqq0.service1 = reqq0.service2;
			reqq0.slot2 = reqq0.slot3;
			reqq0.service2 = reqq0.service3;
			reqq0.slot3 = reqq0.slot4;
			reqq0.service3 = reqq0.service4;
			reqq0.slot4 = 0;
		}
		if(proc1.busy == 0 && reqq1.slot1 > 0){
			proc1.depart = ctime + reqq1.service1 -1;
			proc1.busy = 1;
			reqq1.slot1 = reqq1.slot2;
			reqq1.service1 = reqq1.service2;
			reqq1.slot2 = reqq1.slot3;
			reqq1.service2 = reqq1.service3;
			reqq1.slot3 = reqq1.slot4;
			reqq1.service3 = reqq1.service4;
			reqq1.slot4 = 0;
		}

		//End of time slot

		//REmove requests from proc that completed
		//service
		if(proc0.busy == 1 && proc0.depart == ctime)
			proc0.busy = 0;
		if(proc1.busy == 1 && proc1.depart == ctime)
			proc1.busy = 0;

		//Transfer request from queue to idle proc
		if(proc0.busy == 0 && reqq0.slot1 > 0){
			proc0.depart = ctime + reqq0.service1;
			proc0.busy = 1;
			reqq0.slot1 = reqq0.slot2;
			reqq0.service1 = reqq0.service2;
			reqq0.slot2 = reqq0.slot3;
			reqq0.service2 = reqq0.service3;
			reqq0.slot3 = reqq0.slot4;
			reqq0.service3 = reqq0.service4;
			reqq0.slot4 = 0;
		}
		if(proc1.busy == 0 && reqq1.slot1 > 0){
			proc1.depart = ctime + reqq1.service1;
			proc1.busy = 1;
			reqq1.slot1 = reqq1.slot2;
			reqq1.service1 = reqq1.service2;
			reqq1.slot2 = reqq1.slot3;
			reqq1.service2 = reqq1.service3;
			reqq1.slot3 = reqq1.slot4;
			reqq1.service3 = reqq1.service4;
			reqq1.slot4 = 0;
		}
	}
	return blocked;


} 

// Returns the number of blocked requests for Warrior 4 
// *** Implement this function ***
int warrior4(Request * list)
{

   war4process proc; // Processor
   Request * reqhead;  // Request list
   Request * nextreq;  // Next arriving request.
   war4queue  reqq;
   int i;
   int ctime;
   int blocked = 0; // Number of requests blocked

   nextreq = list;
   proc.busy1 = proc.busy2 = 0;   // Initialize processor
   reqq.occ = 0;  // Initialize the request queue. 
   for (i = 0; i <= 8; i++) {
      reqq.slot[i] = 0;
   }

   for (ctime = 0; nextreq != NULL; ctime++) {
      if (reqq.slot[7] == 0) reqq.occ = 0;
   
      // Beginning of the time slot
      // If there is an arrival then put into request queue 
      if (nextreq->arrival == ctime) { // Arrival
         if (reqq.occ == 1) blocked++;
         else {
            for (i = 0; i < 8; i++) {
               if (reqq.slot[i] == 0) {
                  reqq.slot[i] = 1;
                  reqq.service[i] = nextreq->service;
                  break;
               }
            }
         }
         if (reqq.slot[7] == 1) reqq.occ = 1;
         nextreq = nextreq->next; // Point to the next arrival
      }
      // Transfer a request from request queue to idle processor 
      if (proc.busy1 == 0 && reqq.slot[0] > 0) {
         proc.depart1 = ctime + reqq.service[0] - 1;
         proc.busy1 = 1;
         for (i = 0; i < 8; i++) {
            reqq.slot[i] = reqq.slot[i+1];
            reqq.service[i] = reqq.service[i+1];
         }
      }
      if (proc.busy2 == 0 && reqq.slot[0] > 0) {
         proc.depart2 = ctime + reqq.service[0] - 1;
         proc.busy2 = 1;
         for (i = 0; i < 8; i++) {
            reqq.slot[i] = reqq.slot[i+1];
            reqq.service[i] = reqq.service[i+1];
         }
      }
   
      // End of the time slot 
      // Remove requests from the processor that have completed service 
      if (proc.busy1 == 1 && proc.depart1 == ctime) proc.busy1 = 0;
      if (proc.busy2 == 1 && proc.depart2 == ctime) proc.busy2 = 0;
   
      // Transfer a request from request queue to idle processor 
      if (proc.busy1 == 0 && reqq.slot[0] > 0) {
         proc.depart1 = ctime + reqq.service[0];
         proc.busy1 = 1;
         for (i = 0; i < 8; i++) {
            reqq.slot[i] = reqq.slot[i+1];
            reqq.service[i] = reqq.service[i+1];
         }
      }
      if (proc.busy2 == 0 && reqq.slot[0] > 0) {
         proc.depart2 = ctime + reqq.service[0];
         proc.busy2 = 1;
         for (i = 0; i < 8; i++) {
            reqq.slot[i] = reqq.slot[i+1];
            reqq.service[i] = reqq.service[i+1];
         }
      } 
   } 
   return blocked; 
}
// Returns the number of blocked request for Warrior 3
// Currently it assumes a request queue size of 1.  
// *** Rewrite it so it has a queue size of 4. *** 
int warrior3(Request * list)
{
Processor proc; // Processor
Request * reqhead;  // Request list
Request * nextreq;  // Next arriving request.
Reqqueue  reqq;

int ctime;
int blocked=0; // Number of requests blocked

nextreq=list;
proc.busy=0;   // Initialize processor
reqq.occ = 0;  // Initialize the request queue. 
reqq.slot1 = reqq.slot2 = reqq.slot3 = reqq.slot4 = 0;

for (ctime=0; nextreq!= NULL; ctime++) {

   if(reqq.slot4 == 0)
	reqq.occ = 0;

   // Beginning of the time slot
   // If there is an arrival then put into request queue 
   if (nextreq->arrival == ctime) { // Arrival
      if (reqq.occ == 1) blocked++;
      else{
      	if(reqq.slot1 == 0){
		reqq.slot1 = 1;
		reqq.service1 = nextreq->service;
	}
      	else if(reqq.slot2 == 0){
		reqq.slot2 = 1;
		reqq.service2 = nextreq->service;
	}
      	else if(reqq.slot3 == 0){
		reqq.slot3 = 1;
		reqq.service3 = nextreq->service;
	}
      	else if(reqq.slot4 == 0){
		reqq.slot4 = 1;
		reqq.service4 = nextreq->service;
		reqq.occ = 1;
	}
      }
      nextreq = nextreq->next; // Point to the next arrival 
   }
   // Transfer a request from request queue to idle processor 
   if (proc.busy==0 && reqq.slot1>0) {
      proc.depart = ctime + reqq.service1 - 1;
      proc.busy=1;
      reqq.slot1 = reqq.slot2;
      reqq.service1 = reqq.service2;
      reqq.slot2 = reqq.slot3;
      reqq.service2 = reqq.service3;
      reqq.slot3 = reqq.slot4;
      reqq.service3 = reqq.service4;
      reqq.slot4 = 0;
   } 

   // End of the time slot 
   // Remove requests from the processor that have completed service 
   if (proc.busy==1 && proc.depart==ctime) proc.busy=0;

   // Transfer a request from request queue to idle processor 
   if (proc.busy==0 && reqq.slot1>0) {
      proc.depart = ctime + reqq.service1;
      proc.busy=1;
      reqq.slot1 = reqq.slot2;
      reqq.service1 = reqq.service2;
      reqq.slot2 = reqq.slot3;
      reqq.service2 = reqq.service3;
      reqq.slot3 = reqq.slot4;
      reqq.service3 = reqq.service4;
      reqq.slot4 = 0;
   } 

} 
return blocked;
}


// Queries user for a data file that has a list of requests.
// The data is read and put into a linked list.
// The link to the link list is returned.  If the return value
// is NULL then there are no requests or the file couldn't be open.

Request * getrequests(void)
{
FILE *fp;
char filename[200];
int i;
int num;       // Number of requests
int arrival;   // Arrival time of request
int service;   // Service time of request 
Request *head;    // Head of the queue
Request *tail;    // Tail of queue
Request *new;

printf("Enter file with arriving customer data: ");
scanf("%s",filename);
fp = fopen(filename,"r");
if (fp==NULL) {
   printf("File '%s' not found or can't be opened\n",filename);
   return(NULL); 
   }
fscanf(fp,"%d",&num);
printf("Number of arriving customers = %d\n",num);

// Fill queue with customer data
head=tail=NULL;  // Initialize queue
for (i=0; i<num; i++) {
   // Load a new customer and create an item for it
   fscanf(fp,"%d %d",&arrival,&service);
   new = (Request*) malloc(sizeof(Request));
   new->arrival = arrival;
   new->service = service;
   new->next = NULL;
   // Insert into queue
   if (head==NULL) { // Insert into empty queue
      head=new;
      tail=new;
      }
   else {
      tail->next = new; // Insert at the tail
      tail=new;         
      }
//   printf("Customer %i: %d %d\n",i,tail->arrival,tail->service);
//   Above is used to debug -- check if everything was loaded properly
   }

fclose(fp);

if (head==NULL) printf("No requests in the file\n");
return(head);
}


