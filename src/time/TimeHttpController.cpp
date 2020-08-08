#include <libreism/time/TimeHttpController.h>
#include <libreism/time/ntp/NTPClient.h>
#include <iomanip>
#include <regex>
#include <boost/algorithm/string.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>

namespace libreism::api::v1 {
    TimeHttpController::TimeHttpController() {
        _ntpClient = new libreism::NTPClient("time.bora.net");
    }

    TimeHttpController::~TimeHttpController() {
        delete _ntpClient;
    }

    void TimeHttpController::getStandardUnixTime(const drogon::HttpRequestPtr& req, TimeHttpController::Callback&& callback) const {
        const auto timeBegin = std::chrono::steady_clock::now();
        const auto nistStandardTime = this->_ntpClient->getCurrentTimestamp();
        const auto timeEnd = std::chrono::steady_clock::now();

        const auto correctionStandardTime = nistStandardTime + (timeEnd - timeBegin);
        const auto unixTimeMs = std::chrono::duration_cast<std::chrono::microseconds>(correctionStandardTime.time_since_epoch()).count();

        const auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setBody(std::to_string(unixTimeMs));

        callback(resp);
    }

    // Thanks to bsergeev
    // https://github.com/boostorg/beast/issues/787#issuecomment-376259849
    struct ParsedURI {
        std::string protocol;
        std::string domain; // only domain must be present
        bool isSecureProtocol{};
        std::string port;
        std::string resource;
        std::string query; // everything after '?', possibly nothing
    };

    ParsedURI parseURI(const std::string& url) {
        ParsedURI result = ParsedURI();
        auto valueOr = [](const std::string& value, std::string&& deflt) -> auto {
            return (value.empty() ? deflt : value);
        };
        // Note: only "http", "https", "ws", and "wss" protocols are supported
        static const std::regex PARSE_URL{ R"((([httpsw]{2,5})://)?([^/ :]+)(:(\d+))?(/([^ ?]+)?)?/?\??([^/ ]+\=[^/ ]+)?)",
                                           std::regex_constants::ECMAScript | std::regex_constants::icase };
        std::smatch match;
        if (std::regex_match(url, match, PARSE_URL) && match.size() == 9) {
            result.protocol = valueOr(boost::algorithm::to_lower_copy(std::string(match[2])), "http");
            result.domain = match[3];
            result.isSecureProtocol = (result.protocol == "https" || result.protocol == "wss");
            result.port = valueOr(match[5], (result.isSecureProtocol) ? "443" : "80");
            result.resource = valueOr(match[6], "/");
            result.query = match[8];
            assert(!result.domain.empty());
        }
        return result;
    }

