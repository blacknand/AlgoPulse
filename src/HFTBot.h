#ifndef HFT_BOT_H
#define HFT_BOT_H

#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <boost/asio.hpp>
#include <zmq.hpp>

#include "DataIngestion.h"


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
    std::queue<MarketData> dataQueue;  
    std::mutex queueMutex;             
    bool running = true;

    void processData();
    void detectAnomaly(const MarketData& data);

    DataIngestion* ingestion;
public:
    HFTBot();
    ~HFTBot();
    void start();
};

#endif // HFT_BOT_H