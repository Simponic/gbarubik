
#ifndef USU_VECTOR_HPP
#define USU_VECTOR_HPP

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <tuple>

namespace usu {
template <typename T> class vector {
public:
  using size_type = std::size_t;
  using reference = T &;
  using pointer = std::shared_ptr<T[]>;
  using value_type = T;
  using resize_type = std::function<size_type(size_type)>;

  static resize_type DEFAULT_RESIZER;
  static size_type DEFAULT_CAPACITY;

  vector();
  vector(size_type size);
  vector(resize_type resize);
  vector(std::initializer_list<T> list);
  vector(size_type size, resize_type resize);
  vector(std::initializer_list<T> list, resize_type resize);

  class iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    iterator();
    iterator(pointer data);
    iterator(size_type pos, pointer data);
    iterator(const iterator &obj);
    iterator(iterator &&obj) noexcept;

    iterator &operator=(const iterator &rhs);
    iterator &operator=(iterator &&rhs);
    bool operator==(const iterator &rhs) const;
    bool operator!=(const iterator &rhs) const;
    iterator operator++();
    iterator operator--();
    iterator operator++(int);
    iterator operator--(int);
    value_type operator*();
    std::shared_ptr<T> operator->();

  private:
    pointer m_data;
    size_type m_pos;
  };

  reference operator[](size_type index);
  void add(T value);
  void insert(size_type index, T value);
  void remove(size_type index);
  void clear();
  size_type size();
  size_type capacity();
  iterator begin();
  iterator end();

protected:
  pointer m_data;
  size_type m_capacity;
  size_type m_size;
  resize_type m_resize;

  void maybe_resize();
};

template <> class vector<bool> : public vector<std::uint8_t> {
public:
  using size_type = std::size_t;
  using pointer = std::shared_ptr<std::uint8_t[]>;
  using resize_type = std::function<size_type(size_type)>;

  vector();
  vector(size_type size);
  vector(resize_type resize);
  vector(std::initializer_list<bool> list);
  vector(size_type size, resize_type resize);
  vector(std::initializer_list<bool> list, resize_type resize);

  class reference {
  public:
    reference();
    reference(size_type pos, pointer data);
    reference &operator=(bool value);
    operator bool();
    bool operator==(const reference &rhs) const;
    bool operator!=(const reference &rhs) const;

    bool get() const;
    void add();
    void sub();

  private:
    size_type m_pos;
    pointer m_data;
  };

  class iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    iterator();
    iterator(pointer data);
    iterator(size_type pos, pointer data);
    iterator(const iterator &obj);
    iterator(iterator &&obj) noexcept;

    iterator &operator=(const iterator &rhs);
    iterator &operator=(iterator &&rhs);
    bool operator==(const iterator &rhs) const;
    bool operator!=(const iterator &rhs) const;
    iterator operator++();
    iterator operator--();
    iterator operator++(int);
    iterator operator--(int);
    reference operator*();

  private:
    reference m_ref;
  };

  reference operator[](size_type index);

  void add(bool value);
  void insert(size_type index, bool value);
  void remove(size_type index);

  iterator begin();
  iterator end();

private:
  void maybe_resize();
};

template <typename T>
vector<T>::iterator::iterator() : vector<T>::iterator::iterator(nullptr) {}

template <typename T>
vector<T>::iterator::iterator(pointer data) : m_data(data), m_pos(0) {}

template <typename T>
vector<T>::iterator::iterator(size_type pos, pointer data)
    : m_data(data), m_pos(pos) {}

template <typename T> vector<T>::iterator::iterator(const iterator &obj) {
  m_pos = obj.m_pos;
  m_data = obj.m_data;
}

template <typename T> vector<T>::iterator::iterator(iterator &&obj) noexcept {
  m_pos = obj.m_pos;
  m_data = obj.m_data;
  obj.m_pos = 0;
  obj.m_data = nullptr;
}

template <typename T>
bool vector<T>::iterator::operator!=(const iterator &rhs) const {
  return m_pos != rhs.m_pos;
}

