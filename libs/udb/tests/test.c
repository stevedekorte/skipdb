
#include "UDB.h"
#include <time.h>
#include <stdlib.h>
#include "Date.h"

int main(void)
{
	List *pids = List_new();
	PID_TYPE pid;
	Datum d;
	int i, max;
	int v;
	time_t t1;
	double dt;
		
	UDB *u = UDB_new();
	
	printf("UDB (unordered database) performance test:\n");
	UDB_setPath_(u, "test.udb");
	UDB_delete(u);
	UDB_open(u);
	
	max = 30;

	t1 = Date_SecondsFrom1970ToNow();
	for (i = 0; i < max; i ++)
	{
		UDB_beginTransaction(u);
		d.size = sizeof(int);
		v = i;
		d.data = (void *)&v;
		
		pid = UDB_allocPid(u);
		UDB_at_put_(u, pid, d);
		//printf("at %i put %i\n", (int)pid, v); 
		UDB_commitTransaction(u);
	}
	dt = Date_SecondsFrom1970ToNow()-t1;;
	printf("	%i individual sequential transactional writes per second\n", (int)((double)max/dt));

	UDB_close(u);
	UDB_delete(u);
	UDB_open(u);

	max = 1024*64;
	
	t1 = Date_SecondsFrom1970ToNow();
	UDB_beginTransaction(u);
	for (i = 0; i < max; i ++)
	{
		d.size = sizeof(int);
		v = i;
		d.data = (void *)&v;
		
		pid = UDB_allocPid(u);
		UDB_at_put_(u, pid, d);
		List_append_(pids, (void *)pid);
		//printf("at %i put %i\n", (int)pid, v); 
	}
	UDB_commitTransaction(u);
	dt = Date_SecondsFrom1970ToNow()-t1;;
	printf("	%i group sequential transactional writes per second\n", (int)((double)max/dt));


	t1 = Date_SecondsFrom1970ToNow();
	UDB_beginTransaction(u);
	for (i = 0; i < max; i ++)
	{
		int v;
		pid = (PID_TYPE)List_at_(pids, i);
		d = UDB_at_(u, pid);

		if (d.data == NULL || d.size == 0)
		{
			printf("ERROR: no record at %i\n", pid);
		}
		else
		{
			if(d.size != 4)
			{
				printf("ERROR: wrong size %i at pid %i\n", d.size, pid);
			}
			else
			{
				memcpy(&v, d.data, sizeof(int));

				if(v != i)
				{
					printf("ERROR: wrong value %i at pid %i\n", v, pid);
				}
			}
		}
	}
	UDB_commitTransaction(u);
	dt = Date_SecondsFrom1970ToNow()-t1;;
	printf("	%i reads per second\n", (int)((float)max/dt));
	
	
	t1 = Date_SecondsFrom1970ToNow();
	UDB_beginTransaction(u);
	for (i = 0; i < max; i ++)
	{
		UDB_removeAt_(u, (size_t)List_at_(pids, i));
	}
	UDB_commitTransaction(u);
	dt = Date_SecondsFrom1970ToNow()-t1;;
	printf("	%i group transactional removes per second\n", (int)((float)max/dt));
	
	UDB_delete(u);
	UDB_free(u);
	List_free(pids);
	return 0;
}