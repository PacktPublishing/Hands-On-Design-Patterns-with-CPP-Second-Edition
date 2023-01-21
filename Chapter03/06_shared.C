#include <memory>
#include <iostream>

template <typename T> struct ListNode { 
    T data;
    std::shared_ptr<ListNode> next;
    std::shared_ptr<ListNode> prev;
};

template <typename T> class ListIterator {
    std::shared_ptr<ListNode<T>> node_;
    public:
    ListIterator(std::shared_ptr<ListNode<T>> node) : node_(node) {}
    ListIterator(ListNode<T>* node) : node_(node) {}
    T& operator*() { return node_->data; }
    const T& operator*() const { return node_->data; }
    ListIterator& operator++() { node_ = node_->next; return *this; }
    ListIterator& operator--() { node_ = node_->prev; return *this; }
    bool operator==(const ListIterator& that) const { return node_.get() == that.node_.get(); }
    bool operator!=(const ListIterator& that) const { return !this->operator==(that); }
};

template <typename T> class List {
    std::shared_ptr<ListNode<T>> head_;
    public:
    List() = default;
    void insert(const T& data) {
        head_.reset(new ListNode<T>{data, head_, nullptr});
    }
    using iterator = ListIterator<T>;
    iterator begin() { return head_; }
    iterator end() { return nullptr; }
    ~List() {
        while (head_) { // Reset all next and prev pointers
            ListNode<T>* node = head_.get();
            node->prev.reset();
            head_ = std::move(node->next); // resets next, transfers ownership to head_
        }
        std::cout << "List deleted" << std::endl;
    }
};

struct S {
    int i {};
    S(int i) : i(i) { std::cout << "S created, i = " << i << std::endl; }
    S(const S& s) : i(s.i) { std::cout << "S copied, i = " << i << std::endl; }
    ~S() { std::cout << "S deleted, i = " << i << std::endl; }
    friend std::ostream& operator<<(std::ostream& out, const S& s) { out << s.i; return out; }
};

int main() {
    std::unique_ptr<List<S>> l{new List<S>};
    l->insert(1);
    l->insert(2);
    l->insert(3);
    std::cout << "List: ";
    for (const S& i : *l) std::cout << i << " ";
    std::cout << std::endl;
    {
        List<S>::iterator it = l->begin(); ++it;
        l.reset();
        std::cout << "List is deleted but iterator keeps its object alive: " << *it << std::endl;
        std::cout << "Deleting the last iterator" << std::endl;
    }
    std::cout << "Everything is deleted" << std::endl;
}
