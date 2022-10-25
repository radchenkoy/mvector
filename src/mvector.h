/*
 * mvector.h
 *
 *  Created on: Oct 18, 2022
 *      Author: yradchenko
 */

#ifndef MVECTOR_H_
#define MVECTOR_H_

#include <cstddef>
#include <cstring>
#include <new>
#include <initializer_list>
#include <ostream>



#define M_DEFAULT_ALLOC_PAGE 16

// allocator
struct Mallocator
{
  static void* alloc(size_t n)
  {
      try
      {
          return new char[n];
      }
      catch (const std::bad_alloc&)
      {
          //TODO: Log here
          throw;
      }
  }
  static void  free(void* p)
  {
      delete [] static_cast<char*>(p);
  }
};



template<bool>
struct __m_uninitialized_ops
{
  template<typename T>
  static void construct(T* p)
  {
      new (static_cast<void*>(p)) T();
  }

  template<typename T>
  static void construct(T* p, const T& v)
  {
      new (static_cast<void*>(p)) T(v);
  }

  template<typename T>
  static void destruct(T* p){ p->~T(); }

  template<typename T>
  static void destruct_n(T* first, size_t n)
  {
    while (n--)
      destruct(first++);
  }

  template<typename T>
  static void fill_n(T* first, size_t n, const T& value)
  {
    T* tmp = first;
    try {
      while (n--){
        construct(first, value);
        ++first;
      }
    }
    catch(...){
      destruct_n(tmp, first - tmp);
      throw;
    }
  }

  template <typename T>
  static void copy_n(T* dest, T* first, size_t n)
  {
    T* tmp = dest;
    try {
      while (n--){
        construct(dest, *first++);
        ++dest;
      }
    }
    catch(...){
      destruct_n(tmp, dest - tmp);
      throw;
    }
  }

  template <typename T>
  static void move_n(T* dest, T* first, size_t n){
    if (dest < first)
    {
      while (n--)
      {
        construct(dest++, *first);
        destruct(first++);
      }
    }
    else
    {
      dest += n;
      first += n;

      while (n--)
      {
        construct(--dest, *(--first));
        destruct(first);
      }
    }
  }

  template <typename T>
  static void move_n_nov(T* dest, T* first, size_t n){
    while (n--){
      construct(dest++, *first);
      destruct(first++);
    }
  }

};

template<>
struct __m_uninitialized_ops<true>
{
  template<typename T>
  static void construct(T* p){ *p = 0; }

  template<typename T>
  static void construct(T* p, const T v){ *p = v; }

  template<typename T>
  static void destruct(T*){}

  template<typename T>
  static void destruct_n(T*, size_t){}

  template<typename T>
  static void fill_n(T* first, size_t n, const T value)
  {
    while (n--)
      *first++ = value;
  }

  template <typename T>
  static void copy_n(T* dest, T* first, size_t n)
  {
    memcpy(dest, first, sizeof(T) * n);
  }

  template <typename T>
  static void move_n(T* dest, T* first, size_t n){
    memmove(dest, first, sizeof(T) * n);
  }

  template <typename T>
  static void move_n_nov(T* dest, T* first, size_t n)
  {
    memcpy(dest, first, sizeof(T) * n);
  }
};

template<typename T>
struct M_Type_Traits {
  enum {
    has_trivial_ctor    = 0,
    has_trivial_copy    = 0,
    has_trivial_assign  = 0,
    has_trivial_dtor    = 0
  };
};

// traits for vector element
template<typename T>
struct Mtraits
{
  typedef T         value_type;
  typedef T&        reference;
  typedef const T&  const_reference;
  typedef const T&  param_type;
  typedef const T&  retval_type;
};


struct M_FND_Type_Traits {
  enum {
    has_trivial_ctor    = 1,
    has_trivial_copy    = 1,
    has_trivial_assign  = 1,
    has_trivial_dtor    = 1
  };
};

template<typename T>
struct M_FND_traits
{
  typedef T         value_type;
  typedef T&        reference;
  typedef const T&  const_reference;
  typedef T const   param_type;
  typedef T const   retval_type;
};

#define FTYPE_TRAITS_SPECIALIZE(T)                                              \
        template<> struct M_Type_Traits<T>          : M_FND_Type_Traits     {}; \
        template<> struct M_Type_Traits<T*>         : M_FND_Type_Traits     {}; \
        template<> struct M_Type_Traits<const T*>   : M_FND_Type_Traits     {}; \
        template<> struct M_Type_Traits<T**>        : M_FND_Type_Traits     {}; \
        template<> struct M_Type_Traits<T* const *> : M_FND_Type_Traits     {}; \
        template<> struct M_Type_Traits<const T**>  : M_FND_Type_Traits     {}; \
        template<> struct Mtraits<T>                : M_FND_traits<T>       {}; \
        template<> struct Mtraits<T*>               : M_FND_traits<T*>      {}; \
        template<> struct Mtraits<T**>              : M_FND_traits<T**>     {}

