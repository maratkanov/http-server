#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "request_handler.hpp"

namespace http {
namespace server {

/// Represents a single connection from a client.
class connection :
        public boost::enable_shared_from_this<connection>,
        private boost::noncopyable
{
public:
    explicit connection(boost::asio::io_service& io_service, request_handler& handler);

    /// Get the socket associated with the connection.
    boost::asio::ip::tcp::socket& socket();

    /// Start operations for the connection.
    void start();

private:
    /// Handle completion of a read operation.
    void handle_read(const boost::system::error_code& e,
                     std::size_t bytes_transferred);

    /// Handle completion of a write operation.
    void handle_write(const boost::system::error_code& e);

    /// Socket for the connection.
    boost::asio::ip::tcp::socket socket_;


    /// The handler used to process the incoming request.
    request_handler& request_handler_;

    /// Buffer for incoming data.
    boost::array<char, 8192> buffer_;

    std::string make_datetime_string();

//      /// The incoming request.
//      request request_;

//      /// The parser for the incoming request.
//      request_parser request_parser_;

//      /// The reply to be sent back to the client.
//      reply reply_;
};

typedef boost::shared_ptr<connection> connection_ptr;

}
}
#endif // CONNECTION_HPP
