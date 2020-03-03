#pragma once
#include <libresim/time/ntp/Packets.h>

namespace libresim {
    class NTPClient {
    public:
        static NTPClient& getInstance();
        std::chrono::system_clock::time_point getCurrentTimestamp();

        NTPClient(const NTPClient&) = delete;
        NTPClient operator=(const NTPClient&) = delete;

    private:
        NTPClient();
        ~NTPClient() {
            delete uSocket;
        };

        uint64_t NTP_TIMESTAMP_DELTA = 2208988800;
        Packets::udp::endpoint ntpServerEP;
        Packets::udp::socket* uSocket; // UDP client-socket to communicate with NTP server
    };

} // namespace libresim