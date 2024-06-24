#include<stdio.h>
#include<stdlib.h>

int main()
{
	int *ptr;
	int n,i;
	printf("enter number of elements:");
	scanf("%d",&n);
	printf("Enter number of elements :%d\n",n);
	//DYnamically allocation memory using malloc()
	ptr=(int*)malloc(n* sizeof(int));
	//checkif the memory has been successfully 
	
	//allocated by malloc or not
	if(ptr==NULL){
		printf("Memory not allocated.\n");
		exit(0);
	}
	else {
		//Memory has beem successfully allocated 
		printf("Memory successfully allocated malloc.\n");
//Get the elements of the array
		for(i=0;i<n;++i)
		{
			ptr[i]=i+1;
		}
//printf the elements of the array
		printf("the elements of the array are:");
		for(i=0;i<n;++i)
		{
			 printf("%d, ", ptr[i]);
		}
	}
              return 0;
}


