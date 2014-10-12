#include <stdio.h>

int main(int argc, char ** argv)
{
	union 
	{
		short s;
		char c[sizeof(short)];
	} un;

	un.s = 0x0102;
	if(sizeof(short) == 2)
	{
		printf("un.c[0]: 0x%x, un.c[1]: 0x%x\n",un.c[0],un.c[1]);
		if(un.c[0] == 1 && un.c[1] == 2)
			printf("big_endian\n");
		else if(un.c[0] == 2 && un.c[1] == 1)
			printf("little-endian\n");
		else
			printf("unknow\n");
	}
	else
		printf("sizeof(short) = %d\n", (int)sizeof(short));
	return 0;	
}
