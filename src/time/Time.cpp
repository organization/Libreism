#include <libresim/time/Time.h>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/algorithm/string.hpp>
#include <regex>
#include <iomanip>

namespace libresim::api::v1 {
    void Time::getStandardUnixTime(const drogon::HttpRequestPtr& req, Time::Callback&& callback) const {
        //TODO
    }

    // Thanks to bsergeev
    // https://github.com/boostorg/beast/issues/787#issuecomment-376259849
    struct ParsedURI {
        std::string protocol;
        std::string domain;  // only domain must be present
        std::string port;
        std::string resource;
        std::string query;   // everything after '?', possibly nothing
    };

    ParsedURI parseURI(const std::string& url) {
        ParsedURI result = ParsedURI();
        auto value_or = [](const std::string& value, std::string&& deflt) -> std::string {
            return (value.empty() ? deflt : value);
        };
        // Note: only "http", "https", "ws", and "wss" protocols are supported
        static const std::regex PARSE_URL{ R"((([httpsw]{2,5})://)?([^/ :]+)(:(\d+))?(/([^ ?]+)?)?/?\??([^/ ]+\=[^/ ]+)?)",
                                           std::regex_constants::ECMAScript | std::regex_constants::icase };
        std::smatch match;
        if (std::regex_match(url, match, PARSE_URL) && match.size() == 9) {
            result.protocol = value_or(boost::algorithm::to_lower_copy(std::string(match[2])), "http");
            result.domain = match[3];
            const bool is_sequre_protocol = (result.protocol == "https" || result.protocol == "wss");
            result.port = value_or(match[5], (is_sequre_protocol)? "443" : "80");
            result.resource = value_or(match[6], "/");
            result.query = match[8];
            assert(!result.domain.empty());
        }
        return result;
    }

    void Time::getServerClock(const drogon::HttpRequestPtr& rgeteq, Callback&& callback, std::string&& url) const {
        const auto parsedUrl = parseURI(url);

        namespace beast = boost::beast;

        beast::net::io_context ioContext;

        beast::net::ip::tcp::resolver resolver(ioContext);
        beast::tcp_stream stream(ioContext);

        const auto results = resolver.resolve(parsedUrl.domain, parsedUrl.port);

        stream.connect(results);

        beast::http::request<beast::http::string_body> beastReq{beast::http::verb::get, parsedUrl.query, 11};
        beastReq.set(beast::http::field::host, parsedUrl.domain);
        beastReq.set(beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        beast::http::write(stream, beastReq);

        beast::flat_buffer buffer;
        beast::http::response<beast::http::dynamic_body> res;

        beast::http::read(stream, buffer, res);

        const auto date = res.base()["date"].to_string();

        std::tm tm = {};
        std::stringstream ss(date);
        ss >> std::get_time(&tm, "%a, %d %b %Y %H:%M:%S %Z"); // RFC1123
        const auto unixTimeMs = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::from_time_t(std::mktime(&tm)).time_since_epoch()).count();

        const auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setBody(std::to_string(unixTimeMs));
        resp->setExpiredTime(0); //disable cache

        callback(resp);
    }
}