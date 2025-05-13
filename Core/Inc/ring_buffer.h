#pragma once

#include <stdint.h>
#include <stdbool.h>

#define RING_BUFFER_SIZE 128  

typedef struct {
    uint8_t buffer[RING_BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} RingBuffer;

void  RingBuffer_Init(RingBuffer* rb);
bool  RingBuffer_Put(RingBuffer* rb, uint8_t data);
bool  RingBuffer_Get(RingBuffer* rb, uint8_t* data);
uint16_t RingBuffer_Available(const RingBuffer* rb);
bool  RingBuffer_IsEmpty(const RingBuffer* rb);
bool  RingBuffer_IsFull(const RingBuffer* rb);




