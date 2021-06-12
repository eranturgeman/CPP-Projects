
#include <cmath>
#include <algorithm>
#include <iterator>

/**
 * DEFAULT_STATIC_CAPACITY default static capacity for the vector
 */
#define DEFAULT_STATIC_CAPACITY 16

/**
 * INDEX_ERROR_MSG error message text in case of index exception
 */
#define INDEX_ERROR_MSG "Index out of range!"

/**
 * INVALID_RANGE_ERROR_MSG error message text in case of out of range exception
 */
#define INVALID_RANGE_ERROR_MSG "Invalid range!"

/**
 * INITIAL_VEC_SIZE initial size for the vecotr
 */
#define INITIAL_VEC_SIZE 0

#ifndef EX6__VL_VECTOR_H_
#define EX6__VL_VECTOR_H_

template<typename T, size_t StaticCapacity = DEFAULT_STATIC_CAPACITY>
class VLVector {
  T static_arr_[StaticCapacity];
  T *dynamic_arr_;
  T *data_;
  size_t cur_size_;
  size_t cur_cap_;

  /**
   * push all elements from 'push_from' position by 'how_many' cells
   * @param push_from index to push from
   * @param how_many how many cells to push forward
   */
  void PushAllForward(size_t push_from, size_t how_many) noexcept;

  /**
   * expands the data array capacity by increasing its size (or creating new
   * dynamic array in case we used the static array so far) and copying all
   * elements from old array to new array + free old array if needed
   * @param to_add amount of elements to add
   * @param is_using_dynamic boolean variable saying if at the beginning of
   * the function we use the dynamic array
   */
  void ExpandDataArray(size_t to_add, bool is_using_dynamic);

  /**
   * by checking the size of the vector after the addition of 'to_add'
   * elements returns whether yes or not suppose to use the dynamic array
   * @param to_add amount of elements to add
   * @return true is size after addition > static capacity, false otherwise
   */
  inline bool ShouldUseDynamic(size_t to_add) const noexcept {
    return (cur_size_ + to_add) > StaticCapacity;
  }

  /**
   * checks if the array we are storing the data is the static array or the
   * dynamic array
   * @return true is we uses the dynamic array, false if we use the static array
   */
  inline bool IsUsingDynamic() const noexcept {
    return cur_cap_ >
        StaticCapacity;
  }

  /**
   * Calculates the capacity the vector should have after inserting
   * amount_to_add more elements
   * @param cur_size current vector size
   * @param amount_to_add number of elements to add
   * @return size vector should have after the addition
   */
  size_t CalculateCapacity(size_t cur_size, size_t amount_to_add) const
  noexcept {
    return (cur_size + amount_to_add <= StaticCapacity) ? StaticCapacity :
           std::floor((3 * (cur_size + amount_to_add)) / 2);
  }

 public:

  //_____________________________iterators___________________________________
  /**
   * iterator type for VLVector object
   */
  typedef T *iterator;
  /**
   * const iterator type for VLVector object
   */
  typedef const T *const_iterator;

  /**
   * returns an iterator to the vector's beginning
   * @return iterator (T*) to the first cell in the data array
   */
  iterator begin() noexcept { return data_; };

  /**
   * returns an iterator to the vector's end
   * @return iterator (T*) to one past the last cell in the data array
   */
  iterator end() noexcept { return data_ + cur_size_; };

  /**
   * returns a const iterator for a const vector's beginning
   * @return const_iterator (const T*) to the first cell in the data array
   */
  const_iterator begin() const noexcept { return data_; }

  /**
   * returns a const iterator for a const vector's end
   * @return const_iterator (const T*) to one past the last cell in the data array
   */
  const_iterator end() const noexcept { return data_ + cur_size_; }

  /**
   * returns a const iterator for non-const vector's beginning
   * @return const iterator (const T*) to the first cell in the data array
   */
  const_iterator cbegin() const noexcept { return data_; }

