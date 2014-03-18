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
        acceptor_.accept(new_connection_->socket());
        boost::thread(boost::bind(&server::handle_client_session, this, new_connection_));
    }
}

void server::handle_client_session(connection_ptr connection_pointer) {
    connection_pointer->start();
}

}
}


