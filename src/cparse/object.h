/*!
 * @file
 * @header cParse Object
 * Functions for dealing with objects on parse.com
 */
#ifndef CPARSE_OBJECT_H

/*! @parseOnly */
#define CPARSE_OBJECT_H

#include <time.h>
#include <cparse/defines.h>
#include <pthread.h>

/*! callback for iterating object key/values */
typedef void (*cParseObjectAttributeCallback)(cParseObject *obj, const char *key, cParseJson *value, void *arg);

BEGIN_DECL

/* initializers */

/*! makes a copy of a parse object
 * @param obj the object instance
 * @param other the object to copy from
 */
void cparse_object_copy(cParseObject *obj, cParseObject *other);

/*! allocates a parse object with a class name.
 * @param className the type of object to create
 * @return the allocated object
 */
cParseObject *cparse_object_with_class_name(const char *className);

/*! allocates a parse object with json data
 * @param className the type of object to create
 * @param data the data to copy from
 * @return the allocated object
 */
cParseObject *cparse_object_with_class_data(const char *className, cParseJson *data);

/*! deallocates a parse object
 * @param obj the object instance
 */
void cparse_object_free(cParseObject *obj);

/* getters/setters */

/*! gets the memory size of an object
 * @return the size in bytes
 */
size_t cparse_object_sizeof();

/*! gets the parse object id
 * @param obj the object instance
 * @return the id or NULL of not set
 */
const char *cparse_object_id(cParseObject *obj);

/*! gets the type of parse object
 * @param obj the object instance
 * @return the object type
 */
const char *cparse_object_class_name(cParseObject *obj);

/*! gets the time when the object was created
 * @param obj the object instance
 * @return the time the object was created or zero if the object has not been saved
 */
time_t cparse_object_created_at(cParseObject *obj);

/*! gets the time when the object was updated
 * @param obj the object instance
 * @return the time the object was updated or zero if the object has not been updated
 */
time_t cparse_object_updated_at(cParseObject *obj);

/*! gets the access control list for an object
 * @param obj the object instance
 * @return the access control list
 */
cParseJson *cparse_object_acl(cParseObject *obj);

/* client/rest methods */

/*! saves a parse object
 * @param obj the object instance
 * @param error a pointer to an error that gets allocated if not successful.
 * @return true if successful
 */
bool cparse_object_save(cParseObject *obj, cParseError **error);

/*! saves a parse object in the background
 * @param obj the object instance
 * @param callback a callback issued after the object is saved
 * @param param a user defined parameter for the callback
 * @return the background thread identifier
 */
bool cparse_object_save_in_background(cParseObject *obj, cParseObjectCallback callback, void *param);

/*! updates a parse object
 * @param obj the object instance
 * @param attributes the object attributes to update
 * @param error a pointer to an error that gets allocated if not successful.
 * @return true if successful
 */
bool cparse_object_update(cParseObject *obj, cParseJson *attributes, cParseError **error);

/*! updates a parse object in the background
 * @param obj the object instance
 * @param attributes the attributes to update
 * @param callback the callback issued after the update
 * @param param a user defined parameter for the callback
 * @return a background thread identifier
 */
bool cparse_object_update_in_background(cParseObject *obj, cParseJson *attributes, cParseObjectCallback callback, void *param);

/*! deletes and object
 * @param obj the object instance
 * @param error a pointer to an error that gets allocated if not successful.
 * @return true if successful
 */
bool cparse_object_delete(cParseObject *obj, cParseError **error);

/*! deletes an object in the background
 * @param obj the object instance
 * @param callback a callback issues after the object is deleted
 * @param param a user defined parameter for the callback
 * @return the background thread identifier
 */
bool cparse_object_delete_in_background(cParseObject *obj, cParseObjectCallback callback, void *param);

/*! refreshes an object' attributes. This does not include other object refereces. (see fetch)
 * @param obj the object instance
 * @param error a pointer to an error that will get allocated if not successful
 * @return true if successful
 */
bool cparse_object_refresh(cParseObject *obj, cParseError **error);

/*! refreshes an object's attributes in the background. Does not refresh other object refereces. (see fetch)
 * @param obj the object instance
 * @param callback the callback issued after the refresh
 * @param param the user defined parameter for the callback
 * @return the background thread identifier
 */
bool cparse_object_refresh_in_background(cParseObject *obj, cParseObjectCallback callback, void *param);

/*! refreshes an object's attributes, including refereces to other objects.
 * @param obj the object instance
 * @param error a pointer to an error that gets allocated if not successful
 * @return true if successful
 */
bool cparse_object_fetch(cParseObject *obj, cParseError **error);

/*! refreshes an object's attributes in the background, including references to other objects
 * @param obj the object instance
 * @param callback the callback issues after the fetch
 * @param param the user defined parameter for the callback
 * @return a background thread identifier
 */
bool cparse_object_fetch_in_background(cParseObject *obj, cParseObjectCallback callback, void *param);

/* setters */

/*! tests if the object exists (was saved)
 * @param obj the object instance
 * @return true if the object exists
 */
