#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

////////////////////////// HERE /////////////////////////////////

struct Process {
	
	
	// each process has a 	
	char pid[10];
	char ppid[10];
	char name[40];
	char vsize[16];
	int procNum;
};

// everything between here and the next long line of slashes is stuff
// I copied straight from the internet. I've never implemented a  before.
/////////////////////////////////////////////////////////////////////////////////////////

#define MAX 300

struct Vertex {
   bool visited;
   char label[10];
   char parent[10];
};

//stack variables

int stack[MAX]; 
int top = -1; 

//graph variables

//array of vertices
struct Vertex* lstVertices[MAX];

//adjacency matrix
int adjMatrix[MAX][MAX];

//vertex count
int vertexCount = 0;

//stack functions

void push(int item) { 
   stack[++top] = item; 
} 

int pop() { 
   return stack[top--]; 
} 

int peek() {
   return stack[top];
}

bool isStackEmpty() {
   return top == -1;
}

//graph functions

//add vertex to the vertex list
void addVertex(char *label, char *parent) {
   struct Vertex* vertex = (struct Vertex*) malloc(sizeof(struct Vertex));
   strcpy(vertex->label, label);
   strcpy(vertex->parent, parent);
   //vertex->label = label;  
   vertex->visited = false;     
   lstVertices[vertexCount++] = vertex;
}

//add edge to edge array
void addEdge(int start,int end) {
   adjMatrix[start][end] = 1;
   //adjMatrix[end][start] = 1;
}

//display the vertex
void displayVertex(int vertexIndex) {
   printf("%s %s\n",lstVertices[vertexIndex]->label, lstVertices[vertexIndex]->parent);
   
}       

//get the adjacent unvisited vertex
int getAdjUnvisitedVertex(int vertexIndex) {
   int i;

   for(i = 0; i < vertexCount; i++) {
      if(adjMatrix[vertexIndex][i] == 1 && lstVertices[i]->visited == false) {
         return i;
      }
   }

   return -1;
}

void depthFirstSearch() {
   printf("in depthFirstSearch()\n");
   int i;

   //mark first node as visited
   lstVertices[0]->visited = true;

   //display the vertex
   displayVertex(0);   

   //push vertex index in stack
   push(0);

   while(!isStackEmpty()) {
      //get the unvisited vertex of vertex which is at top of the stack
      int unvisitedVertex = getAdjUnvisitedVertex(peek());

      //no adjacent vertex found
      if(unvisitedVertex == -1) {
         pop();
      } else {
         lstVertices[unvisitedVertex]->visited = true;
         displayVertex(unvisitedVertex);
         push(unvisitedVertex);
      }
   }

   //stack is empty, search is complete, reset the visited flag        
   for(i = 0;i < vertexCount;i++) {
      lstVertices[i]->visited = false;
   }   
   printf("\ndepthFirst complete\n");     
}

