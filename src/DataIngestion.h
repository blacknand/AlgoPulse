#ifndef DATA_INGESTION_H
#define DATA_INGESTION_H

#pragma once

#include <zmq.hpp>
#include <string>
#include <functional>

struct MarketData;

class DataIngestion {
public:
    using DataCallback = std::function<void(const MarketData&)>;
    DataIngestion(const std::string& endpoint, DataCallback callback);
    void start();
    void stop();

private:
    bool running;
    zmq::context_t context;
    zmq::socket_t socket;
    std::string endpoint;
    DataCallback callback;
};

#endif  // DATA_INGESTION_H