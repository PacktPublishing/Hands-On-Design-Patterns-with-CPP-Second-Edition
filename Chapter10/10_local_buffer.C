#include <stdlib.h>
#include <string.h>
#include <new>
#include <utility>
#include <functional>

#include "benchmark/benchmark.h"

#define REQUIRES(...) std::enable_if_t<__VA_ARGS__, int> = 0

template<size_t S, size_t A = alignof(void*)>
struct BufferTrivial {
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
    const T* as() const noexcept { return const_cast<BufferTrivial*>(this)->as<T>(); }

    BufferTrivial() = default;

    template <typename T, typename... Args
        //, REQUIRES(valid_type<T>() && std::is_constructible_v<T, Args...>)
        >
        requires(valid_type<T>() && std::constructible_from<T, Args...>)
    BufferTrivial(std::in_place_type_t<T>, Args&& ...args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
    {
        static_assert(std::is_trivially_destructible_v<T>, "");
        static_assert(std::is_trivially_copyable_v<T>, "");
        ::new (static_cast<void*>(as<T>())) T(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args
        //, REQUIRES(valid_type<T>() && std::is_constructible_v<T, Args...>)
        >
        requires(valid_type<T>() && std::constructible_from<T, Args...>)
    T* emplace(Args&& ...args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
    {
        static_assert(std::is_trivially_destructible_v<T>, "");
        static_assert(std::is_trivially_copyable_v<T>, "");
        return ::new (static_cast<void*>(as<T>())) T(std::forward<Args>(args)...);
    }

    void swap(BufferTrivial& that) noexcept {
        alignas(alignment) char tmp[size];
        ::memcpy(tmp, this->space_, size);
        ::memcpy(this->space_, that.space_, size);
        ::memcpy(that.space_, tmp, size);
    }
}; // BufferTrivial

template<typename Signature, size_t Size = sizeof(void*)*2, size_t Alignment = alignof(void*)> class FunctionTrivial;

template<size_t Size, size_t Alignment, typename Res, typename... Args>
class FunctionTrivial<Res(Args...), Size, Alignment>
{
    BufferTrivial<Size, Alignment> space_;
    using executor_t = Res(*)(Args..., void*);
    static constexpr Res default_executor(Args..., void*) { throw std::bad_function_call(); }
    constexpr static executor_t default_executor_ = default_executor;
    executor_t executor_ = default_executor_;

    template<typename Callable> static Res executor(Args... args, void* this_function) {
        static_assert(!std::is_member_function_pointer_v<Callable>, "");
        return (*reinterpret_cast<Callable*>(&static_cast<FunctionTrivial*>(this_function)->space_))(std::forward<Args>(args)...);
    }

public:
    // Default-constructed function is null (invocation will throw).
    FunctionTrivial() = default;

    // Construct a function from a callable.
    // The callable must be trivially copyable and trivially destructible.
    // The callable must not exceed the size and alignment of this function.
    template <typename CallableArg, typename Callable = std::decay_t<CallableArg> 
        //, REQUIRES(!std::is_same_v<FunctionTrivial, Callable>)
        >
        requires(!std::same_as<FunctionTrivial, Callable>)
    FunctionTrivial(CallableArg&& callable) : 
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
    FunctionTrivial(const FunctionTrivial&) = default;

    // The call operator invokes the callable.
    template <typename... CallArgs>
    Res operator()(CallArgs&&... callargs) const {
        return this->executor_(std::forward<CallArgs>(callargs)..., const_cast<FunctionTrivial*>(this));
    }

}; // FunctionTrivial
template<size_t Size, size_t Alignment, typename Res, typename... Args>
constexpr typename FunctionTrivial<Res(Args...), Size, Alignment>::executor_t FunctionTrivial<Res(Args...), Size, Alignment>::default_executor_;

using F = int(int, int, int, int);

using FF = FunctionTrivial<F>;

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