FTYPE_TRAITS_SPECIALIZE(char);
FTYPE_TRAITS_SPECIALIZE(unsigned char);
FTYPE_TRAITS_SPECIALIZE(short);
FTYPE_TRAITS_SPECIALIZE(unsigned short);
FTYPE_TRAITS_SPECIALIZE(int);
FTYPE_TRAITS_SPECIALIZE(unsigned int);
FTYPE_TRAITS_SPECIALIZE(long);
FTYPE_TRAITS_SPECIALIZE(unsigned long);
FTYPE_TRAITS_SPECIALIZE(float);
FTYPE_TRAITS_SPECIALIZE(double);


template<typename T>
inline void __m_construct(T* p, typename Mtraits<T>::param_type v){
  //TODO: assert p
  __m_uninitialized_ops<M_Type_Traits<T>::has_trivial_copy>::construct(p, v);
}

template <typename T>
inline void __m_uninitialized_fill_n(T* first, size_t n, typename Mtraits<T>::param_type value)
{
  //TODO: asserts for first and n
  __m_uninitialized_ops<M_Type_Traits<T>::has_trivial_copy>::fill_n(first, n, value);
}

template <typename T>
inline void __m_uninitialized_fill(T* first, T* last, typename Mtraits<T>::param_type value)
{
  //TODO: assert (first <= last);
  __m_uninitialized_fill_n(first, last - first, value);
}

template <typename T>
inline void __m_uninitialized_move_n(T* dest, T* first, size_t n)
{
  //TODO: assert dest != first
  __m_uninitialized_ops<M_Type_Traits<T>::has_trivial_copy>::move_n(dest, first, n);
}

template <typename T>
inline void __m_uninitialized_move(T* dest, T* first, T* last)
{
  //TODO: assert(first <= last);
  __m_uninitialized_move_n(dest, first, last - first);
}

template <typename T>
inline void __m_uninitialized_copy_n(T* dest, T* first, size_t n)
{
  //TODO: asserts (dest + n <= first || first + n <= dest);
  __m_uninitialized_ops<M_Type_Traits<T>::has_trivial_copy>::copy_n(dest, first, n);
}

template<typename T>
inline void __m_destruct(T* p)
{
  //TODO: assert(p);
  __m_uninitialized_ops<M_Type_Traits<T>::has_trivial_dtor>::destruct(p);
}

template<typename T>
inline void __m_destruct_n(T* first, size_t n)
{
  //TODO: assert(first || !n);
  __m_uninitialized_ops<M_Type_Traits<T>::has_trivial_dtor>::destruct_n(first, n);
}

template<typename T>
inline void __m_destruct(T* first, T* last)
{
  //TODO: assert(first <= last);
  __m_destruct_n(first, last - first);
}

template <typename T>
inline void __m_uninitialized_move_n_nov(T* dest, T* first, size_t n)
{
  //TODO: assert(dest + n <= first || first + n <= dest);
  __m_uninitialized_ops<M_Type_Traits<T>::has_trivial_copy>::move_n_nov(dest, first, n);
}


//vector
template<typename T, class __A = Mallocator>
class Mvector
{
public:

    typedef typename Mtraits<T>::value_type      value_type;
    typedef typename Mtraits<T>::reference       reference;
    typedef typename Mtraits<T>::const_reference const_reference;
    typedef typename Mtraits<T>::param_type      param_type;
    typedef typename Mtraits<T>::retval_type     retval_type;

    Mvector()
      : buffer_(0),
        capacity_(0),
        size_(0),
        alloc_page_(M_DEFAULT_ALLOC_PAGE)
    {}

    explicit Mvector(size_t n, param_type value = T())
      : buffer_(static_cast<T*>(__A::alloc(n * sizeof(T)))),
        capacity_(n),
        size_(n),
        alloc_page_(M_DEFAULT_ALLOC_PAGE)
    {
      __m_uninitialized_fill_n(buffer_, n, value);
    }

    Mvector(const Mvector& src)
      : buffer_(static_cast<T*>(__A::alloc(src.size_ * sizeof(T)))),
        capacity_(src.size_),
        size_(src.size_),
        alloc_page_(M_DEFAULT_ALLOC_PAGE)
    {
      __m_uninitialized_copy_n(buffer_, src.buffer_, size_);
    }

    Mvector (const std::initializer_list<T> &list)
    : Mvector(list.size())
    {
        int count = 0;
        for (auto &element : list)
        {
            buffer_[count] = element;
            ++count;
        }
    }

