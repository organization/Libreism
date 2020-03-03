#pragma once
#include <drogon/HttpController.h>

namespace libresim::api {
    namespace v1 {
        class TimeHttpController : public drogon::HttpController<TimeHttpController, false> {
        public:
            METHOD_LIST_BEGIN
            METHOD_ADD(TimeHttpController::getStandardUnixTime, "/sttime", drogon::Get); //path is /libresim/api/v1/TimeHttpController/sttime
            METHOD_ADD(TimeHttpController::getServerClock, "/servtime/{url}", drogon::Get); //path is /libresim/api/v1/TimeHttpController/servtime/{arg1}
            METHOD_LIST_END

            using Callback = std::function<void(const drogon::HttpResponsePtr&)>;

            void getStandardUnixTime(const drogon::HttpRequestPtr& req, Callback&& callback) const;
            void getServerClock(const drogon::HttpRequestPtr& req, Callback&& callback, std::string&& url) const;
        };
    } //namespace v1
} //namespace libresim::api
