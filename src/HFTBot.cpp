#include "HFTBot.h"

void HFTBot::simulateMarketData() {
    while (running) {
        MarketData data{"AAPL", 150.0 + (rand() % 10) / 10.0, 150.5 + (rand() % 10) / 10.0,
                        100, 100, std::chrono::microseconds(std::chrono::system_clock::now().time_since_epoch())};
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            dataQueue.push(data);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Simulate data stream
    }
}

// Process data for anomalies
void HFTBot::processData() {
    while (running) {
        MarketData data;
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (dataQueue.empty()) continue;
            data = dataQueue.front();
            dataQueue.pop();
        }
        detectAnomaly(data);
    }
}

// Simple anomaly detection (placeholder)
void HFTBot::detectAnomaly(const MarketData& data) {
    double spread = data.askPrice - data.bidPrice;
    if (spread > 1.0) {  // Example: Large spread as anomaly
        std::cout << "Anomaly detected for " << data.symbol << ": Spread = " << spread << std::endl;
        // Trigger trade logic here
    }
}


HFTBot::HFTBot() {
    // Set up ZeroMQ socket (placeholder for real feed)
    socket.connect("tcp://localhost:5555");
    socket.set(zmq::sockopt::subscribe, "");  // Subscribe to all messages
}


void HFTBot::start() {
    std::thread dataFeed(&HFTBot::simulateMarketData, this);
    std::thread processor(&HFTBot::processData, this);
    std::cout << "HFT Bot started...\n";

    // Run for a bit, then stop (for demo)
    std::this_thread::sleep_for(std::chrono::seconds(5));
    running = false;

    dataFeed.join();
    processor.join();
}
