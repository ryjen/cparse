#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <cparse/error.h>
#include <cparse/parse.h>
#include <cparse/object.h>
#include "parse.test.h"
#include "client.h"

extern const char *cparse_app_id;

extern const char *cparse_facebook_app_id;

extern bool cparse_offline_messages;

extern bool cparse_error_messages;

struct obj_list {
    struct obj_list *next;
    cParseObject *obj;
    bool remote_delete;
};

struct obj_list *first_obj = NULL;

cParseObject *cparse_new_test_object(const char *name, int score)
{
    cParseObject *obj = NULL;

    obj = cparse_object_with_class_name(TEST_CLASS);

    cparse_object_set_string(obj, "playerName", name);
    cparse_object_set_number(obj, "score", score);

    cparse_add_test_object(obj);

    return obj;
}

void cparse_add_test_object(cParseObject *obj)
{
    struct obj_list *node = NULL;

    node = malloc(sizeof(struct obj_list));

    if (node != NULL) {
        node->obj = obj;
        node->remote_delete = false;

        node->next = first_obj;
        first_obj = node;
    }
}

int cparse_delete_test_object(cParseObject *obj)
{
    struct obj_list *node = NULL;

    for (node = first_obj; node != NULL; node = node->next) {
        if (node->obj == obj) {
            node->remote_delete = true;
            return 1;
        }
    }
    return 0;
}

int cparse_save_test_object(cParseObject *obj)
{
    cParseError *error = NULL;
    bool rval = false;

    cparse_get_client()->timeout = 10;

    rval = cparse_object_save(obj, &error);

    if (!rval || error) {
        if (error) {
            printf("Save error: %s\n", cparse_error_message(error));

            cparse_error_free(error);
        }
        return 0;
    }

    return cparse_delete_test_object(obj);
}

int cparse_cleanup_test_objects()
{
    struct obj_list *node, *next_node;

    for (node = first_obj; node != NULL; node = next_node) {
        next_node = node->next;

        if (node->obj) {
            if (node->remote_delete) {
                cParseError *error = NULL;

                if (!cparse_object_delete(node->obj, &error)) {
                    if (error) {
                        printf("delete error: %s\n", cparse_error_message(error));
                    }
                }
            }

            cparse_object_free(node->obj);
        }

        free(node);
    }

    first_obj = NULL;

    return 1;
}

int cparse_create_and_save_test_object(const char *name, int score)
{
    cParseObject *obj = cparse_new_test_object(name, score);

    return cparse_save_test_object(obj);
}

const char *rand_name()
{
    static char buf[100][5];
    static int index = 0;
    static const char *names[] = {"frank", "bobby", "mike", "ted", "tony", "rob", "erin"};

    int fuzz = rand() % 1000;

    int name = rand() % (sizeof(names) / sizeof(names[0]));

    ++index, index %= 5;
    snprintf(buf[index], 100, "%s%04d", names[name], fuzz);

    return buf[index];
}


static void cparse_test_setup()
{
}

static void cparse_test_teardown()
{
}

Suite *cparse_parse_suite(void)
{
    Suite *s = suite_create("Config");

    /* Core test case */
    TCase *tc = tcase_create("Config");
    tcase_add_checked_fixture(tc, cparse_test_setup, cparse_test_teardown);
    suite_add_tcase(s, tc);

    return s;
}
