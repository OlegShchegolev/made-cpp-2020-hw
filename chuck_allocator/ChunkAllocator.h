template <typename T> struct chunk {
  T *data;
  size_t size;
  size_t space_left;
  chunk *next;
  chunk(size_t alloc_size) : size(alloc_size) {
    space_left = size;
    next = nullptr;
    data = reinterpret_cast<T *>(new char[size]);
  }
  ~chunk() { delete[] data; }
};

template <typename T> class ChunkAllocator {
  const int CHUNK_SIZE = 10;
  static size_t n;
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
    ++n;
    chunk_list = nullptr;
  }
  ChunkAllocator(const ChunkAllocator<T> &alloc) {
    if (this != &alloc) {
      ++n;
      chunk_list = alloc.chunk_list;
    }
  }
  ~ChunkAllocator() {
    chunk<T> *tmp;
    --n;
    if (n == 0) {
      while (chunk_list) {
        tmp = chunk_list->next;
        delete chunk_list;
        chunk_list = tmp;
      }
    }
  }
  ChunkAllocator &operator=(const ChunkAllocator &alloc) {
    chunk_list = alloc.chunk_list;
    return *this;
  }
  T *allocate(const size_t size) {
    if (!chunk_list) {
      size_t alloc_size = CHUNK_SIZE > size ? CHUNK_SIZE : size;
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
      size_t alloc_size = CHUNK_SIZE > size ? CHUNK_SIZE : size;
      chunk<T> *new_chunk = new chunk<T>(alloc_size);
      new_chunk->space_left = alloc_size - size;
      tmp = chunk_list;
      while (tmp->next)
        tmp = tmp->next;
      tmp->next = new_chunk;
      return new_chunk->data;
    }
  }
  void deallocate(T *p, size_t size) {}
  template <class... Args> void construct(T *p, Args &&... args) {
    new (p) T(args...);
  }
  void destroy(T *p) { p->~T(); }
};