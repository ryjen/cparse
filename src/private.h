#ifndef CPARSE_PRIVATE_H
#define CPARSE_PRIVATE_H

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

struct cparse_object
{
    /* rest of fields */
    cParseJson *attributes;
    cParseACL *acl;
    char *className;
    char *objectId;
    time_t updatedAt;
    time_t createdAt;
};


struct cparse_query
{
    cParseJson *where;
    cParseObject **results;
    char *className;
    char *keys;
    size_t size;
    int limit;
    int skip;
    bool trace;
    bool count;
};

struct cparse_acl
{
    cParseACL *next;
    char *name;
    bool read;
    bool write;
};

struct cparse_query_builder
{
    cParseJson *json;
};

struct cparse_op_array
{
    char *operation;
    cParseJson *objects;
};

struct cparse_op_decrement
{
    int amount;

};

struct cparse_op_increment
{
    int amount;
};


struct cparse_type_bytes
{
    void *data;
    size_t size;
};

struct cparse_type_date
{
    time_t value;
};

struct cparse_type_file
{
    char *localFileName;
    char *parseFileName;
    char *contentType;
    char *body;
    char *url;
};

struct cparse_type_geopoint
{
    double latitude;
    double longitude;
};

struct cparse_type_pointer
{
    char *className;
    char *objectId;
};

typedef bool (*cParseObjectAction)(cParseObject *obj, cParseError **error);

/* for background threads */
typedef struct
{
    cParseObject *obj;
    cParseObjectCallback callback; /* the callback passed by user */
    cParseObjectAction action; /* the method to call in thread */
    pthread_t thread;
    void (*cleanup)(cParseObject *);
} cParseObjectThread;

/* this runs a object action in the background */
pthread_t cparse_object_run_in_background(cParseObject *obj, cParseObjectAction action, cParseObjectCallback callback, void (*cleanup)(cParseObject *));

#define CPARSE_USER_CLASS_NAME "users"

bool cparse_class_name_is_user(const char *className);
bool cparse_class_name_is_object(const char *className);

#ifdef __cplusplus
}
#endif

#endif
