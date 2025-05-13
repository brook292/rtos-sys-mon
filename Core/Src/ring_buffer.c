#include "ring_buffer.h"

void RingBuffer_Init(RingBuffer* rb) {
    rb->head = 0;
    rb->tail = 0;
}

bool RingBuffer_Put(RingBuffer* rb, uint8_t data) {
    uint16_t next = (rb->head + 1) & (RING_BUFFER_SIZE-1);
    if (next == rb->tail) {
        return false;  // buffer full
    }
    rb->buffer[rb->head] = data;
    rb->head = next;
    return true;
}

bool RingBuffer_Get(RingBuffer* rb, uint8_t* data) {
    if (rb->head == rb->tail) {
        return false;  // buffer empty
    }
    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % RING_BUFFER_SIZE;
    return true;
}

uint16_t RingBuffer_Available(const RingBuffer* rb) {
    if (rb->head >= rb->tail)
        return rb->head - rb->tail;
    else
        return RING_BUFFER_SIZE - rb->tail + rb->head;
}

bool RingBuffer_IsEmpty(const RingBuffer* rb) {
    return rb->head == rb->tail;
}

bool RingBuffer_IsFull(const RingBuffer* rb) {
    return ((rb->head + 1) % RING_BUFFER_SIZE) == rb->tail;
}
