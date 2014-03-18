#include "server.hpp"
#include <ctime>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

namespace http {
namespace server {

server::server(const std::string &address, const std::string &port, const std::string &doc_root) :
    io_service_(),
    acceptor_(io_service_),
    new_connection_(),
    request_handler_(doc_root)
{
    // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
    boost::asio::ip::tcp::resolver resolver(io_service_);
    boost::asio::ip::tcp::resolver::query query(address, port);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();
}

void server::run() {
    while (true) {
        std::cout << "===  main while  ===\n";

        new_connection_.reset(new connection(io_service_, request_handler_));
//        socket_ptr sock(new tcp::socket(io_service_));
//        acceptor_.accept(*sock);
        acceptor_.accept(new_connection_->socket());
//        boost::thread(boost::bind(&server::handle_client_session, this, sock));
//        boost::thread(boost::bind(&connection::start(), new_connection_);
        new_connection_->start();
    }
}

std::string server::make_datetime_string() {
    using namespace std;    // time_t, time, ctime
    time_t now = time(0);
    return ctime(&now);
}

void server::handle_client_session(socket_ptr socket) {
    while (true) {
        std::cout << "=== client while ===\n";
        char data[512];
        boost::system::error_code ignored_error;

//        size_t len = boost::asio::read(*socket, boost::asio::buffer(data), boost::asio::transfer_at_least(1), ignored_error);
        size_t len = socket->read_some(boost::asio::buffer(data), ignored_error);
        if (ignored_error == boost::asio::error::eof)
            break;
        if (len > 0) {
            std::cout << data;
            std::string message = make_datetime_string();

            boost::asio::write(*socket, boost::asio::buffer(message), ignored_error);
            *socket->shutdown(tcp::socket::shutdown_both, ignored_error);
            *socket->close(ignored_error);
            break;
        }
    }
}

}
}


