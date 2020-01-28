#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

struct Process {
	
	char* processID;
	char* parentID;
	char* processName;
	char* virtualSize;
};

int main(void)
{
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
	
	// Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
	// for readdir()
	
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
	while ((de = readdir(dr)) != NULL) {
		
		// definining all the variables we need below. comm1 and comm2 are used
		// when the process name has a space in it (is two words long). 
		char pid[10], ppid[5], vsize[12], comm[30], dummy[15], comm1[15], comm2[15];
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
			
			// this opens "/proc/<pid>/stat" for reading
			FILE *fp = fopen(fileName, "r");
			memset(fileName, 0, strlen(fileName));


			// I accept that this line below is ugly, but I don't know a way around it.
			// It scans the stat file, which has several values separated by spaces,
			// and scans each value into a variable as needed. Since vsize is the (I
			// think) 23rd variable, it's necessary to scan this many variables. To
			// scan in the values we aren't concerned with, we are using a dummy
			// variable.
			fscanf(fp, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s", pid, comm, dummy, 
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

				// only need 2nd and 3rd values here
				fscanf(fp, "%s %s %s", dummy, comm1, comm2);

				// combining both comm1 and comm2 into comm
				strcat(comm, comm1);
				strcat(comm, " ");
				strcat(comm, comm2);
	
			}
			
			struct Process newProcess;
			newProcess.processID = dirName;
			newProcess.parentID = ppid;
			newProcess.processName = comm;
			newProcess.virtualSize = vsize;

			

			processes[processCounter] = newProcess;

			processCounter += 1;

			printf("processCounter: %d\n", processCounter);
			/*
			printf("PID: %s\nCOMM: %s\nPPID: %s\nVSIZE: %s\n\n\n", dirName, comm, ppid, vsize);
			processes[0][processCounter] = {pid, ppid, comm, vsize];
			printf("******** pid: %s\n", processes[0][processCounter]);
			processes[1][processCounter] = ppid;
			processes[2][processCounter] = comm;
			processes[3][processCounter] = vsize;
			*/
			memset(fileName, 0, strlen(fileName));
			
		}
	}

	
	closedir(dr);
	for (int i = 0; i < numberOfProcesses; i++){
		printf("Process %d: pid: %s ppid: %s comm: %s vsize: %s\n", i, processes[i].processID, processes[i].parentID, processes[i].processName, processes[i].virtualSize);
		
	}
	printf("number of processes: %d\n", numberOfProcesses);
	printf("Done!\n");
	printf("%ld\n", sizeof(struct Process));
	return 0;
	
}
