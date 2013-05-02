#include <cparse/type/date.h>
#include <cparse/exception.h>
#include "../protocol.h"

namespace cparse
{
    namespace type
    {
        void Date::fromString(const std::string &s)
        {
            struct tm tp;

            if (!strptime(s.c_str(), FORMAT, &tp))
                throw Exception("unable to convert time string");

            value_ = mktime(&tp);
        }

        const char *const Date::FORMAT = "%FT%T%z";

        Date::Date() : value_(time(0))
        {}

        Date::Date(time_t value) : value_(value)
        {}

        Date::Date(const std::string &value)
        {
            fromString(value);
        }

        Date::Date(const Value &obj)
        {
            fromValue(obj);
        }

        void Date::fromValue(const Value &obj)
        {
            if(obj.contains("iso"))
                fromString(obj.getString("iso"));
        }

        Value Date::toValue() const {
            Value value;

            value.setString(protocol::KEY_TYPE, protocol::TYPE_DATE);

            value.setString("iso", toString());

            return value;
        }

        time_t Date::getTimestamp() const {
            return value_;
        }

        std::string Date::toString() const
        {
            char buf[BUFSIZ+1] = {0};

            strftime(buf, BUFSIZ, FORMAT, localtime(&value_));

            return buf;
        }
    }
}