  /**
   * returns a const iterator for non-const vector's end
   * @return const iterator (const T*) to one past the last cell in the data array
   */
  const_iterator cend() const noexcept { return data_ + cur_size_; }

  /**
   * reverse iterator for VLVector
   */
  typedef std::reverse_iterator<iterator> reverse_iterator;
  /**
   * const reverse iterator for VLVector
   */
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  /**
   * returns reverse iterator object starting for the vector last element
   * @return reverse iterator starting from data_[cur_size_ - 1]
   */
  reverse_iterator rbegin() { return reverse_iterator(end()); }

  /**
   * returns reverse iterator object pointing one cell before the vector's
   * first cell
   * @return reverse iterator pointing to data_[-1]
   */
  reverse_iterator rend() { return reverse_iterator(begin()); }

  /**
   * returns const reverse iterator object starting for a const vector's last
   * element
   * @return const reverse iterator starting from data_[cur_size_ - 1]
   */
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end());}

  /**
   * returns const reverse iterator object pointing one cell before a
   * const vector's first cell
   * @return const reverse iterator pointing to data_[-1]
   */
  const_reverse_iterator rend() const { return const_reverse_iterator(begin());}

  /**
   * returns const reverse iterator object starting for a vector's last
   * element
   * @return const reverse iterator starting from data_[cur_size_ - 1]
   */
  const_reverse_iterator crbegin() const { return const_reverse_iterator(end());}

  /**
   * returns const reverse iterator object pointing one cell before a
   * const vector's first cell
   * @return const reverse iterator pointing to data_[-1]
   */
  const_reverse_iterator crend() const { return const_reverse_iterator(begin());}
  //_________________________constructors___________________________
  /**
   * Default constructor
   */
  VLVector() noexcept: dynamic_arr_(nullptr), data_(static_arr_), cur_size_
      (INITIAL_VEC_SIZE), cur_cap_(StaticCapacity) {}

  /**
   * Copy constructor- creates new vector with the same elements type and
   * static capacity as other and copying all elements from other to new vector
   * @param other vector to copy from
   */
  VLVector(const VLVector &other) noexcept(false): VLVector() { *this = other; }

  /**
   * Constructor initialize new vector with 'count' times the element 'elem'
   * @param count the amount that elem needs to be added to the vector
   * @param elem element of type T to add to the vector
   */
  VLVector(size_t count, const T &elem) noexcept(false);

  template<typename InputIterator>
  VLVector(InputIterator first, InputIterator last);

  /**
   * Destructor for the vector- freeing data array in case that vector using
   * the dynamic array to store data
   */
  ~VLVector() noexcept {
    if (this->IsUsingDynamic()) {
      delete[] data_;
    }
  }
  //_________________________operators___________________________
  /**
   * = operator. creates new vector which is the exact copy of other
   * @param other vector to copy
   * @return reference to nw vector
   */
  VLVector<T, StaticCapacity> &operator=(const VLVector &other) noexcept(false);

  /**
 * checks if the content of two vectors are equal: StaticCapacity, size,
 * elements
 * @param other VLVector object to compare to
 * @return true if equals, false otherwise
 */
  bool operator==(const VLVector &other) const noexcept;

  /**
   * checks if the content of two vectors are not equal: StaticCapacity, size,
   * elements
   * @param other VLVector object to compare to
   * @return true if not equals, false otherwise
   */
  bool operator!=(const VLVector &other) const noexcept {
    return !(*this == other);
  }

  /**
   * returns reference to the element in index 'index' in the vector
   * @return reference to the element in the requested index in the vector
   */
  T &operator[](size_t index) noexcept { return data_[index]; }

  /**
 * returns value of the element in index 'index' in the vector
 * @return value of the element in the requested index in the vector
 */
  T operator[](size_t index) const noexcept { return data_[index]; }

