#pragma once

#include <array>
#include <chrono>
#include <iostream>
#include <string>

//Components of the Boost Library
#include <boost/asio.hpp>

namespace libreism {

    /**
    *  A Network Time Protocol Client that queries the DateTime from the Time Server located at hostname
    */
    class NTPClient {
    private:
        std::string _host_name;
        unsigned short _port;

    public:
        explicit NTPClient(std::string&& i_hostname);
        std::chrono::system_clock::time_point getCurrentTimestamp();
    };

} // namespace libreism