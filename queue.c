// Name:  <put your name here>
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
   int service;     // Service time of the request (busy=1, idle=0)
} Reqqueue;

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

blockeddbl = dblwarrior(reqqueue);
printf("Double Warrior:\n");
printf("   -> Daily number blocked = %d\n",blockeddbl);
printf("\n");

blocked4 = warrior4(reqqueue);
printf("Warrior 4:\n");
printf("   -> Daily number blocked = %d\n",blocked4);
printf("\n");
}

// Returns the number of blocked requests for Double Warrior 3 
// *** Implement this function *** 
int dblwarrior(Request * list)
{
return 0;
} 

// Returns the number of blocked requests for Warrior 4 
// *** Implement this function ***
int warrior4(Request * list)
{
return 0;
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

for (ctime=0; nextreq!= NULL; ctime++) {

   // Beginning of the time slot
   // If there is an arrival then put into request queue 
   if (nextreq->arrival == ctime) { // Arrival
      if (reqq.occ == 1) blocked++;
      else {
         reqq.occ++;
         reqq.service = nextreq->service;
      }
      nextreq = nextreq->next; // Point to the next arrival 
   }
   // Transfer a request from request queue to idle processor 
   if (proc.busy==0 && reqq.occ>0) {
      proc.depart = ctime + reqq.service - 1;
      proc.busy=1;
      reqq.occ--;
   } 

   // End of the time slot 
   // Remove requests from the processor that have completed service 
   if (proc.busy==1 && proc.depart==ctime) proc.busy=0;

   // Transfer a request from request queue to idle processor 
   if (proc.busy == 0 && reqq.occ>0) {
      proc.depart = ctime + reqq.service;
      proc.busy=1;
      reqq.occ--;
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


