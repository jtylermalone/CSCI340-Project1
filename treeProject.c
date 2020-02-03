#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

struct Process
{
	char pid[5];
	char ppid[10];
	char name[40];
	char vsize[16];
	int parentProcess;
	int childProcess[];
};

void buildTree(struct Process processes[]) {
	for(int i = 0; i < (sizeof(processes)/(sizeof(processes[0]))); i++) {
		int procid = processes[i].pid;
		int childProcs = 0;
		for(int j = 0; j < (sizeof(processes)/(sizeof(processes[0]))); j++) {
			if(procid == processes[j].ppid) {
				processes[i].childProcess[childProcs] = processes[j].pid;
				processes[j].parentProcess = processes[i].pid;
				childProcs++;
			}
		}
		childProcs = 0;
	}
}

void preorderTraversal(struct Process* rootProc) {
	//printf("%s", rootProc.pid);
}

int main(int argc, char *argv[])
{
	int numProcess = 0;
	DIR *dr = opendir("/proc");
	struct dirent *de;

	if (dr == NULL)
	{
		printf("Could not open current directory");
		return 0;
	}

	while ((de = (readdir(dr))) != NULL)
	{
		char *dirName = de->d_name;

		if (isdigit(*dirName) != 0)
		{
			numProcess++;
		}
		//struct Process* processes = malloc(sizeof(struct Process) * numProcess);
	}
	closedir(dr);
	// in this while loop, we are only concerned with getting the current
	// number of running processes.

	// this list will hold all of the process structs that we instantiate.
	// I don't know if it's actually useful or not.
	struct Process processes[numProcess * sizeof(struct Process)];
	dr = opendir("/proc/");

	// this while loop is where we actually loop through each running process.
	while ((de = (readdir(dr))) != NULL)
	{

		// definining all the variables we need below. comm1 and comm2 are used
		// when the process name has a space in it (is two words long).
		char *pid[5], ppid[5], vsize[20], comm[50], dummy[15], comm1[15], comm2[15];
		//char buff[255]; saw this online, no idea if it's necessary or not

		// de is the variable that holds each directory in /proc/ in turn,
		// and de->d_name is used to obtain the directory name.
		char *dirName = de->d_name;
		//printf("File name: %s\n", dirName);

		if (isdigit(*dirName) != 0)
		{

			// assuming that no stat file will have a full path
			// longer than 20 characters
			char fileName[20];

			// this adds "/proc/<pid>/stat" to the fileName variable
			snprintf(fileName, 20, "/proc/%s/stat", dirName);

			// this opens filename for reading
			FILE *fp = fopen(fileName, "r");
			//printf("File name: %s", fileName);
			// resetting filename
			memset(fileName, 0, strlen(fileName));

			// I accept that this line below is ugly, but I don't know a way around it.
			// It scans the stat file, which has several values separated by spaces,
			// and scans each value into a variable as needed. Since vsize is the (I
			// think) 23rd variable, it's necessary to scan this many variables. To
			// scan in the values we aren't concerned with, we are using a dummy
			// variable.
			fscanf(fp, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", *pid, comm, dummy,
				   ppid, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy, dummy,
				   dummy, dummy, dummy, dummy, dummy, vsize);

			// this if statement executes if the process name is two words (i.e. contains
			// a space). Since each comm ends with a ), if there is a space in the comm
			// name then comm (at this point) won't end with ). Then, we have to combine
			// both parts of comm. I hope that makes some sort of sense.
			if (comm[strlen(comm) - 1] != ')')
			{

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
			comm[0] = "";
			comm[strlen(comm) - 1] = ""; 
			printf("(%s) %s , %s kb\n", *pid, comm, vsize);
			
			strcpy(newProcess.pid, dirName);
			strcpy(newProcess.ppid, ppid);
			strcpy(newProcess.name, comm);
			strcpy(newProcess.vsize, vsize);
			//addVertex(newProcess.pid, newProcess.ppid);

			// adding the new process to the processes list
			processes[numProcess] = newProcess;

			// processCounter is what is being assigned to each
			// process struct's pid field.

			// here we are resetting each of the variable's back
			// so that they're blank. C is weird so I think
			// this is necessary here...
			memset(fileName, 0, strlen(fileName));
			memset(dirName, 0, strlen(dirName));
			memset(comm, 0, strlen(comm));
			memset(ppid, 0, strlen(ppid));
			memset(vsize, 0, strlen(vsize));
		}
		buildTree(processes);
	}
	printf("number of processes: %d\n", numProcess);
	printf("Done!\n");

	return 0;
}
