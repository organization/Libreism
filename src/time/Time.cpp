#include <libresim/time/Time.h>

namespace libresim::api::v1 {
    void Time::getStandardUnixTime(const drogon::HttpRequestPtr& req, Time::Callback&& callback) const {
        //TODO
    }

    void Time::getServerClock(const drogon::HttpRequestPtr& req, Callback&& callback, std::string&& url) const {
        //TODO
    }
}