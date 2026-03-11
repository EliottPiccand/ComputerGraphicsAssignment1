#pragma once

#include <array>
#include <cstddef>

template <typename T, size_t Size> class CyclicArray
{
  private:
    std::array<T, Size> data;
    size_t cursor;

    class Iterator
    {
      private:
        const CyclicArray *array;
        size_t index;
        size_t count;

      public:
        Iterator(const CyclicArray *a, size_t start, size_t c = 0) : array(a), index(start), count(c)
        {
        }

        const T &operator*() const
        {
            return array->data[index];
        }

        Iterator &operator++()
        {
            index = (index + 1) % Size;
            count += 1;
            return *this;
        }

        bool operator!=(const Iterator &other) const
        {
            return count != other.count;
        }
    };

  public:
    CyclicArray() : cursor(0) {};

    void push(const T &value)
    {
        data[cursor] = value;
        cursor = (cursor + 1) % Size;
    }

    const T &head() const {
        return data[(cursor + Size - 1) % Size];
    }

    Iterator begin() const
    {
        return Iterator(this, cursor, 0);
    }
    Iterator end() const
    {
        return Iterator(this, cursor, Size);
    }
};
