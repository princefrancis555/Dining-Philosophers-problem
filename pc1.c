#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<error.h>
void main()
{
    void up(int id)
    {
            struct sembuf sb[1]={0,1,0};
            if(semop(id,&sb[0],1)==-1)
            printf("Error");
    }
    void down(int id)
    {
            struct sembuf sb[1]={0,-1,0};
            if(semop(id,&sb[0],1)==-1)
            printf("Error");
    }
    
        int mutex,val,val1,val2,val3,full,empty,pid,i,n;
	struct sembuf s[]= { 0,-1,0};
        key_t key;
	printf("enter the size of the buffer");
	scanf("%d",&n);
	
	key = ftok("k.c", 'a');
	mutex = semget(key,1, IPC_CREAT | 0666);
	printf(" mutex created with id %d\n",mutex);
	semctl(mutex, 0 , SETVAL ,1);
        printf("mutex initialized as 1\n");

	key = ftok("k.c", 'b');
	full=semget(key,1, IPC_CREAT | 0666);
	printf(" full created with id %d\n",full);
	semctl(full, 0 , SETVAL ,0);
        printf("full initialized as 0\n");

	key = ftok("k.c", 'c');
	empty=semget(key,1, IPC_CREAT | 0666);
	printf(" empty created with id %d\n",empty);
	semctl(empty, 0 , SETVAL ,n);
        printf("empty initialized as n\n");
	fflush(stdout);
   
    pid=fork();
    if (pid>0)
    {
        printf("Producer running!");
        for(i=0;i<n;i++)
        {
            down(empty);
            down(mutex);
            printf("Producer producing item %d \n",i);
            up(mutex);
            up(full);
            sleep(2);
            }
        }
    else if (pid==0)
    {
        printf("Consumer running\n");
        for(i=0;i<n;i++)
            {
            down(full);
            down(mutex);
            printf("consumer consuming item %d \n",i);
            up(mutex);
            up(empty);
            sleep(2);
            }
    }
   
}


