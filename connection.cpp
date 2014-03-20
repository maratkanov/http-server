#include "connection.hpp"
#include <vector>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

namespace http {
namespace server {

connection::connection(boost::asio::io_service& io_service, request_handler& handler)
  : socket_(io_service),
    request_handler_(handler)
{
}

boost::asio::ip::tcp::socket& connection::socket()
{
  return socket_;
}

void connection::start() {
    boost::asio::streambuf b;
    boost::asio::read_until(socket_, b, "\r\n\r\n");

    std::istream is(&b);
    std::string s;
    while (std::getline(is, s)) {
        std::cout << s << std::endl;
    }

    std::string message = make_datetime_string();
    boost::system::error_code ignored_error;

    boost::asio::write(socket_, boost::asio::buffer(message), ignored_error);
}

void connection::handle_read(const boost::system::error_code& e, std::size_t bytes_transferred) {
}

void connection::handle_write(const boost::system::error_code &e) {
}

std::string connection::make_datetime_string() {
    using namespace std;    // time_t, time, ctime
    time_t now = time(0);
    return ctime(&now);
}

}
}
