#include <cstddef>

template <typename T> struct chunk {
  T *data;
  std::size_t size;
  std::size_t space_left;
  chunk *next;
  chunk(std::size_t alloc_size) : size(alloc_size) {
    space_left = size;
    next = nullptr;
    data = reinterpret_cast<T *>(new char[size]);
  }
  ~chunk() { delete[] data; }
};

template <typename T> class ChunkAllocator {
  const int CHUNK_SIZE = 10;
  std::size_t* n = nullptr;
  chunk<T> *chunk_list;

public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  template <class U> struct rebind { typedef ChunkAllocator<U> other; };

  ChunkAllocator() {
    n = new std::size_t(1);
    chunk_list = nullptr;
  }
  ChunkAllocator(const ChunkAllocator<T> &alloc) {
    if (this != &alloc) {
      n = alloc.n;
      ++(*n);
      chunk_list = alloc.chunk_list;
    }
  }
  ~ChunkAllocator() {
    chunk<T> *tmp;
    --(*n);
    if (*n == 0) {
      while (chunk_list) {
        tmp = chunk_list->next;
        delete chunk_list;
        chunk_list = tmp;
      }
      delete n;
    }
  }
  ChunkAllocator &operator=(const ChunkAllocator &alloc) {
    chunk_list = alloc.chunk_list;
    n = alloc.n;
    ++(*n);
    return *this;
  }
  T *allocate(const std::size_t size) {
    if (!chunk_list) {
      std::size_t alloc_size = CHUNK_SIZE > size ? CHUNK_SIZE : size;
      chunk_list = new chunk<T>(alloc_size);
      chunk_list->space_left = alloc_size - size;
      return chunk_list->data;
    } else {
      chunk<T> *tmp = chunk_list;
      while (tmp) {
        if (tmp->space_left >= size) {
          T *cur = tmp->data + tmp->size - tmp->space_left;
          tmp->space_left -= size;
          return cur;
        }
        tmp = tmp->next;
      }
      std::size_t alloc_size = CHUNK_SIZE > size ? CHUNK_SIZE : size;
      chunk<T> *new_chunk = new chunk<T>(alloc_size);
      new_chunk->space_left = alloc_size - size;
      tmp = chunk_list;
      while (tmp->next)
        tmp = tmp->next;
      tmp->next = new_chunk;
      return new_chunk->data;
    }
  }
  void deallocate(T *p, std::size_t size) {}
  template <class... Args> void construct(T *p, Args &&... args) {
    new (p) T(args...);
  }
  void destroy(T *p) { p->~T(); }
};