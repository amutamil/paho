

#include"ClientConnection.h"
#include"ClientConnectionOptions.h"


int myconnect(MQTTClient* client, struct pubsub_opts* opts)
{
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	MQTTClient_SSLOptions ssl_opts = MQTTClient_SSLOptions_initializer;
	MQTTClient_willOptions will_opts = MQTTClient_willOptions_initializer;
	int rc = 0;

	printConnectedIfVerboseEntered(opts);

	

	SetConnectionOption(&conn_opts, opts);

	SetWillTopicOption(opts, &conn_opts, &will_opts);

	if(isConnectionIsSSLorWSS(opts))
	{
		SetSSLOption(opts, &conn_opts, &ssl_opts);
	}
	conn_opts.cleansession = 1;
	rc = MQTTClient_connect(client, &conn_opts);
	
		

	printMessageOnSuccessfullConnection(opts->verbose,rc);
	printMessageOnFailedConnection(opts->quiet, rc);
	

	return rc;
}