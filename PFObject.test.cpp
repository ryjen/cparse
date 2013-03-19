#include <cparse/Parse.h>
#include <cparse/PFObject.h>
#include <cparse/PFException.h>
#include <igloo/igloo.h>

using namespace cparse;
using namespace igloo;
using namespace std;

namespace cparse
{
	bool validate_class_name(const string &value);
}

Context(PFObjectTest)
{
	PFObject *obj_;

	static void SetUpContext() 
	{
	    Parse::set_application_id("CODG0SmrNhqoZOheWT0Q4sATFvQTdZhzOjGA5OGb");

	    Parse::set_api_key("wXu2P8buzDdTq02mQgJkxrJz1TEHNgOvKtDTUuZ7");
	}

	void SetUp() 
	{
	    obj_ = PFObject::objectWithClassName("TestCase");
	}

	void TearDown()
	{
		delete obj_;
	}

	Spec(ValidClassName)
	{
		bool value = validate_class_name("_@#$@#$^*");

		Assert::That(false, Equals(value));

		value = validate_class_name("1234ABC");

		Assert::That(false, Equals(value));

		value = validate_class_name("ACBD123412");

		Assert::That(true, Equals(value));
	}

	Spec(ObjectForKey)
	{
		AssertThrows(cparse::PFException, obj_->get("testVal1"));

		PFValue sValue("test string 1");

		obj_->set("testVal1", sValue);

		Assert::That(sValue, Equals(obj_->get("testVal1")));

		PFValue iValue(1234567LL);

		obj_->set("testVal2", iValue);

		Assert::That(iValue, Equals(obj_->get("testVal2")));

		PFValue bValue(true);

		obj_->set("testVal3", bValue);

		Assert::That(bValue, Equals(obj_->get("testVal3")));
	}

	Spec(test_cparse_object_save)
	{

		obj_->set_int("score", 1234);

		obj_->set_str("name", "spok");

		Assert::That(obj_->save(), Equals(true));

	    Assert::That(obj_->is_valid(), Equals(true));
	}
	/*
	void test_cparse_object_callback(CParseObject *obj, CParseError *error)
	{
	    fail_unless(obj->objectId != NULL);
	}

	START_TEST(test_cparse_object_save_in_background)
	{
	    cparse_object_set_number(cp_obj, "score", 4567);

	    cparse_object_set_string(cp_obj, "status", "saved in background");

	    pthread_t thread = cparse_object_save_in_background(cp_obj, test_cparse_object_callback);

	    pthread_join(thread, NULL);
	}
	END_TEST*/


	Spec(testSetObjectForKey)
	{
		obj_->set_int("id", 1234);

	    Assert::That(obj_->get_int("id"), Equals(1234));

	    obj_->set_str("id", "replaced");

	    Assert::That(obj_->get_str("id"), Equals("replaced"));
	}

	Spec(testRemoveObjectForKey)
	{
	    PFValue value(1234);

	    obj_->set("main", value);

	    Assert::That(obj_->get_str("main"), Equals("1234"));

	    obj_->remove("main");

	    Assert::That(obj_->contains("main"), Equals(false));

	}

};
