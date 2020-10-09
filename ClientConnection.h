#if !defined(CLIENTCONNECTION_H)
#define CLIENTCONNECTION__H
#pragma once
#include"MQTTClient.h"
#include"pubsub_opts.h"
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

int myconnect(MQTTClient* client, struct pubsub_opts* opts);
#endif
