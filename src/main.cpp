#include <libresim/libresim.h>
#include <iostream>
#include <drogon/drogon.h>

int main() {
    drogon::app()
    .setLogPath(".")
    .setLogLevel(trantor::Logger::kWarn)
    .addListener("0.0.0.0", 80) //TODO: loadConfigFile("./config.json")
    .setThreadNum(16)
    .registerController(std::make_shared<libresim::api::v1::TimeHttpController>())
    .run();
}