//_________________________functions___________________________
  /**
   * @return current amount of elements the vector holds
   */
  size_t Size() const noexcept { return cur_size_; }
  /**
   * @return maximum amount of elements the vector can hold in it's current
   * state
   */
  size_t Capacity() const noexcept { return cur_cap_; }

  /**
   * @return true is vector holds no elements or false otherwise
   */
  bool Empty() const noexcept { return (cur_size_ == 0); }

  /**
   * for non constant vector returns reference to the element in index
   * 'index' and if index is out of bound throws std::out_of_range exception
   * @param index index of the requested element
   * @return reference to the elements in index 'index'
   */
  T &At(size_t index) noexcept(false);

  /**
   * for const vector returns const reference (not changeable) to the element
   * in index 'index' and if index is out of bound throws std::out_of_range
   * @param index index of the requested element
   * @return const reference to the elements in the index 'index'
   */
  T At(size_t index) const noexcept(false);

  /**
   * adds new element at the end of vector, switch from static array to dynamic
   * array (or the other way around) in needed (may throw exception in
   * allocation)
   */
  void PushBack(const T &new_elem);

  /**
   * @return pointer to the array holds the data in the current state- the
   * dynamic array or the static array
   */
  T *Data() noexcept { return data_; }

  /**
 * @return const pointer to the array holds the data in the current state- the
 * dynamic array or the static array
 */
  const T *Data() const { return data_; }

  /**
   * clears all data from the vector and resets it: free dynamic array if in
   * use, set capacity back to Static Capacity, set size to 0
   */
  void Clear() noexcept;

  /**
   * insert single element of type T to the vector, left to the element that
   * position iterator points to.
   * CAUTION!: if used incorrectly leads to unexpected behaviour (read position)
   * @param position iterator pointing to a valid position in the vector
   * (valid: begin() <= position <= end() ) which symbolize the place the new
   * element should go to
   * @param to_add element to add
   * @return iterator stats at the new element inserted
   */
  iterator Insert(const_iterator position, const T &to_add) noexcept(false);

  /**
   * insert range of elements between first to last iterators, left to the
   * element that position iterator points to
   * CAUTION!: if used incorrectly leads to unexpected behaviour or crush.
   * VALIDITY: begin() <= position <= end() && [first,last) is valid range
   * where first <= last
   * @tparam InputIterator iterator pointing to the beginning of the range to
   * insert
   * @param position iterator pointing to one past the end of the range to
   * insert
   * @param first input_iterator starting the range to insert
   * @param last input_iterator ending the range to insert
   * @return iterator stats at the most left new element inserted
   */
  template<typename InputIterator>
  iterator Insert(const_iterator position, InputIterator first,
                  InputIterator last)
  noexcept(false);

  /**
   * removes the last element in the vector. if vector is empty- return
   * without doing anything
   */
  void PopBack() noexcept;

  /**
   * erasing element that to iterator position points to.
   * CAUTION!: if used incorrectly leads to unexpected behaviour and returns
   * end() or crushes
   * VALIDITY: begin()<= position < end()
   * EDGE CASE: if vector is empty- does nothing and returns end()
   * @param position iterator points the the element we want to remove
   * @return iterator pointing to the element on the right from the element
   * that got removed
   */
  iterator Erase(const_iterator position) noexcept;

  /**
   * erasing range of elements from the vector, starting at first and ends at
   * last.
   * CAUTION!: if used incorrectly leads to unexpected behaviour or crushes
   * VALIDITY: begin()<=first<=last<=end()
   * @param first first element in range of elements to remove
   * @param last one past the last elements in range of elements to remove
   * @return iterator to the element on the right of the section got removed
   */
  iterator Erase(const_iterator first, const_iterator last) noexcept;
};

