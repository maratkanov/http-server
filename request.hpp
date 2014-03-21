#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <vector>
#include "header.hpp"
#include <iostream>

namespace http {
namespace server {

/// A request received from a client.
struct request
{
    std::string method;
    std::string uri;
    int http_version_major;
    int http_version_minor;
    std::vector<header> headers;

    void printState() {
        using namespace std;

        cout << "\n=== request state ===\n\n";

        cout << "method: " << method << endl;
        cout << "uri: " << uri << endl;

        cout << "http_version_major: " << http_version_major << endl;
        cout << "http_version_minor: " << http_version_minor << endl;

        cout << "\n--- headers ---\n\n";

        for (int i = 0; i < headers.size(); ++i) {
            cout << headers[i].name << " : " << headers.at(i).value << endl;
        }

        cout << "\n--- headers ---\n";

        cout << "\n=== request state ===\n";
    }
};

}
}

#endif // REQUEST_HPP
