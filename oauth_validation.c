#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "./oauth_validation.h"

#include <uriparser/Uri.h>


bool is_valid_response_type(const char *rt){

    if(rt == NULL || (strcmp(rt, "code") != 0 && strcmp(rt, "token") != 0)){
        return false;
    }

    return true;

}

bool is_valid_redirect_uri(const char *ru){

    UriUriA uri;
    const char * errorPos;

    if(uriParseSingleUriA(&uri, ru, &errorPos) != URI_SUCCESS){
        return false;
    }

    // Redirect URI must have a schema
    if(uri.scheme.first == NULL){
        uriFreeUriMembersA(&uri);
        return false;
    }


    // Redirect URI must not contain a fragment (no # allowed)
    if(uri.fragment.first != NULL){
        uriFreeUriMembersA(&uri);
        return false;
    }

    const char *scheme = uri.scheme.first;

    if (strncmp(scheme, "https", 5) != 0) {
        uriFreeUriMembersA(&uri);
        return false;
    }

    uriFreeUriMembersA(&uri);
    return true;

}
