
#include "JFile.h"
#include "List.h"
#include <stdlib.h>
#include <time.h>
#include "PortableGettimeofday.h"
#include "Date.h"

int main(void)
{
	JFile *j = JFile_new();
	int i, max;
	long seed = time(NULL);
	List *order = List_new();
	time_t t1;
	double dt;
	int journalingOn;
	
	srand(seed);

	for(journalingOn = 0; journalingOn < 2; journalingOn ++)
	{
		max =  1024*256;
		
		JFile_setJournalingOn_(j, journalingOn);
		
		List_removeAll(order);
		for (i = 0; i < max; i ++)
		{
			List_append_(order, (void *)i);
		}
		//List_shuffle(order);
		List_reverse(order);
		
		JFile_setPath_(j, "test.jfile");
		JFile_remove(j);
		JFile_open(j);
		
		printf("\njournaling %s\n", journalingOn ? "on" : "off");
		
		// group random writes
		t1 = Date_SecondsFrom1970ToNow();
		JFile_begin(j);
		for (i = 0; i < max; i ++)
		{
			long pos = ((int)List_at_(order, i)) * sizeof(int);
			int value = pos;
					
			//printf("write %i : %i\n", pos, value);
			
			JFile_setPosition_(j, pos);
			JFile_writeInt_(j, value);
			
		}
		JFile_commitToLog(j);
		JFile_commitToFile(j);
		dt = (Date_SecondsFrom1970ToNow()-t1);
		printf("\n	%i group random transactional writes per second\n", (int)((float)max/dt));
		
		// read to verify writes
		t1 = Date_SecondsFrom1970ToNow();
		for (i = 0; i < max; i ++)
		{
			long pos = ((int)List_at_(order, i)) * sizeof(int);
			int value = pos;
			int readValue;
					
			JFile_setPosition_(j, pos);
			readValue = JFile_readInt(j);
			
			if (readValue != value)
			{
				printf("read  %i : %i got %i ", pos, value, readValue);
				printf("ERROR");
				printf("\n");
			}
		}	
		dt = (Date_SecondsFrom1970ToNow()-t1);
		
		max = 50;
		
		List_removeAll(order);
		for (i = 0; i < max; i ++)
		{
			List_append_(order, (void *)i);
		}
		//List_shuffle(order);
		List_reverse(order);
		
		// individual random writes
		t1 = Date_SecondsFrom1970ToNow();
		for (i = 0; i < max; i ++)
		{
			long pos = ((int)List_at_(order, i)) * sizeof(int);
			int value = pos;
					
			//printf("write %i : %i\n", pos, value);
			
			JFile_begin(j);
			JFile_setPosition_(j, pos);
			JFile_writeInt_(j, value);
			JFile_commitToLog(j);
		}
		JFile_commitToFile(j);
		dt = (Date_SecondsFrom1970ToNow()-t1);
		printf("\n	%i individual random transactional writes per second\n", (int)((float)max/dt));
		
		// read to verify individual random writes
		t1 = Date_SecondsFrom1970ToNow();
		for (i = 0; i < max; i ++)
		{
			long pos = ((int)List_at_(order, i)) * sizeof(int);
			int value = pos;
			int readValue;
					
			JFile_setPosition_(j, pos);
			readValue = JFile_readInt(j);
			
			if (readValue != value)
			{
				printf("read  %i : %i got %i ", pos, value, readValue);
				printf("ERROR");
				printf("\n");
			}
		}	
		dt = (Date_SecondsFrom1970ToNow()-t1);
		//printf("	%i random reads per second\n", (int)((float)max/dt));
		
	}
	
	printf("\nNote: read/write size is 4 bytes\n");
	
	JFile_remove(j);
	JFile_free(j);
	return 0;
}

