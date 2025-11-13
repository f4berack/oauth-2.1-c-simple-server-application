#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <microhttpd.h>

#define PORT 8080

int handler(void *cls, struct MHD_Connection *conn,
            const char *url, const char *method,
            const char *ver, const char *upload_data,
            size_t *upload_data_size, void **con_cls)
{
    char *msg = malloc(strlen("test")+1);
    strcpy(msg, "test");

    if (strcmp(url, "/authorize") == 0) {

        const char *client_id = MHD_lookup_connection_value(conn, MHD_GET_ARGUMENT_KIND, "client_id");
        const char *redirect_uri = MHD_lookup_connection_value(conn, MHD_GET_ARGUMENT_KIND, "redirect_uri");
        const char *response_type = MHD_lookup_connection_value(conn, MHD_GET_ARGUMENT_KIND, "response_type");
        const char *state = MHD_lookup_connection_value(conn, MHD_GET_ARGUMENT_KIND, "state");

        printf("client_id=%s, redirect_uri=%s, response_type=%s, state=%s\n",
               client_id ? client_id : "(null)",
               redirect_uri ? redirect_uri : "(null)",
               response_type ? response_type : "(null)",
               state ? state : "(null)");

        free(msg);
        msg = strdup("authorize");
        if (!msg) {
            fprintf(stderr, "Memory allocation failed\n");
            return MHD_NO;
        }
    }

    struct MHD_Response *resp = MHD_create_response_from_buffer(strlen(msg),
                                                                (void*)msg,
                                                                MHD_RESPMEM_MUST_FREE);
    
    int response = MHD_queue_response(conn, MHD_HTTP_OK, resp);
    
    MHD_destroy_response(resp);

    return response;

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
