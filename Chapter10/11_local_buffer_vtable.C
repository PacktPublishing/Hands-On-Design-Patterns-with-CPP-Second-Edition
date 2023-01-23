#include <stdlib.h>
#include <string.h>
#include <new>
#include <utility>
#include <functional>

#include "benchmark/benchmark.h"

#define REQUIRES(...) std::enable_if_t<__VA_ARGS__, int> = 0

template<size_t S, size_t A = alignof(void*), bool nothrow_movable = false>
struct Buffer {
    constexpr static auto size = S, alignment = A;

    alignas(alignment) char space_[size];

    template <typename T> static constexpr bool valid_type() {
        return sizeof(T) <= S && (A % alignof(T)) == 0;
    }

    template <typename T
        //, REQUIRES(valid_type<T>())
        >
        requires(valid_type<T>())
    T* as() noexcept { return reinterpret_cast<T*>(&space_); }
    template <typename T
        //, REQUIRES(valid_type<T>())
        > 
        requires(valid_type<T>())
    const T* as() const noexcept { return const_cast<Buffer*>(this)->as<T>(); }

    Buffer() = default;

    template <typename T, typename... Args
        //, REQUIRES(valid_type<T>() && std::is_constructible_v<T, Args...>)
        >
        requires(valid_type<T>() && std::constructible_from<T, Args...>)
    Buffer(std::in_place_type_t<T>, Args&& ...args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
        : vtable_(&vtable<Buffer, T>)
    {
        ::new (static_cast<void*>(as<T>())) T(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args
        //, REQUIRES(valid_type<T>() && std::is_constructible_v<T, Args...>)
        >
        requires(valid_type<T>() && std::constructible_from<T, Args...>)
    T* emplace(Args&& ...args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
    {
        if (this->vtable_) this->vtable_->deleter_(this);
        this->vtable_ = &vtable<Buffer, T>;
        return ::new (static_cast<void*>(as<T>())) T(std::forward<Args>(args)...);
    }

    Buffer(const Buffer& that) { if (that.vtable_) that.vtable_->copy_construct_(this, &that); }
    Buffer(Buffer&& that) noexcept(nothrow_movable) { if (that.vtable_) that.vtable_->move_construct_(this, &that); }
    Buffer& operator=(const Buffer& that) {
        if (this->vtable_) this->vtable_->deleter_(this);
        if (that.vtable_) that.vtable_->copy_construct_(this, &that);
        else this->vtable_ = nullptr;
        return *this;
    }
    Buffer& operator=(Buffer&& that) noexcept(nothrow_movable) {
        if (this->vtable_) this->vtable_->deleter_(this);
        if (that.vtable_) that.vtable_->move_construct_(this, &that);
        else this->vtable_ = nullptr;
        return *this;
    }

    ~Buffer() noexcept { if (this->vtable_) this->vtable_->deleter_(this); }

    void swap(Buffer& that) noexcept(nothrow_movable) {
        Buffer tmp(std::move(that));
        that = std::move(*this);
        *this = std::move(tmp);
    }

    // Destroy the object stored in the aligned space.
    void destroy() noexcept {
        if (this->vtable_) this->vtable_->deleter_(this);
        this->vtable_ = nullptr;
    }

    // Type erasure magic is here:
    // vtable_t is the virtual table containing pointers to the static functions
    // for copying, moving, and destroying the type of the object currently
    // stored in the space.
    struct vtable_t { 
        using deleter_t = void(Buffer*);
        using copy_construct_t = void(Buffer*, const Buffer*);
        using move_construct_t = void(Buffer*, Buffer*);

        deleter_t*  deleter_;
        copy_construct_t* copy_construct_;
        move_construct_t* move_construct_;
    };
    const vtable_t* vtable_ = nullptr;
    template <typename U, typename T> constexpr static vtable_t vtable = {
        U::template deleter<T>,
        U::template copy_construct<T>,
        U::template move_construct<T>
    };

    template <typename T
        //, REQUIRES(valid_type<T>() && std::is_nothrow_destructible_v<T>)
        >
        requires(valid_type<T>() && std::is_nothrow_destructible_v<T>)
        static void deleter(Buffer* space) {
        space->as<T>()->~T();
    }

    template <typename T
        //, REQUIRES(valid_type<T>())
        >
        requires(valid_type<T>())
        static void copy_construct(Buffer* to, const Buffer* from) 
        noexcept(std::is_nothrow_copy_constructible_v<T>)
    {
        ::new (static_cast<void*>(to->as<T>())) T(*from->as<T>());
        to->vtable_ = from->vtable_;
    }

    template <typename T
        //, REQUIRES(valid_type<T>())
        >
        requires(valid_type<T>())
        static void move_construct(Buffer* to, Buffer* from)
        noexcept(std::is_nothrow_move_constructible_v<T>)
    {
        ::new (static_cast<void*>(to->as<T>())) T(std::move(*from->as<T>()));
        to->vtable_ = from->vtable_;
    }
}; // Buffer
template<size_t S, size_t A, bool nothrow_movable> template <typename U, typename T> constexpr typename Buffer<S, A, nothrow_movable>::vtable_t Buffer<S, A, nothrow_movable>::vtable;

template<typename Signature, size_t Size = sizeof(void*)*2, size_t Alignment = alignof(void*)> class Function;

template<size_t Size, size_t Alignment, typename Res, typename... Args>
class Function<Res(Args...), Size, Alignment>
{
    Buffer<Size, Alignment> space_;
    using executor_t = Res(*)(Args..., void*);
    static constexpr Res default_executor(Args..., void*) { throw std::bad_function_call(); }
    constexpr static executor_t default_executor_ = default_executor;
    executor_t executor_ = default_executor_;

    template<typename Callable> static Res executor(Args... args, void* this_function) {
        static_assert(!std::is_member_function_pointer_v<Callable>, "");
        return (*reinterpret_cast<Callable*>(&static_cast<Function*>(this_function)->space_))(std::forward<Args>(args)...);
    }

public:
    // Default-constructed function is null (invocation will throw).
    Function() = default;

    // Construct a function from a callable.
    // The callable must be trivially copyable and trivially destructible.
    // The callable must not exceed the size and alignment of this function.
    template <typename CallableArg, typename Callable = std::decay_t<CallableArg> 
        //, REQUIRES(!std::is_same_v<Function, Callable>)
        >
        requires(!std::same_as<Function, Callable>)
    Function(CallableArg&& callable) : 
        space_(std::in_place_type<Callable>, std::forward<CallableArg>(callable)),
        executor_(executor<Callable>)
    {
        static_assert(sizeof(Callable) <= Size, "Callable size is too large, increase size of the space type");
        static_assert(alignof(Callable) <= Alignment, "Callable alignment is too large, increase alignment of the space type");
        static_assert(std::is_trivially_destructible_v<Callable>, "Use Function for non-trivially-destructible callables");
        static_assert(std::is_trivially_copyable_v<Callable>, "Use Function for non-trivially-copyable callables");
        ::new (static_cast<void*>(&space_)) Callable(std::forward<CallableArg>(callable));
    }

    // Copy operations are default, copying is handled by AlignedStorage.
    Function(const Function&) = default;

    // The call operator invokes the callable.
    template <typename... CallArgs>
    Res operator()(CallArgs&&... callargs) const {
        return this->executor_(std::forward<CallArgs>(callargs)..., const_cast<Function*>(this));
    }

}; // Function
template<size_t Size, size_t Alignment, typename Res, typename... Args>
constexpr typename Function<Res(Args...), Size, Alignment>::executor_t Function<Res(Args...), Size, Alignment>::default_executor_;

using F = int(int, int, int, int);

using FF = Function<F>;

void BM_lambda_call(benchmark::State& state) {
    int a = rand(), b = rand(), c = rand(), d = rand();
    int x = rand();
    FF f{ [&](int a, int b, int c, int d) { return x + a + b + c + d; } };
    for (auto _ : state) {
        benchmark::DoNotOptimize(f(a, b, c, d));
        benchmark::ClobberMemory();
    }
}

void BM_lambda_copy(benchmark::State& state) {
    int a = rand(), b = rand(), c = rand(), d = rand();
    int x = rand();
    FF f0{ [&](int a, int b, int c, int d) { return x + a + b + c + d; } };
    for (auto _ : state) {
        FF f(f0); 
        benchmark::DoNotOptimize(f);
        benchmark::DoNotOptimize(f(a, b, c, d));
        benchmark::ClobberMemory();
    }
}

//BENCHMARK(BM_lambda_call);
BENCHMARK(BM_lambda_copy);

BENCHMARK_MAIN();
