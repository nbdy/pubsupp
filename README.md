# pubsupp
`cpp17 header only pubsub library`
## features
- [X] ipc
- [X] [thread safe](https://github.com/nbdy/pubsupp/blob/master/tests/threads.cpp)
- [X] small (127 sloc / 3.4KB header size / 15KB .so size / 8B .a size)

## example
```c++
#include "pubsupp.h"
#include <iostream>

int main() {
    auto* ps = TPubSub<const std::string&>::getInstance();
    // or StrPubSub::getInstance();
    
    ps.subscribe("foo", [](const std::string& msg) {
        std::cout << msg << std::endl;
    });
    
    ps.publish("foo", "bar");
    
    return 0;
}
// or look at the tests
```