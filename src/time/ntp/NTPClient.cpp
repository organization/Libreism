#include <libresim/time/ntp/NTPClient.h>

using namespace libresim;

NTPClient::NTPClient() {
    constexpr auto nistNtp = "132.163.96.1";
    ntpServerEP = Packets::udp::endpoint(boost::asio::ip::address::from_string(nistNtp), 123);

    Packets::n_io_service service;

    // udp\ntp client config
    uSocket = std::make_shared<Packets::udp::socket>(service);

    try {
        uSocket->open(Packets::udp::v4());
        std::cout << "UDP Client started>\n";
    } catch (const std::exception& e) {
        std::cerr << "Cannot start UDP client:\n"
                  << e.what() << '\n';
    }
}

NTPClient& NTPClient::getInstance() {
    static NTPClient ntpc;
    return ntpc;
}

std::chrono::system_clock::time_point NTPClient::getCurrentTimestamp() {
    // Could create shared_ptr, but there is no context for it.
    auto* packet = new NtpPacket();

    uSocket->send_to(boost::asio::buffer(packet, 48), ntpServerEP);

    Packets::udp::endpoint ntpAnswerEndPoint;
    uSocket->receive_from(boost::asio::buffer(packet, 48), ntpAnswerEndPoint);

    time_t timestamp = (ntohl(packet->txTm_s) - NTP_TIMESTAMP_DELTA);

    delete packet;

    return std::chrono::system_clock::from_time_t(timestamp);
}