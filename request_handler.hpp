#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include <string>
#include <boost/noncopyable.hpp>

class request_handler :
        private boost::noncopyable
{
public:
    explicit request_handler(const std::string& doc_root);
    void handle_request();
private:
    std::string doc_root_;
};

#endif // REQUEST_HANDLER_HPP
