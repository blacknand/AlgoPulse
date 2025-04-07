// src/HFTBot.cpp
#include "HFTBot.h"
#include <sstream>

HFTBot::HFTBot() {
    // Push recieved marketData into dataQueue
    ingestion = new DataIngestion("tcp://localhost:5555",
        [this](const MarketData& data) {
            std::lock_guard<std::mutex> lock(this->queueMutex);
            this->dataQueue.push(data);
        }
    );
}

HFTBot::~HFTBot() {
    delete ingestion;
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
// Currently checks for a large bid-ask spread.
void HFTBot::detectAnomaly(const MarketData& data) {
    double spread = data.askPrice - data.bidPrice;
    if (spread > 1.0) {
        std::cout << "Anomaly detected for " << data.symbol << ": Spread = " << spread << std::endl;
        // NOTE: Additional trade logic can be added here.
    }
}

// Start the bot: launch threads for data ingestion and processing.
void HFTBot::start() {
    // Thread for live data ingestion.
    std::thread ingestionThread([this]() {
        ingestion->start();
    });

    // Thread for processing incoming data.
    std::thread processor(&HFTBot::processData, this);

    std::cout << "HFT Bot started with live data ingestion...\n";

    // Run for a set duration for demonstration.
    std::this_thread::sleep_for(std::chrono::seconds(5));
    running = false;

    ingestion->stop();

    ingestionThread.join();
    processor.join();
}
