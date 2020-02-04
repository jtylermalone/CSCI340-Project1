#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

////////////////////////// HERE /////////////////////////////////

struct Process {
	
	char pid[10];
	char ppid[10];
	char name[40];
	char vsize[16];
	int visited;
	char *parentProcess;
	int numChildren;
	int childProcess[];
};

void buildTree(struct Process processes[], int numProcesses) {


	for (int i = 0; i < numProcesses; i++){
		//printf("%s\n", processes[i].pid);
		int childProcs = 0;
		for (int j = 0; j < numProcesses; j++) {
			//printf("pid: %s   ppid: %s\n", processes[i].pid, processes[j].ppid);
			if (strcmp(processes[i].pid, processes[j].ppid) == 0) {
				//printf("pid %s    child pid: %s\n", processes[i].pid, processes[j].pid);
				int childPID = atoi(processes[j].pid);
				//printf("CHILDPID 1: %d\n", childPID);
				processes[i].childProcess[childProcs] = childPID;
				//printf("CHILDPID 2: %d\n\n", processes[i].childProcess[childProcs]);
				printf("childProcs: %d\n", childProcs);
				//childProcs++;
			}
		}
		
		processes[i].numChildren = childProcs;
		//printf("pid: %s  number of children: %d\n", processes[i].pid, processes[i].numChildren);
	}
	/*
	for (int i = 0; i < numProcesses; i++){
		printf("(%s) %s , %s kb\n", processes[i].pid, processes[i].name, processes[i].vsize);
		int childProcs = 0;
		printf("pid:      %s\n", processes[i].pid);
		for (int j = 0; j < numProcesses; j++) {
			if (atoi(processes[i].pid) == atoi(processes[j].ppid)){
				printf("pid: %d   ppid: %d\n", atoi(*processes[i].pid), atoi(processes[j].ppid));
				int childPID = *processes[j].pid;
				processes[i].childProcess[childProcs] = processes[j].pid;
				
				childProcs++;
			}
			
		}
		processes[i].numChildren = childProcs;
		//printf("$$$$$$$$$$$$$$$$ childProcs: %d\n", processes[i].numChildren);
		
	}
		// This print statement is for debugging purposes only
		//printf("pid: %s ppid: %s name: %s vsize: %s\n\n", processes[i].pid, processes[i].ppid, processes[i].name,
		   //processes[i].vsize);
	
	*/
}

/*
void preorderTraversal(struct Process rootProc, struct Process processes[], int numProcesses) {
	
	if (rootProc.childProcess[0] == -1)
		return;

	printf("(%s) %s , %s kb\n", rootProc.pid, rootProc.name, rootProc.vsize);

	for (int i = 0; i < rootProc.numChildren; i++) {
		for (int j = 0; j < numProcesses; j++) {
			//printf("number of children: %d\n", rootProc.numChildren);
			if (rootProc.childProcess[i] == atoi(processes[j].pid)) {
				for (int k = 0; k < rootProc.numChildren; k++) {
					//printf("child %d: %d", i, rootProc.childProcess[i]);	
					
					printf("(%s) %s , %s kb\n", rootProc.pid, rootProc.name, rootProc.vsize);
					//printf(": (%s) %s , %s kb\n", processes[j].pid, processes[j].name, processes[j].vsize);
					//preorderTraversal(processes[j], processes, numProcesses);
					printf("*****************************\n");
					}
			}
		}
	}	

	
}
*/
struct Process findProcess(struct Process processes[], int processesSize, int pid) {

	for (int i = 0; i < processesSize; i++)
		if (atoi(processes[i].pid) == pid)
			return processes[i];
}



void preorderTraversal(struct Process rootProc, struct Process processes[], int processesSize) {

	int size = rootProc.numChildren;
	if (size <= 0)
		return;

	for (int i = 0; i < size; i++) {
		int procNum = rootProc.childProcess[i];
		
		for (int j = 0; j < processesSize; j++) {

			//if (processes[j].pid == procNum)
			
		}
	}

}


