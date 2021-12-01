/* 
 *Rowan Antonuccio 
 *A02184507
 *ECE 5720 Cachelab
 */
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "getopt.h"
#include "cachelab.h"

struct cache_line **cache;//double ptr to chache
struct cache_line
{
	int valid;
	unsigned long int tag;
	int LRU;
};

int main(int argc, char **argv)
{

	int v = 0;	//verbose
	int s = -1;	//bits
	int E = -1;	//associativity
	int b = -1;	//offset
	char *t = "-1";	//trace

	unsigned long int mask;

	int opt;
	while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1)//gather args
	{
		switch (opt)
		{
			case 'h':

				return 0;
				break;
			case 'v':
				v = 1;
				break;
			case 's':
				s = atoi(optarg);
				break;
			case 'E':
				E = atoi(optarg);
				break;
			case 'b':
				b = atoi(optarg);
				break;
			case 't':
				t = optarg;
				break;
			default:
				printf("ERROR\n");
				return 0;
				break;
		}
	}

	//generate tags
	mask = pow(2, s) - 1;

	//create cache
	
	cache = (struct cache_line **) malloc(sizeof(struct cache_line *) *(mask + 1));
	for (int i = 0; i < mask + 1; i++)
	{
		cache[i] = (struct cache_line *) malloc(sizeof(struct cache_line) *E);	//ONLY 2 USES OF MALLOC                      
	}

	//place values in cache
	for (int i = 0; i < mask + 1; i++)
	{
		for (int j = 0; j < E; j++)
		{
			cache[i][j].tag = 0;
			cache[i][j].valid = 0;
			cache[i][j].LRU = 0;
		}
	}

	//open file
	FILE * pFile;
	pFile = fopen(t, "r");
	char ID;
	unsigned long int addr;
	int size;

	int hit = 0, miss = 0, evict = 0, ts = 0;
	while (fscanf(pFile, " %c %lx,%d", &ID, &addr, &size) > 0)
	{

		if (ID == 'I')
		{
			continue;
		}

		unsigned long int x, tag;
		x = ((addr >> b) &mask);
		tag = (addr >> (b + s));

		char *verb;
		int found = 0, evic = 0, val = 0;

		//index through cache
		for (int i = 0; i < E; i++)
		{

			//check validity
			if (cache[x][i].valid == 1)
			{

				if (cache[x][i].tag == tag)
				{
				 	//break when found
					cache[x][i].LRU = ts++;
					found = 1;
					break;
				}
			}
			else
			{

				//for invalids
				val = 1;
				cache[x][i].valid = 1;
				cache[x][i].tag = tag;
				cache[x][i].LRU = ts++;
				break;
			}
		}

		//if no data found, find smallest bit and place in cache
		if (found == 0 && val == 0)
		{
			int j = 0;
			for (int i = 1; i < E; i++)
			{
				if (cache[x][j].LRU > cache[x][i].LRU)
				{
					j = i;
				}
			}
			evic = 1;
			cache[x][j].valid = 1;
			cache[x][j].tag = tag;
			cache[x][j].LRU = ts++;
		}

		switch (ID)
		{

			//load
			case 'L':
				if (found == 1)
				{
					verb = "hit";
					hit++;
				}
				else if (evic == 1)
				{
					verb = "miss evict";
					miss++;
					evict++;
				}
				else
				{
					verb = "miss";
					miss++;
				}
				break;

				//store
			case 'S':
				if (found == 1)
				{
					verb = "hit";
					hit++;
				}
				else if (evic == 1)
				{
					verb = "miss evict";
					miss++;
					evict++;
				}
				else
				{
					verb = "miss";
					miss++;
				}
				break;

				//data modify
			case 'M':
				if (found == 1)
				{
					verb = "hit hit";
					hit++;
					hit++;
				}
				else if (evic == 1)
				{
					verb = "miss evict hit";
					miss++;
					evict++;
					hit++;
				}
				else
				{
					verb = "miss hit";
					miss++;
					hit++;
				}
				break;
		}

		if (v)
		{
			printf("%c %lx,%d %s\n", ID, addr, size, verb);//verbose testing
		}
	}

	printSummary(hit, miss, evict);
	return 0;
}