//single value initialized constructor
template<typename T, size_t StaticCapacity>
VLVector<T, StaticCapacity>::VLVector(const size_t count, const T &elem)
noexcept(false):VLVector() {
  if (this->ShouldUseDynamic(count)) {
    cur_cap_ = CalculateCapacity(INITIAL_VEC_SIZE, count);
    dynamic_arr_ = new T[cur_cap_];
    data_ = dynamic_arr_;
  }
  cur_size_ = count;
  std::fill(this->begin(), this->end(), elem);
}

//iterator constructor
template<typename T, size_t StaticCapacity>
template<typename InputIterator>
VLVector<T, StaticCapacity>::VLVector(InputIterator first, InputIterator
last):VLVector() {
  size_t distance = std::distance(first, last);
  if (this->ShouldUseDynamic(distance)) {
    cur_cap_ = this->CalculateCapacity(0, distance);
    dynamic_arr_ = new T[cur_cap_];
    data_ = dynamic_arr_;
  }
  cur_size_ = distance;
  std::copy(first, last, data_);
}

template<typename T, size_t StaticCapacity>
bool VLVector<T, StaticCapacity>::operator==(const VLVector &other) const
noexcept {
  if (cur_size_ != other.cur_size_) {
    return false;
  }
  for (size_t i = 0; i < cur_size_; ++i) {
    if (data_[i] != other.data_[i]) {
      return false;
    }
  }
  return true;
}

template<typename T, size_t StaticCapacity>
VLVector<T, StaticCapacity> &VLVector<T, StaticCapacity>::operator=(const
    VLVector &other) noexcept(false) {
  if (this == &other) {
    return *this;
  }
  this->Clear();
  if (this->ShouldUseDynamic(other.cur_size_)) {
    size_t new_capacity = (*this).CalculateCapacity(cur_size_, other.cur_size_);
    dynamic_arr_ = new T[new_capacity];
    data_ = dynamic_arr_;
    cur_cap_ = new_capacity;
  }
  cur_size_ = other.cur_size_;

  std::copy(other.cbegin(), other.cend(), this->begin());
  return *this;
}

template<typename T, size_t StaticCapacity>
T &VLVector<T, StaticCapacity>::At(size_t index) noexcept(false) {
  if (index >= cur_size_) {
    throw std::out_of_range(INDEX_ERROR_MSG);
  }
  return data_[index];
}

template<typename T, size_t StaticCapacity>
T VLVector<T, StaticCapacity>::At(size_t index) const noexcept(false) {
  if (index >= cur_size_) {
    throw std::out_of_range(INDEX_ERROR_MSG);
  }
  return data_[index];
}

template<typename T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::PushBack(const T &new_elem) {
  if (cur_size_ == cur_cap_) {
    ExpandDataArray(1, IsUsingDynamic());
  }
  data_[cur_size_++] = new_elem;
}

template<typename T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::Clear() noexcept {
  if (cur_size_ == 0) {
    return;
  }
  if (this->IsUsingDynamic()) {
    delete[] dynamic_arr_;
    dynamic_arr_ = nullptr;
    data_ = static_arr_;
  }
  cur_size_ = 0;
  cur_cap_ = StaticCapacity;
}

template<typename T, size_t StaticCapacity>
typename VLVector<T, StaticCapacity>::iterator VLVector<T,StaticCapacity>::
    Insert(VLVector<T, StaticCapacity>::const_iterator position, const T
    &to_add) noexcept(false) {
  size_t index_to_push_from = position - cbegin();
  if (index_to_push_from > cur_size_ || position - cbegin() < 0) {
    return begin() + index_to_push_from;
    //TODO check cases. in std::vector crush or unexpected
  }
  if (index_to_push_from == cur_size_) {
    this->PushBack(to_add);
  } else {
    if (cur_size_ == cur_cap_) {
      ExpandDataArray(1, IsUsingDynamic());
    }
    PushAllForward(index_to_push_from, 1);
    data_[index_to_push_from] = to_add;
    cur_size_++;
  }
  return begin() + index_to_push_from;
}