    Mvector& operator= (const Mvector& src){
      if (this != &src){
        __m_destruct_n(buffer_, size_);

        if (src.size() > capacity_){
          __A::free(buffer_);
          capacity_ = src.size();
          buffer_ = static_cast<T*>(__A::alloc(capacity_ * sizeof(T)));
        }

        size_ = src.size_;
        __m_uninitialized_copy_n(buffer_, src.buffer_, size_);
      }
      return *this;
    }

    ~Mvector() {
      __m_destruct_n(buffer_, size_);
      __A::free(buffer_);
    }

    //Element access
    reference   operator[](size_t idx)       { return *(buffer_ + idx); } //TODO: check bounder
    retval_type operator[](size_t idx) const { return *(buffer_ + idx); }

    template <typename K>
    class Iterator
    {
        K* cur;
    public:
        Iterator (K* cur_) : cur{cur_} {}

        K* operator+ (int n) { return (cur + n); }
        K* operator- (int n) { return (cur - n); }

        K& operator++ (int) { return *cur++; }
        K& operator-- (int) { return *cur--; }
        K& operator++ ()    { return *++cur; }
        K& operator-- ()    { return *--cur; }

        bool operator!= (const Iterator& it) {return cur != it.cur; }
        bool operator== (const Iterator& it) {return cur == it.cur; }
        K& operator* () { return *cur; }
        K* get_data()   { return cur; }

    };

    typedef Iterator<value_type> iterator;
    typedef Iterator<const value_type> const_iterator;

    iterator begin()              { return iterator(buffer_); }
    iterator end()                { return iterator(buffer_ + size_); }
    const_iterator cbegin() const { return const_iterator(buffer_); }
    const_iterator cend()   const { return const_iterator(buffer_ + size_);}

    //Capacity
    size_t size()     const { return size_; }
    size_t capacity() const { return capacity_; }
    bool   empty()    const { return !size_; }

    void reserve(size_t new_capacity)
    {
      if (new_capacity > capacity_)
      {
        capacity_ = new_capacity;
        change_capacity_ge_sz();
      }
    }

    void shrink_to_fit()
    {
        if (size_ < capacity_)
        {
          capacity_ = size_;
          change_capacity_ge_sz();
        }
    }

    //Modifiers
    void clear()
    {
        __m_destruct(buffer_, buffer_ + size_);
      size_ = 0;
    }

    void resize(size_t new_size, param_type def_value = T())
    {
      if (new_size > size_)
      {
        reserve(new_size);
        __m_uninitialized_fill(buffer_ + size_, buffer_ + new_size, def_value);
      }
      else if (new_size < size_)
        __m_destruct(buffer_ + new_size, buffer_ + size_);

      size_ = new_size;
    }

    void emplace(size_t idx, param_type v)
    {
      if ( size_ == capacity_)
      {
        capacity_ += alloc_page_;
        alloc_page_ *= 2;
        change_capacity_ge_sz();
      }

      //idx -=1;

      __m_uninitialized_move_n(buffer_ + idx+1, buffer_ + idx, size_ - idx);


      __m_construct(buffer_ + idx, v);
      ++size_;
    }


    void emplace(iterator pos, param_type v)
    {

    	int idx = pos.get_data() - buffer_;

      if ( size_ == capacity_)
      {
        capacity_ += alloc_page_;
        alloc_page_ *= 2;
        change_capacity_ge_sz();
      }

      //T* p = pos.get_data();

     // __m_uninitialized_move_n(p+1, p, size_ - (p-buffer_) );

      __m_uninitialized_move_n(buffer_ + idx+1, buffer_ + idx, size_ - idx);


      __m_construct(buffer_ + idx, v);
      ++size_;
    }



    iterator erase( iterator pos )
    {
    	T* p = pos.get_data();
    	if ( p >= buffer_ && p < (buffer_ + size_) )
    	{
            __m_destruct(p);
            __m_uninitialized_move(p, p + 1, buffer_ + size_);
            --size_;
            return iterator(p);
    	}
    	else
    		return iterator(buffer_ + size_);
    }



    void push_bask(param_type v)
    {
      if ( size_ == capacity_)
      {
        capacity_ += alloc_page_;
        alloc_page_ *= 2;
        change_capacity_ge_sz();
      }
      __m_construct(buffer_ + size_, v);
      ++size_;
    }


private:
    void change_capacity_ge_sz()
    {
      T* p = static_cast<T*>(__A::alloc(capacity_ * sizeof(T)));
      try
      {
        __m_uninitialized_move_n_nov(p, buffer_, size_);
      }
      catch(...)
      {
        __A::free(p);
        throw;
      }
      __A::free(buffer_);
      buffer_ = p;
    }

protected:
  T*      buffer_;
  size_t  capacity_;
  size_t  size_;
  size_t  alloc_page_;
};


#endif /* MVECTOR_H_ */
