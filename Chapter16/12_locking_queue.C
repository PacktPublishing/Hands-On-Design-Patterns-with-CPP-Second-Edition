// 11 returning a pair
#include <mutex>
#include <queue>
#include <optional>
#include <iostream>
using std::cout;
using std::endl;

template <typename T> class locking_queue {
    using mutex = std::mutex;
    using lock_guard = std::lock_guard<mutex>;
    public:
    using value_type = typename std::queue<T>::value_type;
    void push(const value_type& value) {
        lock_guard l(m_);
        q_.push(value);
    }
    void push(value_type&& value) {
        lock_guard l(m_);
        q_.push(value);
    }
    std::optional<value_type> pop() {
        lock_guard l(m_);
        if (q_.empty()) return std::nullopt;
        value_type value = std::move(q_.front());
        q_.pop();
        return { value };
    }

    private:
    std::queue<T> q_;
    mutex m_;
};

int main() {
    locking_queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    auto x = q.pop(); if (x) cout << *x << endl; else cout << "empty" << endl;
    x = q.pop(); if (x) cout << *x << endl; else cout << "empty" << endl;
    x = q.pop(); if (x) cout << *x << endl; else cout << "empty" << endl;
    x = q.pop(); if (x) cout << *x << endl; else cout << "empty" << endl;
}
