#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include "connection.hpp"

namespace http {
namespace server {

using boost::asio::ip::tcp;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

class server
{
public:
    /// Construct the server to listen on the specified TCP address and port, and
    /// serve up files from the given directory.
    server(const std::string& address, const std::string& port, const std::string& doc_root);

    /// Run the server's main loop
    void run();



private:
    /// function to return current daytime
    std::string make_datetime_string();

    /// function to handle ckient connections
    void handle_client_session(socket_ptr socket);

    /// The io_service used to perform operations with sockets.
    boost::asio::io_service io_service_;

    /// Acceptor used to listen for incoming connections.
    boost::asio::ip::tcp::acceptor acceptor_;

    /// The next connection to be accepted.
    connection_ptr new_connection_;

    /// The handler for all incoming requests.
    request_handler request_handler_;
};

}
}

#endif // SERVER_HPP
