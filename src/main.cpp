#include <iostream>
#include <drogon/drogon.h>
#include <libresim/libresim.h>

int main() {
    drogon::app()
        .setLogPath(".")
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 80) //TODO: loadConfigFile("./config.json")
        .setThreadNum(16)
        .enableRunAsDaemon()
        .registerController(std::make_shared<libresim::api::v1::Time>())
        .run();
}