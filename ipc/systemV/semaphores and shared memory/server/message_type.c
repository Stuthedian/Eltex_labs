#include "message_type.h"

int increment_sem(int semaphore_id)
{
	return update_sem(semaphore_id, 1);
}

int decrement_sem(int semaphore_id)
{
	return update_sem(semaphore_id, -1);
}

static int update_sem(int semaphore_id, int value)
{
	struct sembuf modify_sem;
	modify_sem.sem_num = 0;
	modify_sem.sem_op = value;
	modify_sem.sem_flg = 0;
	
	return semop(semaphore_id, &modify_sem, 1);
}
