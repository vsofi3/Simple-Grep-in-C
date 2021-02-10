#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#define UNUSED __attribute__((unused))

static void lowering(char buf[]){
	int i;
	for(i = 0; buf[i]; i++){
		buf[i] = tolower(buf[i]);
	}
}


void sgrep(FILE *file, char *pattern, bool doI, bool doV, bool doC, bool multipleFiles,  char *fileName){
	char line[BUFSIZ];
	char lineCopy[BUFSIZ];
	bool lineMatches = false;
	int count = 0;

	while(fgets(line, sizeof line, file) != NULL){
		strcpy(lineCopy,line);
		if(doI){
			lowering(lineCopy);
			lowering(pattern);
		}
		//either true or false
		lineMatches = (strstr(lineCopy,pattern)!= NULL);
		if(doV){
		lineMatches = !lineMatches;
		}

		if(lineMatches){
			count++;
			if(!doC){
			
				if(multipleFiles){
					printf("%s:%s",fileName,line);
				}
				else{
					printf("%s",line);
			}
		}
		}
	}
	
		//print out the count
		
	if(doC){
		if(multipleFiles){
			printf("%s:%d\n",fileName,count);
		}
	else{
		printf("%d\n",count);
	}
	}
}



int main(UNUSED int argc, UNUSED char *argv[]){
	int opt;
	bool doI, doV, doC;
	char *pattern;
	char buf[BUFSIZ];
	FILE *file;
	int exitStatus = EXIT_FAILURE;
	bool multipleFiles = false;
	char *fileName = NULL;

	doI = doV = doC = false;

	opterr = 0;
	while((opt = getopt(argc,argv,"ivc")) != -1){
		switch(opt){
			case 'i': doI = true;break;
			case 'v': doV = true;break;
			case 'c': doC = true;break;
			default: fprintf(stderr,"%s: illegal option, '-%c'\n", argv[0], optopt);
		}
	}	

	int index;
	index = argc - optind;	
	//PUT ERROR MESSAGE HERE
	if(optind == argc){
		fprintf(stderr,"This is an error\n");
		exitStatus = EXIT_FAILURE;
		goto cleanup;
	}


	if(index == 1){
		int fileIndex = NULL;
		//0 files given, use standard input
		//pattern set to first non flag argument
		pattern = argv[optind];
		sgrep(stdin,pattern,doI,doV,doC,multipleFiles,fileName);
		exitStatus = EXIT_SUCCESS;
		goto cleanup;
	}

	if(index > 1){
		if(index > 2){
			multipleFiles = true;
		}
		//checking for one or more files
		int fileIndex;
		//make sure file actually opens
		//is it NULL?
		pattern = argv[optind];
		for(fileIndex=optind+1;fileIndex<argc;fileIndex++){
			file = fopen(argv[fileIndex],"r");
			fileName = argv[fileIndex];
			if(file == NULL){
				fprintf(stderr,"Error- invalid file");
				return EXIT_FAILURE;
			}
			/*
			if(index > 2){
			printf("%s:",argv[fileIndex]);
			}
			*/
			//print the file name
			sgrep(file,pattern,doI,doV,doC,multipleFiles,fileName);
			//error checking
			fclose(file);
		}
		exitStatus = EXIT_SUCCESS;
		goto cleanup;
	}
	cleanup:
	return exitStatus;
}
