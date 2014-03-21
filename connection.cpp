#include "connection.hpp"
#include <vector>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/logic/tribool.hpp>

namespace http {
namespace server {

typedef boost::asio::buffers_iterator<boost::asio::streambuf::const_buffers_type> iterator;

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

    boost::asio::streambuf request_buf;
    boost::asio::read_until(socket_, request_buf, "\r\n\r\n");
    buffer_ << &request_buf;
    std::string request_string = buffer_.str();

    boost::tribool result = request_parser_.parse(request_, request_string.begin(), request_string.end());

//    std::string message = make_datetime_string();
    boost::system::error_code ignored_error;

    if (result) {
        std::cout << "REQUEST OK\n";
//        request_.printState();
        request_handler_.handle_request(request_, reply_);

    } else if (!result) {
        reply_ = reply::stock_reply(reply::bad_request);
        std::cout << "BAD REQUEST\n";
    }
    else {
        std::cout << " SOME ERROR OCCURED DURING PARSING REQUEST\n";
        reply_ = reply::stock_reply(reply::internal_server_error);
        // TODO: add logging
    }

    boost::asio::write(socket_, reply_.to_buffers(), ignored_error);

//    boost::asio::write(socket_, boost::asio::buffer(message), ignored_error);
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
