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
    while (true) {
        std::cout << "=== client while ===\n";
        char data[512];
        boost::system::error_code ignored_error;

//        size_t len = boost::asio::read(*socket, boost::asio::buffer(data), boost::asio::transfer_at_least(1), ignored_error);
        size_t len = socket_.read_some(boost::asio::buffer(data), ignored_error);
        if (ignored_error == boost::asio::error::eof)
            break;
        if (len > 0) {
            std::cout << data;
            std::string message = make_datetime_string();

            boost::asio::write(socket_, boost::asio::buffer(message), ignored_error);
            socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_error);
            socket_.close(ignored_error);
            break;
        }
    }
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