template<typename T, size_t StaticCapacity>
template<typename InputIterator>
typename VLVector<T, StaticCapacity>::iterator VLVector<T, StaticCapacity>::
Insert(VLVector<T, StaticCapacity>::const_iterator position,
       InputIterator first, InputIterator last) noexcept(false) {
  size_t index_to_push_from = position - cbegin();
  if (index_to_push_from > cur_size_ || position - cbegin() < 0) {
    return begin() + index_to_push_from;
  }
  if (last - first < 0) {
    throw std::length_error(INVALID_RANGE_ERROR_MSG);
  }
  size_t elems_to_add = std::distance(first, last);
  if (cur_size_ + elems_to_add >= cur_cap_) {
    ExpandDataArray(elems_to_add, IsUsingDynamic());
  }
  PushAllForward(index_to_push_from, elems_to_add);
  size_t i = 0;
  for (auto it = first; it != last; ++it) {
    data_[index_to_push_from + i] = *it;
    i++;
  }
  cur_size_ += elems_to_add;
  return begin() + index_to_push_from;
}

template<typename T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::PopBack() noexcept {
  if (cur_size_ == 0) {
    return;
  }
  //dynamic->static
  if (IsUsingDynamic() && cur_size_ - 1 == StaticCapacity) {
    std::copy(cbegin(), cend() - 1, static_arr_);
    delete[] dynamic_arr_;
    dynamic_arr_ = nullptr;
    data_ = static_arr_;
    cur_cap_ = StaticCapacity;
  }
  //static->static and dynamic->dynamic
  cur_size_--;
}

template<typename T, size_t StaticCapacity>
typename VLVector<T, StaticCapacity>::iterator VLVector<T,
                                                        StaticCapacity>::Erase(
    const_iterator position) noexcept {
  if (position - cend() >= 0 || cur_size_ == 0) {
    return end();
  }
  long index_to_remove = position - cbegin();
  if (index_to_remove < 0) {
    index_to_remove = 0;
    position = cbegin();
  }
  size_t index = index_to_remove;
  for (auto it = position + 1; it != cend(); ++it) {
    data_[index++] = *it;
  }
  this->PopBack();
  return begin() + index_to_remove;
}

template<typename T, size_t StaticCapacity>
typename VLVector<T, StaticCapacity>::iterator VLVector<T,
                                                        StaticCapacity>::Erase(
    const_iterator first, const_iterator last) noexcept {

  if (last - cend() > 0 || first - last > 0) {
    return end();
  }
  size_t index_to_erase_from = first - cbegin();
  if (first == last) {
    return begin() + index_to_erase_from;
  }
  size_t amount_to_remove = last - first;

  iterator it = begin() + index_to_erase_from;
  for (; last != cend(); ++last, ++it) {
    *it = *last;
  }
  while (amount_to_remove != 0) {
    PopBack();
    amount_to_remove--;
  }
  return begin() + (first - cbegin());
}

//__________________________private functions________________________________
template<typename T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::ExpandDataArray(size_t to_add, bool
is_using_dynamic) {
  size_t tmp_cap = CalculateCapacity(cur_size_, to_add);
  T *tmp = new T[tmp_cap];
  std::copy(this->cbegin(), this->cend(), tmp);
  if (is_using_dynamic) {
    delete[] data_;
  }
  cur_cap_ = tmp_cap;
  dynamic_arr_ = tmp;
  data_ = dynamic_arr_;
}

template<typename T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::PushAllForward(size_t push_from, size_t
how_many) noexcept {
  if (push_from == cur_size_) {
    return;
  }
  size_t i = 0;
  for (auto r_it = rbegin(); r_it != rend() - push_from; ++r_it) {
    data_[cur_size_ - 1 - i + how_many] = *r_it;
    i++;
  }
}
#endif //EX6__VL_VECTOR_H_