bool cparse_object_exists(cParseObject *obj);

/*! sets a number attribute on an object
 * @param obj the object instance
 * @param key the key to identify the value
 * @param value the number attribute to set
 */
void cparse_object_set_number(cParseObject *obj, const char *key, cParseNumber value);

/*! sets a real (float) attribute on an object
 * @param obj the object instance
 * @param key the key to identify the value
 * @param value the attribute value to set
 */
void cparse_object_set_real(cParseObject *obj, const char *key, double value);

/*! sets a bool attribute on an object
 * @param obj the object instance
 * @param key the key to identify the value
 * @param value the attribute value to set
 */
void cparse_object_set_bool(cParseObject *obj, const char *key, bool value);

/*! sets a string attribute on an object
 * @param obj the object instance
 * @param key the key to identify the value
 * @param value the attribute value to set
 */
void cparse_object_set_string(cParseObject *obj, const char *key, const char *value);

/*! sets a json object attribute on an object
 * @param obj the object instance
 * @param key the key to identify the value
 * @param value the attribute value to set
 */
void cparse_object_set(cParseObject *obj, const char *key, cParseJson *value);

/*! tests if an object is a user object
 * @param obj the object instance
 * @return true if the object is a user object
 */
bool cparse_object_is_user(cParseObject *obj);

/*! sets a reference attribute to another object
 * @param obj the object instance
 * @param key the key to identify the reference
 * @param ref the object to reference
 */
void cparse_object_set_reference(cParseObject *obj, const char *key, cParseObject *ref);

/* getters */

/*! get a json attribute for an object
 * @param obj the object instance
 * @param key the key to identify the attribute value
 */
cParseJson *cparse_object_get(cParseObject *obj, const char *key);

/*! get a number attribute for an object. strings will be parsed, if no conversion exists error number is set to EINVAL
 * @param obj the object instance
 * @param key the key to identify the attribute value
 * @return the number or zero if not found
 */
cParseNumber cparse_object_get_number(cParseObject *obj, const char *key, cParseNumber def);

/*! get a double attribute for an object.  strings will be parsed if no conversion exists error number is set to EINVAL
 * @param obj the object instance
 * @param key the key to identify the attribute value
 */
double cparse_object_get_real(cParseObject *obj, const char *key, double def);

/*! get a bool attribute for an object
 * @param obj the object instance
 * @param key the key to identify the attribute value
 */
bool cparse_object_get_bool(cParseObject *obj, const char *key);

/*! get a string attribute for an object
 * @param obj the object instance
 * @param key the key to identify the attribute value
 */
const char *cparse_object_get_string(cParseObject *obj, const char *key);

/*! removes an attribute from an object and returns the value
 * @param obj the object instance
 * @param key the key to identify the value to remove
 * @return the removed attribute or NULL if not found
 */
cParseJson *cparse_object_remove_and_get(cParseObject *obj, const char *key);

/*! removes an attribute from an object
 * @param obj the object instance
 * @param key the key to identify the attribute value
 */
void cparse_object_remove(cParseObject *obj, const char *key);

/*! tests if an object has an attribute
 * @param obj the object instance
 * @param key the key to identify the value to test
 * @return true if the object contains the attribute
 */
bool cparse_object_contains(cParseObject *obj, const char *key);

/* iterator interface */

/*! iterates and objects attributes
 * @param obj the object instance
 * @param callback the callback for each object attribute
 * @param param an optional parameter for the callback
 */
void cparse_object_foreach_attribute(cParseObject *obj, cParseObjectAttributeCallback callback, void *param);

/*! gets the number of attributes in an object.  this does not include the common attributes - id, type, create at, or
 * updated at.
 * @param obj the object instance
 * @return the number of attributes
 */
size_t cparse_object_attribute_size(cParseObject *obj);

/*! merges an object's attributes with a json object, duplicate values will be overwritten with the json object.
 * @param obj the object instance
 * @param json the json to merge with the object.
 */
void cparse_object_merge_json(cParseObject *obj, cParseJson *json);

/*! formats an object into a json string
 * @param obj the object instance
 */
const char *cparse_object_to_json_string(cParseObject *obj);

/*! tests if an object is not a system object
 * @param obj the object instance
 */
bool cparse_object_is_object(cParseObject *obj);

/*! sets public access control list for an object
 * @param obj the object instance
 * @param access the access type (read/write)
 * @param value the access value
 */
void cparse_object_set_public_acl(cParseObject *obj, cParseAccess access, bool value);

/*! sets a user access control list for an object
 * @param obj the object instance
 * @param access the access type (read/write)
 * @param value the access value
 */
void cparse_object_set_user_acl(cParseObject *obj, cParseUser *user, cParseAccess access, bool value);

/*! sets a role access control list for an object
 * @param obj the object instance
 * @param access the access type (read/write)
 * @param value the access value
 */
void cparse_object_set_role_acl(cParseObject *obj, cParseRole *role, cParseAccess access, bool value);

END_DECL

#endif