//////////////////////////////////////////////////////////////  /////////
void printChildren(struct Process processes[], int processesSize) {

	for (int i = 0; i < processesSize; i++) {
		int size = processes[i].numChildren;
		printf("parent pid: %s\n", processes[i].pid);
		for (int j = 0; j < processesSize; j++) {
			if ((strcmp(processes[i].pid, processes[j].ppid) == 0)){
				printf("     pid: %s parent pid: %s\n", processes[j].pid, processes[j].ppid);

			}
		}

	}
	/*
	int size = parentProcess.numChildren;
	if (size == 0)
		return;

	//printf("size: %d\n", size);
	if (size != 0)
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < processesSize; j++) {
				if (parentProcess.childProcess[i] == atoi(processes[j].pid)){
					printf("childprocess: %d   pid: %d\n     ", parentProcess.childProcess[i], atoi(processes[j].pid));	
					//printf("********************************\n\n");
					printChildren(processes[j], processes, processesSize);
				}
			}
		}
	//for (int i = 0; i < size; i++) {
		//printf("      children: %d\n", parentProcess.childProcess[i]);
		//for (int j = 0; j < processesSize; j++){
			
			//printf("     child pid: %d\n", parentProcess.childProcess[i]);
		//}
	
*/
}

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
		
		// This if statement is ugly and long, but it works (at least I think). 
		//It makes sure we only interact with folders that have numeric names.
		if (dirName[0] == '0' || dirName[0] == '1' || dirName[0] == '2' || dirName[0] == '3' || dirName[0] == '4' || 
		  dirName[0] == '6' || dirName[0] == '7' || dirName[0] == '8' || dirName[0] == '9'){
			
			numberOfProcesses += 1;
		}
	}

	closedir(dr);
		
	// this list will hold all of the process structs that we instantiate.
	// I don't know if it's actually useful or not.
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
			// both parts of comm. I hope that makes some sort of sense.
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
		

			// adding the new process to the processes list
			processes[processCounter] = newProcess;

			// processCounter is what is being assigned to each
			// process struct's procNum field.
			processCounter += 1;

			
			// here we are resetting each of the variable's back
			// so that they're blank. C is weird so I think
			// this is necessary here...
			memset(fileName, 0, strlen(fileName));
			memset(dirName, 0, strlen(dirName));
			memset(comm, 0, strlen(comm));
			memset(ppid, 0, strlen(ppid));
			memset(vsize, 0, strlen(vsize));
			
		}
	}

	
	closedir(dr);
	
	// set adjacency matrix to all zeros
	
	// I think what's going on here is that edges are being added
	// to the graph (not sure if they're added correctly) and I am 
	// (I think!) correctly changing the relevant entries in the 	
	// adjacency matrix to 1's. 
	for (int i = 0; i < numberOfProcesses; i++){
		
		// This print statement is for debugging purposes only
		//printf("pid: %s ppid: %s name: %s vsize: %s\n\n", processes[i].pid, processes[i].ppid, processes[i].name,
		   //processes[i].vsize);
		
		
	}

	

	printf("number of processes: %d\n", numberOfProcesses);
	buildTree(processes, numberOfProcesses);
	//printf("number of children: %d\n", processes[0].numChildren);
	//preorderTraversal(processes[0], processes, numberOfProcesses);
	//for (int i = 0; i < processes[0].numChildren; i++)
		//printf("child %d: %d\n", i, processes[0].childProcess[i]);
	//printChildren(processes[0]);
	//struct Process test = findProcess(processes, numberOfProcesses, 1364);
	//printf("**** %s\n", test.pid);
	for (int i = 0; i < numberOfProcesses; i++){
		//printf("i: %d\n", i);
		printChildren(processes, numberOfProcesses);
	}
	printf("Done!\n");

	return 0;
	
}
