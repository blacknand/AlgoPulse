#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <boost/asio.hpp>
#include <zmq.hpp>


// Simulated market data structure
struct MarketData {
    std::string symbol;
    double bidPrice;
    double askPrice;
    int bidVolume;
    int askVolume;
    std::chrono::microseconds timestamp;
};


class HFTBot {
private:
    zmq::context_t context{1};  // ZeroMQ context for messaging
    zmq::socket_t socket{context, ZMQ_SUB};  // Subscriber socket
    std::queue<MarketData> dataQueue;  // Queue for incoming data
    std::mutex queueMutex;  // Thread safety
    bool running = true;

    void simulateMarketData();
    void processData();
    void detectAnomaly(const MarketData& data);
public:
    HFTBot();
    void start();
}