#include <libreism/time/ntp/NTPClient.h>

namespace libreism {

    /**
     *  NTPClient
     *  @Param i_hostname - The time server host name which you are connecting to obtain the time
     *						eg. the pool.ntp.org project virtual cluster of timeservers
     */
    NTPClient::NTPClient(std::string&& i_hostname)
    : _host_name(i_hostname), _port(123) {
        //Host name is defined by you and port number is 123 for time protocol
    }

    /**
     * RequestDatetime_UNIX()
     * @Returns long - number of seconds from the Unix Epoch start time
     */
    std::chrono::system_clock::time_point NTPClient::getCurrentTimestamp() {
        time_t timeRecv = 0;

        boost::asio::io_service io_service;

        boost::asio::ip::udp::resolver resolver(io_service);
        boost::asio::ip::udp::resolver::query query(
            boost::asio::ip::udp::v4(),
            this->_host_name,
            "ntp");

        boost::asio::ip::udp::endpoint receiver_endpoint = *resolver.resolve(query);

        boost::asio::ip::udp::socket socket(io_service);
        socket.open(boost::asio::ip::udp::v4());

        std::array<unsigned char, 48> sendBuf{ 010, 0, 0, 0, 0, 0, 0, 0, 0 };

        socket.send_to(boost::asio::buffer(sendBuf), receiver_endpoint);

        std::array<unsigned long, 1024> recvBuf{};
        boost::asio::ip::udp::endpoint sender_endpoint;

        try {
            size_t len = socket.receive_from(
                boost::asio::buffer(recvBuf),
                sender_endpoint);

            timeRecv = ntohl((time_t) recvBuf[4]);

            timeRecv -= 2208988800U; //Unix time starts from 01/01/1970 == 2208988800U

        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }

        return std::chrono::system_clock::from_time_t(timeRecv);
    }

}