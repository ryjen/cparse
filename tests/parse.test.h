#ifndef CPARSE_TEST_H_
#define CPARSE_TEST_H_

#define TEST_CLASS "GameScore"

cParseObject *cparse_new_test_object(const char *name, int score);
int cparse_cleanup_test_object(cParseObject *obj);
int cparse_save_test_object(cParseObject *obj);
int cparse_cleanup_test_objects();
int cparse_create_and_save_test_object(const char *name, int score);
void cparse_add_test_object(cParseObject *obj);

const char *rand_name();
void read_env_config();
void read_test_config();
void die(const char *message);
void cleanup();

#endif
