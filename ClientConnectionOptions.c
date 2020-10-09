#include "ClientConnectionOptions.h"

bool  isverboseOptionEntered(int verbose)
{
	if (verbose)
		return true;
	return false;
}
void  printConnectedIfVerboseEntered(struct pubsub_opts* opts)
{
	if (isverboseOptionEntered(opts->verbose))
	{
		printf("Connected\n");
	}

}
void SetWillTopicOption(struct pubsub_opts* opts, MQTTClient_connectOptions *conn_opts, MQTTClient_willOptions *will_opts)
{
	if (opts->will_topic)
	{
		will_opts->message = opts->will_payload;
		will_opts->topicName = opts->will_topic;
		will_opts->qos = opts->will_qos;
		will_opts->retained = opts->will_retain;
		conn_opts->will = &will_opts;
	}
}
void SetConnectionOption(MQTTClient_connectOptions *conn_opts, struct pubsub_opts* opts)
{
	conn_opts->keepAliveInterval = opts->keepalive;
	conn_opts->username = opts->username;
	conn_opts->password = opts->password;
	conn_opts->MQTTVersion = opts->MQTTVersion;
}
void SetSSLOption(struct pubsub_opts* opts, MQTTClient_connectOptions *conn_opts, MQTTClient_SSLOptions *ssl_opts)
{
	if (opts->insecure)
		ssl_opts->verify = 0;
	else
		ssl_opts->verify = 1;
	ssl_opts->CApath = opts->capath;
	ssl_opts->keyStore = opts->cert;
	ssl_opts->trustStore = opts->cafile;
	ssl_opts->privateKey = opts->key;
	ssl_opts->privateKeyPassword = opts->keypass;
	ssl_opts->enabledCipherSuites = opts->ciphers;
	conn_opts->ssl = &ssl_opts;
}
bool isConnectionSuccess(int verbose, const int rc)
{
	if (verbose && rc == MQTTCLIENT_SUCCESS)
		return true;

	return false;
}
void printMessageOnSuccessfullConnection(int verbose, const int rc)
{
	if (isConnectionSuccess(verbose, rc))
		fprintf(stderr, "Connected\n");

}
bool isConnectionFailed(int quiet, const int rc)
{
	if (rc != MQTTCLIENT_SUCCESS && !quiet)
		return true;

	return false;
}
void printMessageOnFailedConnection(int quiet, const int rc)
{
	if (isConnectionFailed(quiet, rc))
		fprintf(stderr, "Connect failed return code: %s\n", MQTTClient_strerror(rc));
}
bool isConnectionIsSSL(char* connection)
{
	if (strncmp(connection, "ssl://", 6) == 0)
		return true;
	return false;
}
bool isConnectionIsWSS(char* connection)
{
	if (strncmp(connection, "wss://", 6) == 0)
		return true;
	return false;
}

bool isConnectionIsSSLorWSS(struct pubsub_opts *opts)
{
	if (opts->connection && (isConnectionIsSSL(opts->connection) || isConnectionIsWSS(opts->connection)))
		return true;
	return false;
}