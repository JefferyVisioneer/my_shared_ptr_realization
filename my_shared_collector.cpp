#include "my_shared_collector.h"
#include <memory>

template <typename T>
my_collector<T>::my_collector()
{
    stored_ptr = nullptr;
    deleter = nullptr;
    allocator_ = nullptr;
    ptr_refs = 0;
}

template <typename T>
my_collector<T>::my_collector(T* new_ptr)
{
    stored_ptr = new_ptr;

    if (stored_ptr){
        deleter = new deleter_holder<std::default_delete<T>>(std::default_delete<T>{});
        allocator_ = new allocator_holder<std::allocator<T>>(std::allocator<T>{});
    }
    else{
        deleter = nullptr;
        allocator_ = nullptr;
    }

    ptr_refs = 0;
}

template <typename T>
template <typename Deleter>
my_collector<T>::my_collector(T* new_ptr, Deleter d)
{
    stored_ptr = new_ptr;
    deleter = new deleter_holder<Deleter>(d);

    if (stored_ptr){
        allocator_ = new allocator_holder<std::allocator<T>>(std::allocator<T>{});
    }
    else{
        allocator_ = nullptr;
    }

    ptr_refs = 0;
}

template <typename T>
template <typename Deleter, typename Allocator>
my_collector<T>::my_collector(T* new_ptr, Deleter d, Allocator alloc)
{
    stored_ptr = new_ptr;
    deleter = new deleter_holder<Deleter>(d);
    allocator_ = new allocator_holder<Allocator>(alloc);
    ptr_refs = 0;
}

template <typename T>
void my_collector<T>::delete_ptr(T* ptr)
{
    deleter->operator()(ptr);
}

template <typename T>
T* my_collector<T>::create_ptr(size_t size_of_obj)
{
    return allocator_->allocate(size_of_obj);
}

template <typename T>
template <typename Deleter>
typename my_collector<T>::base_holder* my_collector<T>::make_deleter_holder(Deleter d)
{
    return new deleter_holder<Deleter>(d);
}

template <typename T>
template <typename Allocator>
typename my_collector<T>::base_holder* my_collector<T>::make_allocator_holder(Allocator alloc)
{
    return new allocator_holder<Allocator>(alloc);
}
