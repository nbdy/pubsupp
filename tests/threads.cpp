//
// Created by nbdy on 10.01.21.
//

#include <utility>
#include <iostream>

#include "../pubsupp.h"

class ThreadEvent {
    std::string threadName;
    std::string message;

public:
    ThreadEvent() = default;
    ThreadEvent(std::string name, std::string message): threadName(std::move(name)), message(std::move(message)) {}
    ~ThreadEvent() = default;

    [[nodiscard]] std::string getThreadName() const {
        return threadName;
    }

    [[nodiscard]] std::string getMessage() const {
        return message;
    }
};

using EventPubSub = TPubSub<const ThreadEvent&>;

int main() {
    int loops = 1000;
    auto* ps = EventPubSub::getInstance();

    auto loop = [ps, loops](const std::string& channel, const ThreadEvent& event){
        for(int i=0; i<loops; i++) ps->publish(channel, event);
    };

    std::string c = "threads";
    std::string sn = "threadsReceiver";

    int t1c = 0;
    int t2c = 0;

    auto scb = [&t1c, &t2c](const ThreadEvent& e) {
        auto n = e.getThreadName();
        if(n == "t1") t1c++;
        else t2c++;
    };

    ps->subscribe(c, sn, scb);

    std::thread t1 ([c, loop]() {
        std::cout << "starting loop t1" << std::endl;
        loop(c, ThreadEvent("t1", "."));
        std::cout << "finished loop t1" << std::endl;
    });

    t1.detach();

    std::thread t2 ([c, loop] () {
        std::cout << "starting loop t2" << std::endl;
        loop(c, ThreadEvent("t2", "."));
        std::cout << "finished loop t2" << std::endl;
    });

    t2.detach();

    while(t1c < loops || t2c < loops) {
        std::cout << t1c << ":" << t2c << std::endl;
    }

    return 0;
}