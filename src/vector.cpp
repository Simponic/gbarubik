#include "vector.hpp"

usu::vector<bool>::reference::reference()
    : usu::vector<bool>::reference(0, nullptr) {}

usu::vector<bool>::reference::reference(size_type pos, pointer data)
    : m_pos(pos), m_data(data) {}

typename usu::vector<bool>::reference &
usu::vector<bool>::reference::operator=(bool value) {
  std::uint8_t mask = 1 << (7 - (m_pos % 8));
  if (value)
    m_data[m_pos / 8] |= mask;
  else
    m_data[m_pos / 8] &= (~mask);
  return *this;
}

typename usu::vector<bool>::reference
usu::vector<bool>::operator[](size_type index) {
  reference r(index, m_data);
  return r;
}

void usu::vector<bool>::reference::add() { m_pos++; }

void usu::vector<bool>::reference::sub() { m_pos--; }

bool usu::vector<bool>::reference::get() const {
  return ((m_data[m_pos / 8] >> (7 - (m_pos % 8))) & 1) == 1;
}

usu::vector<bool>::reference::operator bool() { return get(); }

bool usu::vector<bool>::reference::operator==(const reference &rhs) const {
  return m_data == rhs.m_data && m_pos == rhs.m_pos;
}

bool usu::vector<bool>::reference::operator!=(const reference &rhs) const {
  return m_data != rhs.m_data || m_pos != rhs.m_pos;
}

usu::vector<bool>::iterator::iterator()
    : usu::vector<bool>::iterator(nullptr) {}

usu::vector<bool>::iterator::iterator(pointer data)
    : usu::vector<bool>::iterator(0, data) {}

usu::vector<bool>::iterator::iterator(size_type pos, pointer data) {
  m_ref = reference(pos, data);
}

usu::vector<bool>::iterator::iterator(const iterator &obj) {
  m_ref = obj.m_ref;
}

usu::vector<bool>::iterator::iterator(iterator &&obj) noexcept {
  m_ref = obj.m_ref;
  obj.m_ref = reference(0, nullptr);
}

typename usu::vector<bool>::iterator usu::vector<bool>::iterator::operator++() {
  m_ref.add();
  return *this;
}

typename usu::vector<bool>::iterator usu::vector<bool>::iterator::operator--() {
  m_ref.sub();
  return *this;
}

typename usu::vector<bool>::iterator
usu::vector<bool>::iterator::operator++(int) {
  iterator it = *this;
  m_ref.add();
  return it;
}

typename usu::vector<bool>::iterator
usu::vector<bool>::iterator::operator--(int) {
  iterator it = *this;
  m_ref.sub();
  return it;
}

typename usu::vector<bool>::reference usu::vector<bool>::iterator::operator*() {
  return m_ref;
}

bool usu::vector<bool>::iterator::operator!=(const iterator &rhs) const {
  return m_ref != rhs.m_ref;
}

bool usu::vector<bool>::iterator::operator==(const iterator &rhs) const {
  return m_ref == rhs.m_ref;
}

usu::vector<bool>::vector() : usu::vector<bool>(0, DEFAULT_RESIZER){};

usu::vector<bool>::vector(size_type size)
    : usu::vector<bool>(size, DEFAULT_RESIZER){};

usu::vector<bool>::vector(resize_type resize) : usu::vector<bool>(0, resize){};

usu::vector<bool>::vector(size_type size, resize_type resize) {
  m_resize = resize;
  m_size = size;
  m_capacity = DEFAULT_CAPACITY > size ? DEFAULT_CAPACITY : resize(size);
  maybe_resize();
};

usu::vector<bool>::vector(std::initializer_list<bool> list)
    : usu::vector<bool>(list, DEFAULT_RESIZER){};

usu::vector<bool>::vector(std::initializer_list<bool> list, resize_type resize)
    : usu::vector<std::uint8_t>(0, resize) {
  for (auto i = list.begin(); i != list.end(); ++i)
    add(*i);
};

void usu::vector<bool>::add(bool value) {
  maybe_resize();
  reference r(m_size++, m_data);
  r = value;
}

void usu::vector<bool>::insert(size_type index, bool value) {
  maybe_resize();

  reference r1(m_size, m_data);
  reference r2(m_size == 0 ? 0 : m_size - 1, m_data);

  for (size_type i = m_size; i > index; --i, r1.sub(), r2.sub())
    r1 = r2.get();
  r1 = value;
  m_size++;
}

void usu::vector<bool>::remove(size_type index) {
  reference r1(index, m_data);
  reference r2(index + 1 >= m_size ? index : index + 1, m_data);

  for (size_type i = index; i < m_size; ++i, r1.add(), r2.add())
    r1 = r2.get();
  r1 = false;
  m_size--;
}

void usu::vector<bool>::maybe_resize() {
  if (m_data == nullptr)
    m_data = std::make_shared<std::uint8_t[]>(m_capacity / 8);
  if (m_size >= m_capacity) {
    m_capacity = std::max(m_capacity == 0 ? 1 : m_resize(m_capacity), m_size);

    pointer newData = std::make_shared<std::uint8_t[]>(m_capacity / 8);
    for (size_type i = 0; i <= m_size / 8; ++i)
      newData[i] = m_data[i];

    m_data = newData;
  }
}

typename usu::vector<bool>::iterator usu::vector<bool>::begin() {
  vector<bool>::iterator begin(m_data);
  return begin;
}

typename usu::vector<bool>::iterator usu::vector<bool>::end() {
  vector<bool>::iterator end(m_size, m_data);
  return end;
}
