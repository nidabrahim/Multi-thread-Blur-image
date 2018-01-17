#include "image.h"
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>


/*
*
* Youssef NIDABRAHIM - F2
*
**/

typedef struct param_p{
	
	int column;
	char **mArgs;
	struct image * mInput;
	struct image * mOutput;

} params;


void * startJob(void * args){
	
	params * arg = (params *) args;

	blur_image_column((arg->mInput), (arg->mOutput), strtoul(arg->mArgs[3], 0, 0), (int) arg->column);
}



int main(int arg_count, char **args)
{
	if(arg_count == 4)
	{
		pid_t wpid;
		int status = 0;
		struct image input = make_image_from_file(args[1]);
		struct image output = make_image(input.type, input.row_count, input.column_count, input.max_value);
		pthread_t thread[input.column_count];
		params threadParam[input.column_count];
		int i = 0;
		for(i = 0; i< input.column_count; i++){
			threadParam[i].column = i;
			threadParam[i].mArgs = args;
			threadParam[i].mInput = &input;
			threadParam[i].mOutput = &output;
		}
		
		
		for(i = 0; i< input.column_count; i++){
			pthread_create(&thread[i], NULL, &startJob, &threadParam[i]);
		}

		
		for(i = 0; i < input.column_count ; i++){
			pthread_join(thread[i],NULL);
		}
		
		write_image_to_file(&output,args[2]);


	
		
	}
	else
	{
		fprintf(stderr, "Essaie plutôt : %s input.ppm output.ppm 10\n", args[0]);
	}
}