// everything above here and below the above line of slashes I copied
// straight from the internet. I've never implemented a graph before.
///////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	// all of this business about
	struct dirent *de; // Pointer for directory entry
	int numberOfProcesses = 0;
	// opendir() returns a pointer of DIR type.
	DIR *dr = opendir("/proc/");
	FILE *fp;
	
	if (dr == NULL)
	{
		printf("Could not open current directory" );
		return 0;
	}
	
	// in this while loop, we are only concerned with getting the current
	// number of running processes.
	while ((de = readdir(dr)) != NULL)
	{
		// de is the variable that holds each directory in /proc/ in turn,
		// and de->d_name is used to obtain the directory name.
		char* dirName = de->d_name;
		
		// This if statement is ugly and long, but it works. It makes sure we 
		// only interact with folders that have numeric names.
		if (dirName[0] == '0' || dirName[0] == '1' || dirName[0] == '2' || dirName[0] == '3' || dirName[0] == '4' || 
		  dirName[0] == '6' || dirName[0] == '7' || dirName[0] == '8' || dirName[0] == '9'){
			
			numberOfProcesses += 1;
		}
	}

	closedir(dr);
	struct Process processes[numberOfProcesses * sizeof(struct Process)];
	int processCounter = 0;
	dr = opendir("/proc/");

	// this while loop is where we actually loop through each running process.
	while ((de = readdir(dr)) != NULL) {
		
		// definining all the variables we need below. comm1 and comm2 are used
		// when the process name has a space in it (is two words long). 
		char* pid[10], ppid[5], vsize[20], comm[50], dummy[15], comm1[15], comm2[15];
		char buff[255]; // saw this online, no idea if it's necessary or not
		
		// de is the variable that holds each directory in /proc/ in turn,
		// and de->d_name is used to obtain the directory name.
		char* dirName = de->d_name;

		if (dirName[0] == '0' || dirName[0] == '1' || dirName[0] == '2' || dirName[0] == '3' || dirName[0] == '4' || 
		  dirName[0] == '6' || dirName[0] == '7' || dirName[0] == '8' || dirName[0] == '9'){
			
			
			// assuming that no stat file will have a full path
			// longer than 20 characters
			char fileName[20];

			// this adds "/proc/<pid>/stat" to the fileName variable
			snprintf(fileName, 20, "/proc/%s/stat", dirName);
			
			// this opens filename for reading
			FILE *fp = fopen(fileName, "r");

			// resetting filename
			memset(fileName, 0, strlen(fileName));


			// I accept that this line below is ugly, but I don't know a way around it.
			// It scans the stat file, which has several values separated by spaces,
			// and scans each value into a variable as needed. Since vsize is the (I
			// think) 23rd variable, it's necessary to scan this many variables. To
			// scan in the values we aren't concerned with, we are using a dummy
			// variable.
			fscanf(fp, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", dummy, comm, dummy, 
			  ppid, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, 
			  dummy, dummy, dummy, dummy, dummy, vsize);

			// this if statement executes if the process name is two words (i.e. contains
			// a space). Since each comm ends with a ), if there is a space in the comm
			// name then comm (at this point) won't end with ). Then, we have to combine 
			// both parts of comm.
			if (comm[strlen(comm)-1] != ')'){
				

				// I'm not 100% sure if this is necessary, but here we reset all relevant
				// stat variables.
				memset(comm, 0, strlen(comm));
				memset(dummy, 0, strlen(dummy));
				
				// this resets the fp pointer back to the beginning of the stat file so
				// that we can scan from scratch
				fseek(fp, 0, SEEK_SET);

				// need to scan for relevant values again
				fscanf(fp, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", dummy, comm1, comm2, dummy, ppid, dummy, dummy, dummy, dummy, dummy, dummy, 
				   dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, vsize);

				// combining both comm1 and comm2 into comm
				strcat(comm, comm1);
				strcat(comm, " ");
				strcat(comm, comm2);
	
			}
			
			// below, we are instantiating a Process and copying the relevant
			// strings into its fields.
			struct Process newProcess;
			strcpy(newProcess.pid, dirName);
			strcpy(newProcess.ppid, ppid);
			strcpy(newProcess.name, comm);
			strcpy(newProcess.vsize, vsize);
			newProcess.procNum = processCounter;
			addVertex(newProcess.pid, newProcess.ppid);

			// adding the new process to the processes list
			processes[processCounter] = newProcess;

			processCounter += 1;

			
			//printf("PID: %s\nCOMM: %s\nPPID: %s\nVSIZE: %s\n\n\n", dirName, comm, ppid, vsize);
			memset(fileName, 0, strlen(fileName));
			memset(dirName, 0, strlen(dirName));
			memset(comm, 0, strlen(comm));
			memset(ppid, 0, strlen(ppid));
			memset(vsize, 0, strlen(vsize));
			
		}
	}

	
	closedir(dr);
	
	// set adjacency matrix to all zeros
	for(int i = 0; i < MAX; i++) 
      		for(int j = 0; j < MAX; j++) 
         		adjMatrix[i][j] = 0;

	for (int i = 0; i < numberOfProcesses; i++){
		//printf("PID: %s\n", processes[i].pid);
		for (int j = 0; j < numberOfProcesses; j++){
			//printf("    PPID: %s\n procnum: %d\n", processes[j].ppid, processes[j].procNum);
			if (strcmp(processes[i].pid, processes[j].ppid) == 0){
				addEdge(processes[i].procNum, processes[j].procNum);
				adjMatrix[processes[i].procNum][processes[j].procNum] = 1;
				//printf("true ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
			}
		
		}
		printf("pid: %s ppid: %s name: %s vsize: %s\n\n", processes[i].pid, processes[i].ppid, processes[i].name,
		   processes[i].vsize);
		
		
	}

	

	printf("number of processes: %d\n", numberOfProcesses);
	depthFirstSearch();
	printf("Done!\n");
	return 0;
	
}
