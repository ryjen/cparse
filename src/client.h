#ifndef CPARSE_CLIENT_H_
#define CPARSE_CLIENT_H_

#include <stdlib.h>
#include <cparse/defines.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! a parse response */
typedef struct cparse_client_response cParseResponse;

/*! a parse request */
typedef struct cparse_client_request cParseRequest;

/*! a list of request headers */
typedef struct cparse_kv_list cParseRequestHeader;

/*! a list of request data (for uri encoding) */
typedef struct cparse_kv_list cParseRequestData;

/*! HTTP Request Method Types */
typedef enum
{
    HttpRequestMethodGet,
    HttpRequestMethodPost,
    HttpRequestMethodPut,
    HttpRequestMethodDelete
} HttpRequestMethod;

/*! allocates a client request
 * \param method the HTTP method to use
 * \param path the endpoint to use
 * \returns the allocated request
 */
cParseRequest *cparse_client_request_with_method_and_path(HttpRequestMethod method, const char *path);

/*! issues a request and returns a json response
 * \param request the request instance
 * \param error a pointer to an error that will get allocated if not successfull.
 * \returns the json response
 */
cParseJson *cparse_client_request_get_json(cParseRequest *request, cParseError **error);

/*! adds a HTTP header to the request.
 * \param request the request instance
 * \param key the header key ex. 'Content-Type'
 * \param value the header value ex 'application/json'
 */
void cparse_client_request_add_header(cParseRequest *request, const char *key, const char *value);

/*! sets the request body. Anything provided with this method will be URI encoded.
 * \param request the request instance
 * \param payload the body text to set
 */
void cparse_client_request_set_payload(cParseRequest *request, const char *payload);

/*! adds data for the request body. Any values will be URI encoded.
 * \param request the request instance
 * \param key the key for the value
 * \param value the value to send
 */
void cparse_client_request_add_data(cParseRequest *request, const char *key, const char *value);

/*! deallocates a request
 * \param request the request instance
 */
void cparse_client_request_free(cParseRequest *request);

/*! performs a request
 * \param request the request instance
 * \param error a pointer to an error that will get allocated if not successful
 * \returns true if successfull otherwise false
 */
bool cparse_client_request_perform(cParseRequest *request, cParseError **error);

#ifdef __cplusplus
}
#endif

#endif
