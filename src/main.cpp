#include <libreism/libreism.h>
#include <drogon/drogon.h>

int main() {
    drogon::app()
        .setLogPath(".")
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 8080) //TODO: loadConfigFile("./config.json")
        .setThreadNum(16)
        .registerController(std::make_shared<libreism::api::v1::TimeHttpController>())
        .run();
}