#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include<semaphore.h>
#include <stdbool.h>
#include<pthread.h>

int row,column;
int A[10][10];  // The matrix we will use.

int columnsumarr[10];  // The array in which we will store the sum of each column.
int columnsumarr_index=0;  // It's index we will use to itereate through it.

// Define a semaphore
sem_t semaphore;


// The function that will be passed on each thread.
void *calculateColumn(void* args){
       
       sem_wait(&semaphore);
       
       int cn = *(int*)args;
       int columnsum =0;

	for (size_t i = 0; i < row; i++)
		   {
		   
			  columnsum+=A[i][cn];  
			    
		 }

	columnsumarr[columnsumarr_index]=columnsum;
	columnsumarr_index++;
	
	sem_post(&semaphore);
	free(args);
}

// Swap function that we will use in the sorting section.
void swap(int *a,int *b){
*a=*b+*a;
*b=*a-*b;
*a=*a-*b;

}

// The function that will be passed on the final thread.
void *bubbleSortAndPrint(){

	bool flag;
	for(int i=0;i<column-1;i++){
	    flag = true;
	    for(int j=0;j<column-i-1;j++){
	        if(columnsumarr[j]>columnsumarr[j+1]){
		    swap(&columnsumarr[j],&columnsumarr[j+1]);
		    flag=false;
		    }
	    }
        if(flag){
	        break;
	        }

	}
	
	for(int i=0;i<columnsumarr_index;i++){

             printf("%d ",columnsumarr[i]);
         }
         
         printf("\n");

}




int main(int argc,char* argv[]){


scanf("%d",&row);
scanf("%d",&column);

//Fill The Original Matrix.
for (size_t i = 0; i < row; i++)
	   {
	   
		    for (size_t j = 0; j < column; j++)
		    {
			 scanf("%d",&A[i][j]);
		
                     }
                     
           }          

pthread_t th[column]; // Array of threads that will operate on the matrix.

sem_init(&semaphore,0,1); //Initialize the semaphore here.Second argument is 0 because we use a single process.Since the semaphores initial value will be 1, the third parameter is 1.
int i;
// Create multiple threads and pass the column index as the parameter.
for(i=0;i<column;i++){
         int* a=malloc(sizeof(int));
         *a = i;
         if(pthread_create( th+i ,NULL,&calculateColumn,a) != 0){
                
                perror("Error While Creating Thread");
                return 2;
                }
       }
       
 // Join the threads.      
for(i=0;i<column;i++){
       
        if(pthread_join( th[i] , NULL) != 0){
            perror("Error While joining thread");
            return 4;
            }       
       }
            
pthread_t final_t; // Final thread which will be doing the sorting and printing.     


if(pthread_create(&final_t,NULL,&bubbleSortAndPrint,NULL) != 0){printf("Error While Creating Thread");}
  
if(pthread_join(final_t, NULL) != 0){printf("Error While joining 1");}
      
           return 0;
}
