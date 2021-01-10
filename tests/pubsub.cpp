//
// Created by nbdy on 10.01.21.
//

#include "../pubsupp.h"
#include <iostream>


int main() {
    std::cout << "random string: " << StrPubSub::generateString() << std::endl;

    auto* ps = StrPubSub::get();
    std::string channel("chan");

    std::string cbn = ps->subscribe(channel, [](const std::string& msg) {
        std::cout << "received: " << msg << std::endl;
    });

    std::cout << "subscribed to " << cbn << " on " << channel << std::endl;

    const char* data = "some string data";
    std::cout << "sending '" << data << "'" << std::endl;
    ps->publish(channel, data);

    std::cout << "unsubscribing from topic '" << channel << "'" << std::endl;
    ps->unsubscribe(channel, cbn);

    std::cout << "sending '" << data << "' again" << std::endl;
    ps->publish(channel, data);

    StrPubSub::destroy();

    return 0;
}