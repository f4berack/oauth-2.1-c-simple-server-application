#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <microhttpd.h>

#include "./oauth_validation.h"
#include "./oauth_logic.h"

typedef uint8_t OAuthErrorCode;

#define OAUTH_ERROR_INVALID_REQUEST          0
#define OAUTH_ERROR_UNAUTHORIZED_CLIENT      1
#define OAUTH_ERROR_ACCESS_DENIED            2
#define OAUTH_ERROR_UNSUPPORTED_RESPONSE_TYPE 3
#define OAUTH_ERROR_INVALID_SCOPE            4
#define OAUTH_ERROR_SERVER_ERROR             5
#define OAUTH_ERROR_TEMPORARILY_UNAVAILABLE  6
#define OAUTH_ERROR_UNKNOWN                  7

typedef struct {
    const OAuthErrorCode *error_code; 
    const char *error_description;
    const char *error_uri;
    const char *state;
} OAuthErrorResponse;

const char *error_code_to_string(OAuthErrorCode code) {
    switch (code) {
        case 0: return "invalid_request";
        case 1: return "unauthorized_client";
        case 2: return "access_denied";
        case 3: return "unsupported_response_type";
        case 4: return "invalid_scope";
        case 5: return "server_error";
        case 6: return "temporarily_unavailable";
        case 7: return "unknown";
        default: return "unknown";
    }
}

int authorize(
    struct MHD_Connection *conn
){

    struct MHD_Response *resp;
    char *msg;
    int response;

    const char *response_type = MHD_lookup_connection_value(conn, MHD_GET_ARGUMENT_KIND, "response_type");
    const char *client_id = MHD_lookup_connection_value(conn, MHD_GET_ARGUMENT_KIND, "client_id");
    const char *redirect_uri = MHD_lookup_connection_value(conn, MHD_GET_ARGUMENT_KIND, "redirect_uri");
    const char *state = MHD_lookup_connection_value(conn, MHD_GET_ARGUMENT_KIND, "state");

    printf("Response type: %s \n", response_type);
    printf("redirect uri: %s \n", redirect_uri);

    if(!is_valid_response_type(response_type)){
        return build_error_response(conn, resp, OAUTH_ERROR_INVALID_REQUEST, "Response type is not valid");
    }

    if(!is_valid_redirect_uri(redirect_uri)){
        return build_error_response(conn, resp, OAUTH_ERROR_INVALID_REQUEST, "Redirect URI is not valid");
    }

    msg = malloc(strlen("test")+1);
    strcpy(msg, "test");

    resp = MHD_create_response_from_buffer(strlen(msg),
                                                        (void*)msg,
                                                        MHD_RESPMEM_MUST_FREE);

    response = MHD_queue_response(conn, MHD_HTTP_BAD_REQUEST, resp);
    
    MHD_destroy_response(resp);

    return response;


}

int build_error_response(
    struct MHD_Connection *mhd_connection,
    struct MHD_Response *mhd_response,
    int error_code, 
    const char *error_description
){

    char *msg;
    OAuthErrorResponse error_response = {error_code_to_string(error_code), error_description, "error_uri", "state"};

    int size = snprintf(NULL, 0, "{\"error_code\":\"%s\",\"error_description\":\"%s\",\"error_uri\":%s,\"state\":%s}",
        error_response.error_code,
        error_description, 
        error_response.error_uri,
        error_response.state);

    msg = (char *)malloc(size + 1);
    
    snprintf(msg, size+1, "{\"error_code\":\"%s\",\"error_description\":\"%s\",\"error_uri\":%s,\"state\":%s}",
        error_response.error_code,
        error_description, 
        error_response.error_uri,
        error_response.state);

    mhd_response = MHD_create_response_from_buffer(strlen(msg) ,(void*) msg, MHD_RESPMEM_MUST_FREE);

    MHD_add_response_header(mhd_response, "Content-Type", "application/json");

    int response = MHD_queue_response(mhd_connection, MHD_HTTP_BAD_REQUEST, mhd_response);

    MHD_destroy_response(mhd_response);

    return response;

}