template <typename T>
bool vector<T>::iterator::operator==(const iterator &rhs) const {
  return m_pos == rhs.m_pos;
}

template <typename T>
typename vector<T>::iterator vector<T>::iterator::operator++() {
  m_pos++;
  return *this;
}

template <typename T>
typename vector<T>::iterator vector<T>::iterator::operator--() {
  m_pos--;
  return *this;
}

template <typename T>
typename vector<T>::iterator vector<T>::iterator::operator++(int) {
  iterator it = *this;
  m_pos++;
  return it;
}

template <typename T>
typename vector<T>::iterator vector<T>::iterator::operator--(int) {
  iterator it = *this;
  m_pos--;
  return it;
}

template <typename T>
typename vector<T>::value_type vector<T>::iterator::operator*() {
  return m_data[m_pos];
}

template <typename T> std::shared_ptr<T> vector<T>::iterator::operator->() {
  return std::make_shared<T>(m_data[m_pos]);
}

template <typename T>
typename vector<T>::resize_type vector<T>::DEFAULT_RESIZER =
    [](size_type currentCapacity) -> size_type { return currentCapacity * 2; };

template <typename T>
typename vector<T>::size_type vector<T>::DEFAULT_CAPACITY = 10;

template <typename T>
vector<T>::vector() : usu::vector<T>(0, DEFAULT_RESIZER) {}

template <typename T>
usu::vector<T>::vector(size_type size)
    : usu::vector<T>(size, DEFAULT_RESIZER) {}

template <typename T>
vector<T>::vector(resize_type resize) : usu::vector<T>(0, resize) {}

template <typename T>
vector<T>::vector(std::initializer_list<T> list)
    : usu::vector<T>(list, DEFAULT_RESIZER) {}

template <typename T>
vector<T>::vector(std::initializer_list<T> list, resize_type resize)
    : usu::vector<T>(0, resize) {
  for (auto i = list.begin(); i < list.end(); ++i)
    add(*i);
}

template <typename T>
vector<T>::vector(size_type size, resize_type resize)
    : m_data(nullptr),
      m_capacity(DEFAULT_CAPACITY > size ? DEFAULT_CAPACITY
                                         : DEFAULT_RESIZER(size)),
      m_size(size), m_resize(resize) {
  maybe_resize();
}

template <typename T> void vector<T>::maybe_resize() {
  if (m_data == nullptr)
    m_data = std::make_shared<T[]>(m_capacity);
  if (m_size >= m_capacity) {
    m_capacity = std::max(m_capacity == 0 ? 1 : m_resize(m_capacity), m_size);

    pointer newData = std::make_shared<T[]>(m_capacity);
    for (size_type i = 0; i < m_size; ++i)
      newData[i] = m_data[i];

    m_data = newData;
  }
}

template <typename T> void vector<T>::add(T value) {
  maybe_resize();
  m_data[m_size++] = value;
}

template <typename T>
typename vector<T>::reference vector<T>::operator[](size_type index) {
  return m_data[index];
}

template <typename T> void vector<T>::insert(size_type index, T value) {
  maybe_resize();

  for (size_type i = m_size; i > index; --i)
    m_data[i] = m_data[i - 1];
  m_data[index] = value;

  m_size++;
}

template <typename T> void vector<T>::remove(size_type index) {
  for (size_type i = index; i < m_size; ++i)
    m_data[i] = m_data[i + 1];
  m_size--;
}

template <typename T> typename vector<T>::iterator vector<T>::begin() {
  vector<T>::iterator begin(m_data);
  return begin;
}

template <typename T> typename vector<T>::iterator vector<T>::end() {
  vector<T>::iterator end(m_size, m_data);
  return end;
}

template <typename T> void vector<T>::clear() { m_size = 0; }

template <typename T> typename vector<T>::size_type vector<T>::size() {
  return m_size;
}

template <typename T> typename vector<T>::size_type vector<T>::capacity() {
  return m_capacity;
}

} // namespace usu

#endif
