#include <cparse/type/geopoint.h>
#include "../protocol.h"

namespace cparse
{
    namespace type
    {
        GeoPoint::GeoPoint() {}

        GeoPoint::GeoPoint(double lat, double lon) : latitude_(lat), longitude_(lon)
        {}

        GeoPoint::GeoPoint(const JSON &obj)
        {
            fromJSON(obj);
        }

        GeoPoint::~GeoPoint()
        {}

        GeoPoint::GeoPoint(const GeoPoint &value) : latitude_(value.latitude_), longitude_(value.longitude_)
        {}

        GeoPoint::GeoPoint(GeoPoint &&value) : latitude_(value.latitude_), longitude_(value.longitude_)
        {}

        GeoPoint &GeoPoint::operator=(const GeoPoint &a)
        {
            if (this != &a)
            {
                latitude_ = a.latitude_;
                longitude_ = a.longitude_;
            }
            return *this;
        }

        GeoPoint &GeoPoint::operator=(GeoPoint && a)
        {
            if (this != &a)
            {
                latitude_ = a.latitude_;
                longitude_ = a.longitude_;
            }
            return *this;
        }


        void GeoPoint::fromJSON(const JSON &obj)
        {
            if (obj.contains("latitude"))
                latitude_ = obj.getDouble("latitude");

            if (obj.contains("longitude"))
                longitude_ = obj.getDouble("longitude");
        }

        double GeoPoint::getLatitude() const
        {
            return latitude_;
        }

        double GeoPoint::getLongitude() const
        {
            return longitude_;
        }

        void GeoPoint::setLatitude(double value)
        {
            latitude_ = value;
        }

        void GeoPoint::setLongitude(double value)
        {
            longitude_ = value;
        }

        JSON GeoPoint::toJSON() const
        {
            JSON value;

            value.setString(protocol::KEY_TYPE, protocol::TYPE_GEOPOINT);

            value.setDouble("latitude", latitude_);

            value.setDouble("longitude", longitude_);

            return value;
        }
    }
}