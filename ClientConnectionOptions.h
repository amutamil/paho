
#if !defined(CLIENTCONNECTIONOPTIONS_H)
#define CLIENTCONNECTIONOPTIONS__H
#include "MQTTClient.h"
#include "pubsub_opts.h"
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
bool isverboseOptionEntered(int verbose);
void printConnectedIfVerboseEntered(struct pubsub_opts* opts);
void SetWillTopicOption(struct pubsub_opts* opts, MQTTClient_connectOptions *conn_opts, MQTTClient_willOptions *will_opts);
void SetSSLOption(struct pubsub_opts* opts, MQTTClient_connectOptions *conn_opts, MQTTClient_SSLOptions *ssl_opts);
void SetConnectionOption(MQTTClient_connectOptions *conn_opts,struct pubsub_opts* opts);
bool isConnectionSuccess(int verbose, const int rc);
void printMessageOnSuccessfullConnection(int verbose, const int rc);
bool isConnectionFailed(int quiet, const int rc);
void printMessageOnFailedConnection(int quiet, const int rc);
bool isConnectionIsSSL(char* connection);
bool isConnectionIsWSS(char* connection);
bool isConnectionIsSSLorWSS(struct pubsub_opts *opts);
#endif