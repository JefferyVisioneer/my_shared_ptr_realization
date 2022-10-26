#ifndef MY_PTR_H_INCLUDED
#define MY_PTR_H_INCLUDED

#include "my_shared_collector.cpp"

template <typename T>
class my_shared_ptr{
    using element_type = std::remove_extent_t<T>;

    element_type* stored_ptr;
    my_collector<element_type>* collector_ptr;

public:
    //all constructors

    my_shared_ptr();
    my_shared_ptr(std::nullptr_t);

    template <typename Y>
    my_shared_ptr(Y*);

    template <typename Y, typename Deleter>
    my_shared_ptr(Y*, Deleter);

    template <typename Deleter>
    my_shared_ptr(std::nullptr_t, Deleter);

    template <typename Y, typename Deleter, typename Allocator>
    my_shared_ptr(Y*, Deleter, Allocator);

    template <typename Deleter, typename Allocator>
    my_shared_ptr(std::nullptr_t, Deleter, Allocator);

    my_shared_ptr(const my_shared_ptr&);

    template <typename Y>
    my_shared_ptr(const my_shared_ptr<Y>&);

    my_shared_ptr(my_shared_ptr&&);

    template <typename Y>
    my_shared_ptr(my_shared_ptr<Y>&&);

    //all operator=()

    my_shared_ptr& operator=(const my_shared_ptr&);

    template <typename Y>
    my_shared_ptr& operator=(const my_shared_ptr<Y>&);

    my_shared_ptr& operator=(my_shared_ptr&&);

    template <typename Y>
    my_shared_ptr& operator=(my_shared_ptr<Y>&&);

    //variations of reset

    void reset();

    template <typename Y>
    void reset(Y*);

    template <typename Y, typename Deleter>
    void reset(Y*, Deleter);

    template <typename Y, typename Deleter, typename Allocator>
    void reset(Y*, Deleter, Allocator);

    void swap(my_shared_ptr& other);

    element_type* get() const { return stored_ptr; }

    T& operator*() const { return *stored_ptr; }
    T* operator->() const { return stored_ptr; }

    //couldn't made operator[]...

    size_t use_count() const { return collector_ptr->get_counts(); }

    //to change control-block object

    my_collector<element_type>* get_collector() const { return collector_ptr; }

    void set_collector(my_collector<element_type>* new_collector) { collector_ptr = new_collector; }

    void set_ptr(element_type* new_ptr) { stored_ptr = new_ptr; }

    ~my_shared_ptr() {
        if (collector_ptr)
            collector_ptr->decrement_count();
    }

};

template <typename T>
std::ostream& operator<<(std::ostream& os, const my_shared_ptr<T> &ptr)
{
    return os << ptr.get() << '\n';
}

template <typename T, typename ...Obj>
my_shared_ptr<T> make_my_shared(Obj ... objects)
{
    return my_shared_ptr<T>{new T(std::forward(objects)...)};
}

#endif // MY_PTR_H_INCLUDED
