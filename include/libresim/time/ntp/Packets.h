/*
    The regulation of the number and byte order of communication between the client and the server are also synonyms for working with booost::aiso.
*/

// https://github.com/TeamIlluminate/Simple-client-server_CPP/blob/master/src/Packets.h

#pragma once

#include <iostream>
#include <boost/asio.hpp>

namespace Packets {
    using socket_ptr = std::shared_ptr<boost::asio::ip::tcp::socket>;
    using n_io_service = boost::asio::io_service; //net input\output service
    using tcp = boost::asio::ip::tcp;
    using udp = boost::asio::ip::udp;
} // namespace Packets

struct TimePacket {
    int8_t flags; // Control byte (8 flags, if necessary, you can control the start / end of the transaction, forwarding when a break, etc.)

    // Message command (is a symbol of a handler function)
    char command[3];
    // Data (arguments). You can use as you like.
    char data[3];
};

// Definition of NTP\UDP\IP packet
struct NtpPacket {
    uint8_t li_vn_mode = 0x1b; // Eight bits. li, vn, and mode.
    // li.   Two bits.   Leap indicator.
    // vn.   Three bits. Version number of the protocol.
    // mode. Three bits. Client will pick mode 3 for client.

    uint8_t stratum{}; // Eight bits. Stratum level of the local clock.
    uint8_t poll{}; // Eight bits. Maximum interval between successive messages.
    uint8_t precision{}; // Eight bits. Precision of the local clock.

    uint32_t rootDelay{}; // 32 bits. Total round trip delay time.
    uint32_t rootDispersion{}; // 32 bits. Max error aloud from primary clock source.
    uint32_t refId{}; // 32 bits. Reference clock identifier.

    uint32_t refTm_s{}; // 32 bits. Reference time-stamp seconds.
    uint32_t refTm_f{}; // 32 bits. Reference time-stamp fraction of a second.

    uint32_t origTm_s{}; // 32 bits. Originate time-stamp seconds.
    uint32_t origTm_f{}; // 32 bits. Originate time-stamp fraction of a second.

    uint32_t rxTm_s{}; // 32 bits. Received time-stamp seconds.
    uint32_t rxTm_f{}; // 32 bits. Received time-stamp fraction of a second.

    uint32_t txTm_s{}; // 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
    uint32_t txTm_f{}; // 32 bits. Transmit time-stamp fraction of a second.
};