    boost::string_view parseServerTime(const ParsedURI& parsedUrl) {
        namespace beast = boost::beast;

        beast::net::io_context ioContext;

        beast::net::ip::tcp::resolver resolver(ioContext);
        beast::tcp_stream stream(ioContext);

        boost::asio::ip::basic_resolver<boost::asio::ip::tcp, boost::asio::executor>::results_type results;
        try {
            results = resolver.resolve(parsedUrl.domain, parsedUrl.port);
            stream.connect(results);
        } catch (const boost::system::system_error& e) {
            std::cerr << e.what() << std::endl;
            return "";
        }

        beast::http::request<beast::http::string_body> beastReq{ beast::http::verb::head, parsedUrl.query, 11 };
        beastReq.set(beast::http::field::host, parsedUrl.domain);
        beastReq.set(beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        beast::error_code writeErrorCode;
        beast::http::write(stream, beastReq, writeErrorCode);
        if (writeErrorCode) {
            std::cerr << writeErrorCode.message() << std::endl;
            return "";
        }

        beast::flat_buffer buffer;

        beast::http::response_parser<beast::http::empty_body> parser;
        parser.skip(true);

        beast::error_code readErrorCode;

        beast::http::read(stream, buffer, parser, readErrorCode);
        if (readErrorCode) {
            std::cerr << readErrorCode.message() << std::endl;
            return "";
        }

        const auto date = parser.release().base()["date"];
        return date;
    }

    boost::string_view parseServerTimeSsl(const ParsedURI& parsedUrl) {
        namespace beast = boost::beast;

        beast::net::io_context ioContext;

        beast::net::ssl::context sslContext(beast::net::ssl::context::tlsv13_client);
        sslContext.set_default_verify_paths();
        sslContext.set_verify_mode(beast::net::ssl::verify_none);
        sslContext.set_options(beast::net::ssl::context::default_workarounds | beast::net::ssl::context::no_sslv2 | beast::net::ssl::context::no_sslv3);

        beast::net::ip::tcp::resolver resolver(ioContext);
        beast::ssl_stream<beast::tcp_stream> stream(ioContext, sslContext);

        if (!SSL_set_tlsext_host_name(stream.native_handle(), parsedUrl.domain.c_str())) {
            beast::error_code ec{ static_cast<int>(::ERR_get_error()), beast::net::error::get_ssl_category() };
            std::cerr << ec.message() << std::endl;
            return "";
        }

        boost::asio::ip::basic_resolver<boost::asio::ip::tcp, boost::asio::executor>::results_type results;
        try {
            results = resolver.resolve(parsedUrl.domain, parsedUrl.port);

            beast::get_lowest_layer(stream).connect(results);
            stream.handshake(beast::net::ssl::stream_base::client);
        } catch (const boost::system::system_error& e) {
            std::cerr << e.what() << std::endl;
            return "";
        }

        beast::http::request<beast::http::string_body> beastReq{ beast::http::verb::head, parsedUrl.query, 11 };
        beastReq.set(beast::http::field::host, parsedUrl.domain);
        beastReq.set(beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        beast::error_code writeErrorCode;
        beast::http::write(stream, beastReq, writeErrorCode);
        if (writeErrorCode) {
            std::cerr << writeErrorCode.message() << std::endl;
            return "";
        }

        beast::flat_buffer buffer;

        beast::http::response_parser<beast::http::empty_body> parser;
        parser.skip(true);

        beast::error_code readErrorCode;

        beast::http::read(stream, buffer, parser, readErrorCode);
        if (readErrorCode) {
            std::cerr << readErrorCode.message() << std::endl;
            return "";
        }

        const auto date = parser.release().base()["date"];
        return date;
    }

    void TimeHttpController::getServerTime(const drogon::HttpRequestPtr& rgeteq, Callback&& callback, std::string&& url) const {
        const auto parsedUrl = parseURI(url);

        const auto timeBegin = std::chrono::steady_clock::now();

        boost::string_view dateStringView = parsedUrl.isSecureProtocol ? parseServerTimeSsl(parsedUrl) : parseServerTime(parsedUrl);

        if (dateStringView.empty()) {
            const auto resp = drogon::HttpResponse::newNotFoundResponse();
            callback(resp);
            return;
        }

        const auto date = dateStringView.to_string();

        const auto timeEnd = std::chrono::steady_clock::now();

        std::tm tm = {};
        std::stringstream ss(date);
        ss >> std::get_time(&tm, "%a, %d %b %Y %H:%M:%S %Z"); // RFC1123

        const auto serverTime = std::chrono::system_clock::from_time_t(std::mktime(&tm)) + (timeEnd - timeBegin);
        const auto unixTimeMs = std::chrono::duration_cast<std::chrono::microseconds>(serverTime.time_since_epoch()).count();

        const auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setBody(std::to_string(unixTimeMs));

        callback(resp);
    }

    void TimeHttpController::getServerTimeJson(const drogon::HttpRequestPtr& req, Callback&& callback, std::string&& url) const {
        const auto parsedUrl = parseURI(url);

        const auto timeBegin = std::chrono::steady_clock::now();
        const auto nistStandardTime = this->_ntpClient->getCurrentTimestamp();

        boost::string_view dateStringView = parsedUrl.isSecureProtocol ? parseServerTimeSsl(parsedUrl) : parseServerTime(parsedUrl);

        if (dateStringView.empty()) {
            Json::Value json;
            json["result"] = false;
            auto resp = drogon::HttpResponse::newHttpJsonResponse(json);

            callback(resp);

            return;
        }

        const auto date = dateStringView.to_string();

        std::tm tm = {};
        std::stringstream ss(date);
        ss >> std::get_time(&tm, "%a, %d %b %Y %H:%M:%S %Z"); // RFC1123

        const auto timeEnd = std::chrono::steady_clock::now();

        const auto serverTime = std::chrono::system_clock::from_time_t(std::mktime(&tm)) + (timeEnd - timeBegin);
        const int serverUnixTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(serverTime.time_since_epoch()).count();
        const int standardUnixTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(nistStandardTime.time_since_epoch()).count();
        const int networkLatency = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeBegin).count();

        Json::Value json;
        json["result"] = true;
        json["server_time"] = serverUnixTimeMs;
        json["standard_time"] = standardUnixTimeMs;
        json["network_latency"] = networkLatency;

        auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
        resp->addHeader("Access-Control-Allow-Origin", "https://time.zvz.be");
        callback(resp);
    }
} // namespace libreism::api::v1
