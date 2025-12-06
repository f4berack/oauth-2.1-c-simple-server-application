#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <microhttpd.h>

#include "./config.h"
#include "./oauth_logic.h"

#include <uriparser/Uri.h>

enum MHD_Result handler(void *cls, struct MHD_Connection *conn,
            const char *url, const char *method,
            const char *ver, const char *upload_data,
            size_t *upload_data_size, void **con_cls)
{

    if (strcmp(url, "/authorize") == 0 && strcmp(method, "GET") == 0) {
        return authorize(conn);
    } else {
        return MHD_NO;
    }

}

int main()
{
    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(
        MHD_USE_INTERNAL_POLLING_THREAD,
        SERVER_PORT,
        NULL, 
        NULL,                  
        &handler, 
        NULL,              
        MHD_OPTION_END
    );

    if (daemon == NULL) {
        fputs("Error during creating daemon", stderr);
        return EXIT_FAILURE;
    }

    printf("Simple OAuth 2.1 compliant server application running on port %d\n", SERVER_PORT);
    getchar();

    MHD_stop_daemon(daemon);
    return EXIT_SUCCESS;
}
