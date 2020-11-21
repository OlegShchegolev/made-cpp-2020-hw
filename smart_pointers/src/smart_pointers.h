#pragma once

namespace task {

template <class T> struct ControlBlock {
  T *ptr;
  long ref_count;
  long weak_ref_count;
};

template <class T> class WeakPtr;

template <class T> class UniquePtr {
  typedef T *pointer;
  pointer ptr;

public:
  explicit UniquePtr(pointer p) noexcept : ptr(p) {}
  UniquePtr(const UniquePtr &r) = delete;
  UniquePtr(UniquePtr &&u) noexcept {
    ptr = u.ptr;
    u.ptr = nullptr;
  };
  UniquePtr &operator=(const UniquePtr &r) = delete;
  UniquePtr &operator=(UniquePtr &&r) noexcept {
    if (ptr == r.ptr)
      return *this;
    delete ptr;
    ptr = r.ptr;
    r.ptr = nullptr;
    return *this;
  };
  ~UniquePtr() { delete ptr; };
  typename std::add_lvalue_reference<T>::type operator*() const {
    return *ptr;
  };
  pointer operator->() const noexcept { return ptr; };
  pointer get() const noexcept { return ptr; };
  pointer release() noexcept {
    pointer tmp = ptr;
    ptr = nullptr;
    return tmp;
  };
  void reset(pointer p = pointer()) noexcept {
    auto old_ptr = ptr;
    ptr = p;
    delete ptr;
  };
  void swap(UniquePtr &other) noexcept {
    auto tmp = ptr;
    ptr = other.ptr;
    other.ptr = tmp;
  };
};

template <class T> class SharedPtr {
  typedef std::remove_extent_t<T> element_type;
  typedef WeakPtr<T> weak_type;
  ControlBlock<T> *cb;
  void del() {
    if (cb) {
      --(cb->ref_count);
      if (cb->ref_count == 0) {
        delete cb->ptr;
        cb->ptr = nullptr;
        if (cb->weak_ref_count == 0) {
          delete cb;
          cb = nullptr;
        }
      }
    }
  }

public:
  explicit SharedPtr(T *ptr = nullptr) {
    cb = new ControlBlock<T>();
    cb->ptr = ptr;
    cb->ref_count = 1; // ptr ? 1 : 0;
    cb->weak_ref_count = 0;
  }
  SharedPtr(const SharedPtr<T> &r) noexcept {
    cb = r.cb;
    if (cb)
      ++(cb->ref_count);
    // if (cb -> ref_count == 1 && cb -> ptr == nullptr)
    //     ++ (cb -> ref_count)
  }
  SharedPtr(SharedPtr<T> &&r) noexcept {
    cb = r.cb;
    r.cb = nullptr;
  }
  ~SharedPtr() { del(); }
  SharedPtr &operator=(const SharedPtr<T> &r) noexcept {
    if (cb == r.cb)
      return *this;
    del();
    cb = r.cb;
    ++(cb->ref_count);
    return *this;
  }
  SharedPtr &operator=(SharedPtr<T> &&r) noexcept {
    del();
    cb = r.cb;
    r.cb = nullptr;
    return *this;
  }
  explicit SharedPtr(const WeakPtr<T> &r) {
    cb = r.cb;
    ++(cb->ref_count);
  }
  T *operator->() const noexcept { return cb->ptr; }
  T &operator*() const noexcept { return *(cb->ptr); }
  T *get() const noexcept { return cb ? cb->ptr : nullptr; }
  long use_count() const noexcept { return cb ? cb->ref_count : 0; }
  void reset(T *ptr = nullptr) {
    del();
    cb = new ControlBlock<T>();
    cb->ptr = ptr;
    cb->ref_count = ptr ? 1 : 0;
    cb->weak_ref_count = 0;
  }
  void swap(SharedPtr<T> &r) noexcept {
    auto tmp = cb;
    cb = r.cb;
    r.cb = tmp;
  }
  template <class U> friend class WeakPtr;
};

template <class T> class WeakPtr {
  typedef std::remove_extent_t<T> element_type;
  ControlBlock<element_type> *cb;
  void del_weak_ptr() {
    if (cb) {
      if (cb->ref_count == 0 && cb->weak_ref_count == 0) {
        delete cb;
        cb = nullptr;
      } else
        --cb->weak_ref_count;
    }
  }

public:
  constexpr WeakPtr() noexcept {
    cb = new ControlBlock<T>();
    cb->ref_count = 0;
    cb->weak_ref_count = 1;
  }
  WeakPtr(const SharedPtr<element_type> &r) noexcept {
    cb = r.cb;
    ++cb->weak_ref_count;
  }
  WeakPtr(const WeakPtr &r) noexcept {
    cb = r.cb;
    ++cb->weak_ref_count;
  }
  WeakPtr(WeakPtr &&r) noexcept {
    cb = r.cb;
    r.cb = nullptr;
  }
  ~WeakPtr() {
    if (cb) {
      --cb->weak_ref_count;
      if (cb->ref_count == 0 && cb->weak_ref_count == 0) {
        delete cb;
        cb = nullptr;
      }
    }
  }
  WeakPtr &operator=(const WeakPtr &r) noexcept {
    if (cb == r.cb)
      return *this;
    del_weak_ptr();
    cb = r.cb;
    ++cb->weak_ref_count;
  }
  WeakPtr &operator=(WeakPtr &&r) noexcept {
    del_weak_ptr();
    cb = r.cb;
    r.cb = nullptr;
    return *this;
  }
  WeakPtr &operator=(const SharedPtr<T> &r) noexcept {
    if (cb == r.cb)
      return *this;
    del_weak_ptr();
    cb = r.cb;
    ++(cb->weak_ref_count);
    return *this;
  }
  bool expired() const noexcept {
    if (cb->ref_count == 0)
      return true;
    return false;
  };
  long use_count() const noexcept { return cb ? cb->ref_count : 0; };
  SharedPtr<element_type> lock() const noexcept {
    if (!expired())
      return SharedPtr<element_type>(*this);
    return SharedPtr<element_type>();
  };
  void reset() noexcept {
    del_weak_ptr();
    cb = nullptr;
  };
  void swap(WeakPtr &r) noexcept {
    auto tmp = r.cb;
    r.cb = cb;
    cb = tmp;
  };
  template <class U> friend class SharedPtr;
};

} // namespace task

#include "smart_pointers.tpp"