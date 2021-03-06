#include <cparse/object.h>
#include <cparse/role.h>
#include <cparse/json.h>
#include <errno.h>
#include <cparse/util.h>
#include <cparse/query.h>
#include <cparse/user.h>
#include <cparse/types.h>
#include "protocol.h"
#include "log.h"
#include <stdio.h>
#include "private.h"

extern cParseUser *cparse_object_new();

void (*cparse_role_set_public_acl)(cParseRole *role, cParseAccess access, bool value) = cparse_object_set_public_acl;

void (*cparse_role_set_user_acl)(cParseRole *role, cParseUser *user, cParseAccess access, bool value) = cparse_object_set_user_acl;

void (*cparse_role_set_role_acl)(cParseRole *role, cParseRole *other, cParseAccess access, bool value) = cparse_object_set_role_acl;

void (*cparse_role_free)(cParseRole *role) = cparse_object_free;

bool (*cparse_role_save)(cParseRole *role, cParseError **error) = cparse_object_save;

bool (*cparse_role_delete)(cParseRole *role, cParseError **error) = cparse_object_delete;

bool (*cparse_role_delete_in_background)(cParseRole *role, cParseObjectCallback callback, void *param) = cparse_object_delete_in_background;

bool (*cparse_role_save_in_background)(cParseRole *role, cParseObjectCallback callback, void *param) = cparse_object_save_in_background;


cParseRole *cparse_role_with_name(const char *name)
{
    cParseRole *obj = cparse_object_new();

    if (obj == NULL) {
        return NULL;
    }

    obj->className = strdup(CPARSE_CLASS_ROLE);

    obj->urlPath = strdup(CPARSE_ROLES_PATH);

    cparse_object_set_string(obj, CPARSE_KEY_NAME, name);

    return obj;
}

const char *cparse_role_name(cParseRole *role)
{
    return cparse_object_get_string(role, "name");
}

cParseQuery *cparse_role_query_users(cParseRole *role, cParseError **error)
{
    cParseQuery *query = NULL;

    cParseQueryBuilder *builder = NULL;

    if (role == NULL) {
        cparse_log_set_errno(error, EINVAL);
        return NULL;
    }

    query = cparse_user_query_new();

    if (query == NULL) {
        return NULL;
    }

    builder = cparse_query_build_new();

    if (builder == NULL) {
        return NULL;
    }

    cparse_query_build_related_to(builder, "users", role);

    cparse_query_build_where(query, builder);

    cparse_query_build_free(builder);

    if (!cparse_query_find_objects(query, error)) {
        cparse_query_free(query);
        return NULL;
    }

    return query;
}

cParseQuery *cparse_role_query_roles(cParseRole *role, cParseError **error)
{
    cParseQuery *query = NULL;

    cParseQueryBuilder *builder = NULL;

    if (role == NULL) {
        cparse_log_set_errno(error, EINVAL);
        return NULL;
    }

    query = cparse_query_for_object(role);

    builder = cparse_query_build_new();

    cparse_query_build_related_to(builder, "roles", role);

    cparse_query_build_where(query, builder);

    cparse_query_build_free(builder);

    if (!cparse_query_find_objects(query, error)) {
        cparse_query_free(query);
        return NULL;
    }

    return query;
}

static void __cparse_role_add_dependency(cParseRole *role, const char *key, cParseObject *ref)
{
    cParseJson *users = NULL;
    cParseJson *objects = NULL;
    cParseJson *pointer = NULL;

    if (!role || cparse_str_empty(key) || !ref) {
        cparse_log_errno(EINVAL);
        return;
    }

    /* only allow certain values for key/pointer type */
    if (strcmp(key, CPARSE_USERS_PATH) && strcmp(key, CPARSE_ROLES_PATH)) {
        cparse_log_error("Can only by of type users or roles");
        return;
    }

    users = cparse_object_get(role, key);

    if (users == NULL) {
        users = cparse_json_new();
        cparse_json_set_string(users, CPARSE_KEY_OP, CPARSE_KEY_ADD_RELATION);
        cparse_object_set(role, key, users);
    }

    objects = cparse_json_get(users, "objects");

    if (objects == NULL) {
        objects = cparse_json_new_array();
        cparse_json_set(users, "objects", objects);
    }

    if (!cparse_json_is_array(objects)) {
        cparse_log_error("objects is not an array");
        return;
    }

    pointer = cparse_pointer_from_object(ref);

    cparse_json_array_add(objects, pointer);
}


void cparse_role_add_user(cParseRole *role, cParseUser *user)
{
    if (!role || !user) {
        cparse_log_errno(EINVAL);
        return;
    }

    __cparse_role_add_dependency(role, CPARSE_USERS_PATH, user);
}

void cparse_role_add_role(cParseRole *role, cParseRole *other)
{
    if (!role || !other) {
        cparse_log_errno(EINVAL);
        return;
    }

    __cparse_role_add_dependency(role, CPARSE_ROLES_PATH, other);
}
