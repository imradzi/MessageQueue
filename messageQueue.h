#pragma once
#include <queue>
#include <condition_variable>
#include <mutex>

/////////////////////////////////////////////////////////////////////////////
// Name:        messageQueue.h
// Purpose:     messageQueue: Simple Message Queue for intra-thread communication
// Author:      Julian Smart
// Modified by:
// Created:     13 July 2020
// Copyright:   (c) MR Ibrahim
// Licence:     GNU GENERAL PUBLIC LICENSE
/////////////////////////////////////////////////////////////////////////////

template<typename T>
class MessageQueue {
    std::mutex mtxCond;
    std::condition_variable cond;
    std::queue<T> queue;
    std::mutex mtxQueue;
    bool isReady;

public:
    MessageQueue() : isReady(false) {}
    MessageQueue(const MessageQueue&) = delete;
    MessageQueue& operator=(const MessageQueue&) = delete;
    void Send(const T& item) {
        mtxQueue.lock();
        queue.push(item);
        mtxQueue.unlock();

        mtxCond.lock();
        isReady = true;  // this is to guard against 'spurious' signal;
        mtxCond.unlock();

        cond.notify_one();
    }

    bool Receive(T& item, size_t = 0) {
        std::lock_guard<std::mutex> _lock(mtxQueue);
        if (queue.empty()) {
            return false;
        }
        std::unique_lock<std::mutex> lock(mtxCond);
        cond.wait(lock, [&]() { return isReady; });  // this will loop waiting and locking and unlocking the mtxCond;
                                                     // until isReady is true; why? Because sometime Receive may be call before anybody sends
                                                     // when return, the lock is remained locked.
        item = queue.front();
        queue.pop();
        return true;
    }
};
