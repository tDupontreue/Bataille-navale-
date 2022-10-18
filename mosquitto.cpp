#include <iostream>
#include <mosquitto.h>

void main(int argc, char ** argv)
{
	mosquitto_lib_init();
	struct mosquitto * client = mosquitto_new(NULL, true, NULL);
	int test = mosquitto_connect(client, "192.168.64.220", 1883, 10);
	if (test == MOSQ_ERR_SUCCESS)
	{
		std::cout << "vous êtes connecter";
	}
	else
	{
		std::cout << "vous n'êtes pas connecter";
	}
	mosquitto_lib_cleanup();
 }
