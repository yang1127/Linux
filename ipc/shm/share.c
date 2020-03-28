//共享内存
//int shmget(key_t key, size_t size, int shmflg);

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#define IPC_KEY 0x12345678 //ftok:key_t ftok
#define SHM_SIZE 4096

int main()
{
	int shmid = shmget(IPC_KEY, SHM_SIZE, IPC_CREAT | 0664);
	if (shmid < 0)
	{
		perror("shmget error\n");
	} 
	
	//void *shmat(int shmid, const void *shmaddr, int shmflg);
	void *shm_start = shmat(shmid, NULL, 0); //任何方式
	if (shm_start == (void*) - 1)
	{
		perror("shmat error\n");
	}
	int i = 0;
	while(1)
	{	
		sprintf(shm_start, "%s-%d\n", "下雨天~~", i++); //sprintf:格式化字符串放到一个buf中
		sleep(1);
	}
	shmdt(shm_start);
	shmctl(shmid, IPC_RMID, NULL);	
	return 0;
}
