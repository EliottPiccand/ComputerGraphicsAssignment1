#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <optional>
#include <stdexcept>

template <typename T, size_t Size> class CyclicQueue
{
    static_assert(Size > 0, "CyclicQueue size cannot be zero");

  private:
    std::array<T, Size> data;
    size_t cursor; // Points to the next position to write
    size_t m_size; // Number of elements in the queue

  public:
    CyclicQueue() : cursor(0), m_size(0)
    {
    }

    void pushFront(T value)
    {
        if (m_size == Size)
        {
            throw std::runtime_error("CyclicQueue out of size");
        }

        data[cursor] = value;
        cursor = (cursor + 1) % Size;
        m_size = std::min(m_size + 1, Size);
    }

    void popBack()
    {
        if (m_size > 0)
        {
            m_size -= 1;
        }
    }

    size_t size()
    {
        return m_size;
    }

    T &front()
    {
        if (m_size == 0)
        {
            throw std::runtime_error("Cannot get the front of an empty queue");
        }

        return data[(cursor + Size - 1) % Size];
    }

    T &back()
    {
        if (m_size == 0)
        {
            throw std::runtime_error("Cannot get the back of an empty queue");
        }

        return data[(cursor + Size - m_size) % Size];
    }

    class Iterator
    {
      private:
        CyclicQueue *queue;
        size_t index;
        size_t remaining;

      public:
        Iterator(CyclicQueue *q, size_t start, size_t remaining) : queue(q), index(start), remaining(remaining)
        {
        }

        T &operator*()
        {
            return queue->data[index];
        }

        Iterator &operator++()
        {
            if (remaining == 0)
            {
                throw std::runtime_error("Iterator out of bounds");
            }

            index = (index + 1) % Size;
            remaining -= 1;
            return *this;
        }

        bool operator!=(Iterator &other)
        {
            return remaining != other.remaining;
        }
    };

    Iterator begin()
    {
        if (m_size == 0)
        {
            return end();
        }

        size_t start = (cursor + Size - m_size) % Size;
        return Iterator(this, start, m_size);
    }

    Iterator end()
    {
        size_t start = (cursor + Size - m_size) % Size;
        return Iterator(this, start, 0);
    }
};
