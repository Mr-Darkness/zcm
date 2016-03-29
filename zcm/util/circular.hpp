#pragma once
#include <iostream>
#include <cstdlib>
#include <cassert>

// A C++ circular buffer implementation designed for efficiency.
// No unneeded copies or initializations.
// Note: Nothing about this circular buffer is thread-safe
class CircularTest;

template <typename Element>
class Circular
{
  private:
    bool      _delete;
    size_t    _capacity;
    size_t    _front;
    size_t    _back;
    size_t    _size;
    Element** _data;

  public:
    Circular() : Circular(1) {}

    Circular(size_t capacity, bool deleteWhenDone = true)
    {
        assert(capacity > 0);

        this->_capacity = capacity;
        this->_data = new Element*[this->_capacity];
        assert(this->_data);

        this->_size = 0;
        this->_front = 0;
        this->_back = 0;

        this->_delete = deleteWhenDone;
    }

    ~Circular()
    {
        while (this->_size > 0) removeBack();

        delete[] this->_data;
    }

    size_t size() const
    {
        return this->_size;
    }

    size_t capacity() const
    {
        return this->_capacity;
    }

    bool isEmpty() const
    {
        return this->_size == 0;
    }

    bool isFull() const
    {
        return this->_size == this->_capacity;
    }

    void clear()
    {
        while (!isEmpty())
            removeFront();
    }

    bool pushBack(Element* elt)
    {
        if (isFull()) return false;

        this->_data[this->_back] = elt;

        this->_size++;
        this->_back += 1;

        // Handle wrap around
        if (this->_back == this->_capacity) this->_back = 0;
        return true;
    }

    bool pushFront(Element* elt)
    {
        if (isFull()) return false;

        this->_size++;
        if (this->_front != 0) this->_front -= 1;
        else this->_front = this->_capacity - 1;

        this->_data[this->_front] = elt;

        return true;
    }

    Element* front()
    {
        if (isEmpty()) return nullptr;

        return this->_data[this->_front];
    }

    Element* back()
    {
        if (isEmpty()) return nullptr;

        size_t temp;
        if (this->_back == 0) temp = this->_capacity - 1;
        else temp = this->_back - 1;

        return this->_data[temp];
    }

    void popFront()
    {
        assert(!isEmpty());

        this->_size--;
        this->_front += 1;

        // Handle wrap around
        if (this->_front == this->_capacity) this->_front = 0;
    }

    void popBack()
    {
        assert(!isEmpty());

        this->_size--;

        if (this->_back == 0) this->_back = this->_capacity - 1;
        else this->_back--;
    }

    void removeFront()
    {
        assert(!isEmpty());

        if (_delete)
            delete this->_data[this->_front];

        popFront();
    }

    void removeBack()
    {
        assert(!isEmpty());

        popBack();

        if (_delete)
            delete this->_data[this->_back];
    }

    void resize(size_t capacity)
    {
        assert(capacity > 0);

        while (this->_size > capacity) removeFront();

        Element** temp = new Element*[capacity]();
        if (this->_size > 0) {
            if (this->_front < this->_back) {
                memcpy(temp,
                       this->_data + this->_front,
                       this->_size * sizeof(Element*));
                this->_front = 0;
                this->_back = this->_size;
            } else if (this->_front >= this->_back) {
                size_t nWrapAround = this->_capacity - this->_front;
                size_t nWrapAroundBytes = nWrapAround * sizeof(Element*);
                memcpy(temp,
                       this->_data + this->_front,
                       nWrapAroundBytes);
                memcpy(temp + nWrapAround,
                       this->_data,
                       this->_back * sizeof(Element*));
                this->_front = 0;
                this->_back = this->_size;
            }
        } else {
            this->_front = 0;
            this->_back = 0;
        }
        this->_capacity = capacity;
        delete[] this->_data;
        this->_data = temp;
    }

    Element* operator[](size_t i)
    {
        if (i >= size())
            return nullptr;

        size_t idx = i + this->_front;
        if (idx >= this->_capacity) idx -= this->_capacity;

        return this->_data[idx];
    }

    friend class CircularTest;
};
