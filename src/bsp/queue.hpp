/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : queue.hpp
 * @brief          : Header for queue.cpp file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024.12.10 STMicroelectronics.
 * All rights reserved.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
// SimpleQueue.h
#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <Arduino.h>

class SimpleQueue {
public:
    SimpleQueue(volatile void *buffer, size_t element_size, size_t capacity);
    void reset();
    size_t count() const;
    bool isEmpty() const;
    bool isFull() const;
    bool push(const void *element);
    bool pushCyclic(const void *element);
    bool pop(void *element);
    void *peek(size_t index) const;

private:
    volatile int head;
    volatile int tail;
    volatile size_t size;
    size_t capacity;
    volatile void *queue;
    size_t element_size;
};


#endif // QUEUE_HPP
