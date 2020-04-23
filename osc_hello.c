#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lo/lo.h"

/*
int main(void)
{
	printf("OSC Basic TX TCP\n");

	lo_address t = lo_address_new("192.168.1.70", "7770");
	//lo_address t = lo_address_new_with_proto(LO_TCP,"192.168.1.70", "3032");

	if (lo_send(t, "/eos/key/go 0", "") == -1)
	{
		printf("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
    }

    return 0;
}
*/



void error(int num, const char *m, const char *path);
int generic_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data);


/*
int main(void)
{
	printf("OSC Basic TX TCP\n");

	lo_server localServer = lo_server_new_with_proto("7770", LO_TCP, error);
	lo_address destAddr = lo_address_new_with_proto(LO_TCP,"192.168.1.70", "3032");
	lo_server_add_method(localServer, NULL, NULL, generic_handler, NULL);

	getchar();


    lo_send_from(destAddr, localServer, LO_TT_IMMEDIATE, "/eos/key/go 0", "");

    getchar();

    lo_server_free(localServer);

    return 0;
}
*/


int main(void)
{
	printf("OSC Basic TX TCP\n");

	lo_server_thread localServerTh = lo_server_thread_new_with_proto("7770", LO_TCP, error);
	lo_address destAddr = lo_address_new_with_proto(LO_TCP,"192.168.0.99", "3032");
	lo_server_thread_add_method(localServerTh, NULL, NULL, generic_handler, NULL);
	lo_server_thread_start(localServerTh);
	lo_server localServer = lo_server_thread_get_server(localServerTh);

	getchar();

	printf("Send Go\n");
    lo_send_from(destAddr, localServer, LO_TT_IMMEDIATE, "/eos/key/go 0", "");

    getchar();

    lo_server_thread_free(localServerTh);

    return 0;
}



int generic_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data)
{
    int i;

    printf("path: <%s>\n", path);
    for (i = 0; i < argc; i++)
    {
        printf("arg %d '%c' ", i, types[i]);
        lo_arg_pp((lo_type)types[i], argv[i]);
        printf("\n");
    }
    printf("\n");
    fflush(stdout);

    return 1;
}


void error(int num, const char *msg, const char *path)
{
    printf("liblo server error %d in path %s: %s\n", num, path, msg);
    fflush(stdout);
}
