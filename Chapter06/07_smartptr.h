#include <type_traits>
#include <new>
#include <memory>

template <typename T, typename Deleter>
class smartptr {
  public:
  smartptr(T* p, Deleter d) : p_(p), d_(d) {}
  ~smartptr() { d_(p_); }
  T* operator->() { return p_; }
  const T* operator->() const { return p_; }
  private:
  T* p_;
  Deleter d_;
};

template <typename T>
class smartptr_te {
  struct destroy_base {
    virtual void operator()(void*) = 0;
    virtual ~destroy_base() {}
  };
  template <typename Deleter> struct destroy : public destroy_base {
    destroy(Deleter d) : d_(d) {}
    void operator()(void* p) override { d_(static_cast<T*>(p)); }
    Deleter d_;
  };
  public:
  template <typename Deleter> smartptr_te(T* p, Deleter d) : p_(p), d_(new destroy<Deleter>(d)) {}
  ~smartptr_te() { (*d_)(p_); delete d_; }
  T* operator->() { return p_; }
  const T* operator->() const { return p_; }
  private:
  T* p_;
  destroy_base* d_;
};

template <typename T>
class smartptr_te_lb {
  struct destroy_base {
    virtual void operator()(void*) = 0;
    virtual ~destroy_base() {}
  };
  template <typename Deleter> struct destroy : public destroy_base {
    destroy(Deleter d) : d_(d) {}
    void operator()(void* p) override { d_(static_cast<T*>(p)); }
    Deleter d_;
  };
  public:
  template <typename Deleter> smartptr_te_lb(T* p, Deleter d) :
    p_(p),
    d_(sizeof(Deleter) > sizeof(buf_) ? new destroy<Deleter>(d) : new (buf_) destroy<Deleter>(d)) 
  {}
  ~smartptr_te_lb() {
    (*d_)(p_);
    if (static_cast<void*>(d_) == static_cast<void*>(buf_)) {
      d_->~destroy_base();
    } else {
      delete d_;
    }
  }
  T* operator->() { return p_; }
  const T* operator->() const { return p_; }
  private:
  T* p_;
  destroy_base* d_;
  alignas(8) char buf_[16];
};

template <typename T>
class smartptr_te_lb0 {
  struct destroy_base {
    virtual void operator()(void*) = 0;
    virtual ~destroy_base() {}
  };
  template <typename Deleter> struct destroy : public destroy_base {
    destroy(Deleter d) : d_(d) {}
    void operator()(void* p) override { d_(static_cast<T*>(p)); }
    Deleter d_;
  };
  public:
  template <typename Deleter> smartptr_te_lb0(T* p, Deleter d) : p_(p) {
    static_assert(sizeof(Deleter) <= sizeof(buf_), "");
    ::new (static_cast<void*>(buf_)) destroy<Deleter>(d);
  }
  ~smartptr_te_lb0() {
    destroy_base* d = reinterpret_cast<destroy_base*>(buf_);
    (*d)(p_);
    d->~destroy_base();
  }
  T* operator->() { return p_; }
  const T* operator->() const { return p_; }
  private:
  T* p_;
  alignas(8) char buf_[16];
};

template <typename T>
class smartptr_te_static {
  T* p_;
  using destroy_t = void(*)(T*, void*);
  destroy_t destroy_;
  alignas(8) char buf_[8];
  template<typename Deleter> static void invoke_destroy(T* p, void* d) {
    (*static_cast<Deleter*>(d))(p);
  }
  public:
  template <typename Deleter> smartptr_te_static(T* p, Deleter d)
    : p_(p), destroy_(invoke_destroy<Deleter>)
  {
    static_assert(sizeof(Deleter) <= sizeof(buf_));
    static_assert(std::is_trivially_destructible_v<Deleter>);
    ::new (static_cast<void*>(buf_)) Deleter(d);
  }
  ~smartptr_te_static() {
    this->destroy_(p_, buf_);
  }
  T* operator->() { return p_; }
  const T* operator->() const { return p_; }
};

template <typename T>
class smartptr_te_vtable {
  T* p_;
  struct vtable_t {
    using destroy_t = void(*)(T*, void*);
    using destructor_t = void(*)(void*);
    destroy_t destroy_;
    destructor_t destructor_;
  };
  const vtable_t* vtable_ = nullptr;
  template <typename Deleter> static void destroy(T* p, void* d) {
    (*static_cast<Deleter*>(d))(p);
  }
  template <typename Deleter> static void destructor(void* d) {
    static_cast<Deleter*>(d)->~Deleter();
  }

  template <typename Deleter> constexpr static vtable_t vtable = {
    smartptr_te_vtable::template destroy<Deleter>,
    smartptr_te_vtable::template destructor<Deleter>
  };

  alignas(8) char buf_[8];

  public:
  template <typename Deleter> smartptr_te_vtable(T* p, Deleter d)
    : p_(p), vtable_(&vtable<Deleter>)
  {
    static_assert(sizeof(Deleter) <= sizeof(buf_));
    ::new (static_cast<void*>(buf_)) Deleter(d);
  }
  ~smartptr_te_vtable() {
    this->vtable_->destroy_(p_, buf_);
    this->vtable_->destructor_(buf_);
  }
  T* operator->() { return p_; }
  const T* operator->() const { return p_; }
};

struct deleter {
  template <typename T> void operator()(T* p) { delete p; }
};
template <typename T>
struct deleter1 {
  void operator()(T* p) { delete p; }
};

static deleter d;

int* get_raw_ptr();
std::unique_ptr<int, deleter> get_unique_ptr();
std::shared_ptr<int> get_shared_ptr();
std::shared_ptr<int> make_shared_ptr();
smartptr<int, deleter> get_smartptr();
smartptr_te<int> get_smartptr_te();
smartptr_te_lb<int> get_smartptr_te_lb();
smartptr_te_lb0<int> get_smartptr_te_lb0();
smartptr_te_static<int> get_smartptr_te_static();
smartptr_te_vtable<int> get_smartptr_te_vtable();
