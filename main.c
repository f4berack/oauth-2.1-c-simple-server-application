#include <stdio.h>
#include <stdlib.h>
#include <microhttpd.h>

#define PORT 8080

int handler(void *cls, struct MHD_Connection *conn,
            const char *url, const char *method,
            const char *ver, const char *upload_data,
            size_t *upload_data_size, void **con_cls)
{
    const char *msg = "test";
    struct MHD_Response *resp = MHD_create_response_from_buffer(strlen(msg),
                                                                (void*)msg,
                                                                MHD_RESPMEM_MUST_FREE);
    return MHD_queue_response(conn, MHD_HTTP_OK, resp);
}

int main()
{
    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(
        MHD_USE_INTERNAL_POLLING_THREAD,
        PORT,
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

    printf("Simple OAuth 2.1 compliant server application running on port %d\n", PORT);
    getchar();

    MHD_stop_daemon(daemon);
    return EXIT_SUCCESS;
}
