int authorize(struct MHD_Connection *conn);
int build_error_response(struct MHD_Connection *mhd_connection, struct MHD_Response *mhd_response, int error_code, const char *error_description);