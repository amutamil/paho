#include "publisher_main.h"
#include "MQTTClient.h"
#include "subscribe_main.h"
#include<string.h>
#include<stdlib.h>
int main(int argc, char* argv[])
{
	int argcnew = argc - 1;

	char* mode = "";
	char* argvnew[20];
	argvnew[0] = argv[0];
	if (argc > 2)
	{
		mode = argv[1];
		for (int i = 2; i < argc; i++)
		{
			argvnew[i - 1] = argv[i];

		}
	}
	else
	{
		argcnew = 1;
	}


	if (strcmp(mode, "pub") == 0)
	{
		PUBLISHmain(argcnew, argvnew);
	}
	else if (strcmp(mode, "sub") == 0)
	{
		SUBSCRIBEmain(argcnew, argvnew);
	}
	else
	{
		PUBLISHmain(argcnew, argvnew);
	}



	return 0;
}
