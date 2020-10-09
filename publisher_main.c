

#include "MQTTClient.h"
#include "MQTTClientPersistence.h"
#include "pubsub_opts.h"
#include "ClientConnection.h"
#include"TraceCallBacks.h"

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include"subscribe_main.h"

#if defined(_WIN32)
#define sleep Sleep
#else
#include <sys/time.h>
#endif

volatile int toStoppub = 0;


void cfinishpub(int sig)
{
	signal(SIGINT, NULL);
	toStoppub = 1;
}


struct pubsub_opts optspub =
{
	1, 0, 1, 0, "\n", 100,  	/* debug/app options */
	NULL, NULL, 1, 0, 0, /* message options */
	MQTTVERSION_DEFAULT, NULL, "paho-cs-pub", 0, 0, NULL, NULL, "localhost", "1883", NULL, 10, /* MQTT options */
	NULL, NULL, 0, 0, /* will options */
	0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, /* TLS options */
	0, {NULL, NULL}, /* MQTT V5 options */
};




int messageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* m)
{
	/* not expecting any messages */
	return 1;
}





int PUBLISHmain(int argc, char** argv)
{
	MQTTClient client;
	MQTTClient_createOptions createOpts = MQTTClient_createOptions_initializer;
	char* buffer = NULL;
	int rc = 0;
	char* url;
	const char* version = NULL;
#if !defined(_WIN32)
	struct sigaction sa;
#endif
	const char* program_name = "paho_cs_pub";
	MQTTClient_nameValue* infos = MQTTClient_getVersionInfo();

	if (argc < 2)
		usage(&optspub, (pubsub_opts_nameValue*)infos, program_name);

	if (getopts(argc, argv, &optspub) != 0)
		usage(&optspub, (pubsub_opts_nameValue*)infos, program_name);

	if (optspub.connection)
		url = optspub.connection;
	else
	{
		url = malloc(100);
		sprintf(url, "%s:%s", optspub.host, optspub.port);
	}
	if (optspub.verbose)
		printf("URL is %s\n", url);

	if (optspub.tracelevel > 0)
	{
		MQTTClient_setTraceCallback(trace_callback);
		MQTTClient_setTraceLevel(optspub.tracelevel);
	}

	rc = MQTTClient_createWithOptions(&client, url, optspub.clientid, MQTTCLIENT_PERSISTENCE_NONE,
		NULL, &createOpts);
	if (rc != MQTTCLIENT_SUCCESS)
	{
		if (!optspub.quiet)
			fprintf(stderr, "Failed to create client, return code: %s\n", MQTTClient_strerror(rc));
		exit(EXIT_FAILURE);
	}

#if defined(_WIN32)
	signal(SIGINT, cfinishpub);
	signal(SIGTERM, cfinishpub);
#else
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = cfinish;
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
#endif

	rc = MQTTClient_setCallbacks(client, NULL, NULL, messageArrived, NULL);
	if (rc != MQTTCLIENT_SUCCESS)
	{
		if (!optspub.quiet)
			fprintf(stderr, "Failed to set callbacks, return code: %s\n", MQTTClient_strerror(rc));
		exit(EXIT_FAILURE);
	}

	if (myconnect(client,&optspub) != MQTTCLIENT_SUCCESS)
		goto exit;


	while (!toStoppub)
	{
		int data_len = 0;
		int delim_len = 0;

		if (optspub.stdin_lines)
		{
			buffer = malloc(optspub.maxdatalen);

			delim_len = (int)strlen(optspub.delimiter);
			do
			{
				int c = getchar();

				if (c < 0)
					goto exit;
				buffer[data_len++] = c;
				if (data_len > delim_len)
				{
					if (strncmp(optspub.delimiter, &buffer[data_len - delim_len], delim_len) == 0)
						break;
				}
			} while (data_len < optspub.maxdatalen);
		}
		else if (optspub.message)
		{
			buffer = optspub.message;
			data_len = (int)strlen(optspub.message);
		}
		else if (optspub.filename)
		{
			buffer = readfile(&data_len, &optspub);
			if (buffer == NULL)
				goto exit;
		}
		if (optspub.verbose)
			fprintf(stderr, "Publishing data of length %d\n", data_len);

			rc = MQTTClient_publish(client, optspub.topic, data_len, buffer, optspub.qos, optspub.retained, NULL);
		if (optspub.stdin_lines == 0)
			break;

		if (rc != 0)
		{
			myconnect(client,&optspub);
				rc = MQTTClient_publish(client, optspub.topic, data_len, buffer, optspub.qos, optspub.retained, NULL);
		}
		if (optspub.qos > 0)
			MQTTClient_yield();
	}

exit:
	if (optspub.filename || optspub.stdin_lines)
		free(buffer);
		rc = MQTTClient_disconnect(client, 0);

	MQTTClient_destroy(&client);

	return EXIT_SUCCESS;
}
