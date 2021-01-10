#ifndef PUBSUPP_PUBSUPP_H
#define PUBSUPP_PUBSUPP_H

#include <map>
#include <mutex>
#include <thread>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <functional>

template <typename T> using SubscribeCallback = std::function<void(T)>;
template <typename T> using CallbackMap = std::map<std::string, SubscribeCallback<T>>;
template <typename T> using SubscriptionMap = std::map<std::string, CallbackMap<T>>;

/*!
 * publish subscribe library for inter class communication
 */
template <typename T>
class TPubSub {
    inline static TPubSub* instance = nullptr;

    std::mutex mtx;
    SubscriptionMap<T> subscriptions;
public:
    TPubSub() = default;

    /*!
     * returns a PubSub instance, use this to instantiate the PubSub object
     * @return Pubsub*
     */
    static TPubSub* getInstance() {
        if(instance == nullptr) instance = new TPubSub<T>();
        return instance;
    }

    /*!
     * @return getInstance();
     */
    static TPubSub* get() {
        return getInstance();
    }

    /*!
     * delete the current instance
     */
    static void destroy() {
        delete instance;
    }

    /*!
     * generates a random string
     * @param length
     * @param charset
     * @return std::string with specified length
     */
    static std::string generateString(int length=64, std::string charset="123456789BCDFGHJKLMNPQRSTVWXZbcdfghjklmnpqrstvwxz") {
        std::string r;
        for(int _ = 0; _ < length; _++) r += charset[rand() % (charset.size() - 1)];
        return r;
    };

    /*!
     * publish a message to this channel
     * @param channel
     * @param msg
     */
    void publish(const std::string& channel, T msg) {
        mtx.lock();
        for(const auto& topic : subscriptions) {
            for(const auto& kv : topic.second) kv.second(msg);
        }
        mtx.unlock();
    };

    /*!
     * subscribe to a topic
     * @param channel
     * @param callback
     * @return callback name, should be stored if you wanna unsubscribe
     */
    std::string subscribe(const std::string& channel, SubscribeCallback<T> callback) {
        auto r = generateString();
        mtx.lock();
        subscriptions[channel][r] = callback;
        mtx.unlock();
        return r;
    }

    /*!
     * subscribe to a topic
     * @param channel
     * @param callbackName
     * @param callback
     */
    void subscribe(const std::string& channel, const std::string& callbackName, SubscribeCallback<T> callback) {
        mtx.lock();
        subscriptions[channel][callbackName] = callback;
        mtx.unlock();
    }

    /*!
     * unsubscribe from a topic
     * @param channel
     * @param callbackName
     */
    void unsubscribe(const std::string& channel, const std::string& callbackName) {
        mtx.lock();
        subscriptions[channel].erase(callbackName);
        mtx.unlock();
    }

    /*!
     * remove all callbacks for a topic
     * @param channel
     */
    void clear(const std::string& channel) {
        mtx.lock();
        subscriptions[channel].clear();
        mtx.unlock();
    }

    /*!
     * remove all callbacks
     */
    void clear() {
        mtx.lock();
        subscriptions.clear();
        mtx.unlock();
    }
};

class VoidPubSub : public TPubSub<void*> {
public:
    VoidPubSub() = default;
};

class StrPubSub : public TPubSub<const std::string&> {
public:
    StrPubSub() = default;
};


#endif //PUBSUPP_PUBSUPP_H
