#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "./oauth_validation.h"

bool is_valid_response_type(const char *rt){

    if(rt == NULL || (strcmp(rt, "code") != 0 && strcmp(rt, "token") != 0)){
        return false;
    }

    return true;

}
