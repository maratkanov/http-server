#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

using boost::asio::ip::tcp;

std::string make_datetime_string() {
    using namespace std;    // time_t, time, ctime
    time_t now = time(0);
    return ctime(&now);
}

typedef boost::shared_ptr<tcp::socket> socket_ptr;

//size_t read_complete(char * buf, const boost::system::error_code & err, size_t bytes) {
//    if (err) {
//        return 0;
//    }
//    bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
//    return found ? 0 : 1;
//}

void client_session(socket_ptr socket) {
    while (true) {
        std::cout << "=== client while ===\n";
        char data[512];
        boost::system::error_code ignored_error;

        size_t len = socket->read_some(boost::asio::buffer(data), ignored_error);
        if (ignored_error == boost::asio::error::eof)
            break;
//        size_t len = boost::asio::read(*socket, boost::asio::buffer(data), boost::bind(read_complete, data, _1, _2));
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

int main() {
    try {
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);

        std::string address = "0.0.0.0";
        std::string port = "8080";

        tcp::resolver::query query(address, port);
        tcp::endpoint endpoint = *resolver.resolve(query);

        tcp::acceptor acceptor(io_service, endpoint);

        while (true) {
            std::cout << "===  main while  ===\n";
            socket_ptr sock(new tcp::socket(io_service));
            acceptor.accept(*sock);
            boost::thread(boost::bind(client_session, sock));
//            client_session(sock);
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
