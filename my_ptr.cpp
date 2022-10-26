#include "my_ptr.h"

template <typename T>
my_shared_ptr<T>::my_shared_ptr()
{
    stored_ptr = nullptr;
    collector_ptr = nullptr;
}

template <typename T>
my_shared_ptr<T>::my_shared_ptr(std::nullptr_t new_ptr)
{
    stored_ptr = new_ptr;
    collector_ptr = nullptr;
}

template <typename T>
template <typename Y>
my_shared_ptr<T>::my_shared_ptr(Y* new_ptr)
{
    stored_ptr = new_ptr;
    collector_ptr = new my_collector<element_type>(new_ptr, std::default_delete<T>{}, std::allocator<element_type>{});
    collector_ptr->increment_counts();
}

template <typename T>
template <typename Y, typename Deleter>
my_shared_ptr<T>::my_shared_ptr(Y* new_ptr, Deleter new_delete)
{
    stored_ptr = new_ptr;
    collector_ptr = new my_collector<element_type>(new_ptr, new_delete, std::allocator<element_type>{});
    collector_ptr->increment_counts();
}

template <typename T>
template <typename Deleter>
my_shared_ptr<T>::my_shared_ptr(std::nullptr_t new_ptr, Deleter new_delete)
{
    stored_ptr = new_ptr;
    collector_ptr = new my_collector<element_type>(new_ptr, new_delete, std::allocator<element_type>{});
}

template <typename T>
template <typename Y, typename Deleter, typename Allocator>
my_shared_ptr<T>::my_shared_ptr(Y* new_ptr, Deleter d, Allocator alloc)
{
    stored_ptr = new_ptr;
    collector_ptr = new my_collector<element_type>(new_ptr, d, alloc);
    collector_ptr->increment_counts();
}

template <typename T>
template <typename Deleter, typename Allocator>
my_shared_ptr<T>::my_shared_ptr(std::nullptr_t new_ptr, Deleter d, Allocator alloc)
{
    stored_ptr = new_ptr;
    collector_ptr = new my_collector<element_type>(new_ptr, d, alloc);
}

template <typename T>
my_shared_ptr<T>::my_shared_ptr(const my_shared_ptr<T>& other)
{
    stored_ptr = other.get();
    collector_ptr = other.get_collector();
    collector_ptr->increment_counts();
}

template <typename T>
template <typename Y>
my_shared_ptr<T>::my_shared_ptr(const my_shared_ptr<Y>& other)
{
    stored_ptr = other.get();
    collector_ptr = other.get_collector();
    collector_ptr->increment_counts();
}

template <typename T>
my_shared_ptr<T>::my_shared_ptr(my_shared_ptr<T>&& other)
{
    stored_ptr = other.get();
    other.set_ptr(nullptr);
    collector_ptr = other.get_collector();
    other.set_collector(nullptr);
}

template <typename T>
template <typename Y>
my_shared_ptr<T>::my_shared_ptr(my_shared_ptr<Y>&& other)
{
    stored_ptr = other.get();
    other.set_ptr(nullptr);
    collector_ptr = other.get_collector();
    other.set_collector(nullptr);
}

template <typename T>
my_shared_ptr<T>& my_shared_ptr<T>::operator=(const my_shared_ptr<T>& r)
{
    stored_ptr = r.get();
    set_collector(r.get_collector());
    collector_ptr->increment_counts();

    return *this;
}

template <typename T>
template <typename Y>
my_shared_ptr<T>& my_shared_ptr<T>::operator=(const my_shared_ptr<Y>& r)
{
    stored_ptr = r.get();
    set_collector(r.get_collector());
    collector_ptr->increment_counts();

    return *this;
}

template <typename T>
my_shared_ptr<T>& my_shared_ptr<T>::operator=(my_shared_ptr<T>&& r)
{
    stored_ptr = r.stored_ptr;
    r.stored_ptr = nullptr;
    set_collector(r.collector_ptr);
    r.collector_ptr = nullptr;

    return *this;
}

template <typename T>
template <typename Y>
my_shared_ptr<T>& my_shared_ptr<T>::operator=(my_shared_ptr<Y>&& r)
{
    stored_ptr = r.get();
    r.set_ptr(nullptr);
    set_collector(r.get_collector());
    r.set_collector(nullptr);

    return *this;
}

template <typename T>
void my_shared_ptr<T>::reset()
{
    if (collector_ptr){
        collector_ptr->decrement_count();
    }
    stored_ptr = nullptr;
}

template <typename T>
template <typename Y>
void my_shared_ptr<T>::reset(Y* new_ptr)
{
    if (stored_ptr){
        collector_ptr->decrement_count();
    }

    stored_ptr = new_ptr;
    collector_ptr = new my_collector<element_type>(new_ptr, std::default_delete<T>{}, std::allocator<Y>{});
    collector_ptr->increment_counts();
}

template <typename T>
template <typename Y, typename Deleter>
void my_shared_ptr<T>::reset(Y* new_ptr, Deleter d)
{
    if (stored_ptr){
        collector_ptr->decrement_count();
    }

    stored_ptr = new_ptr;

    collector_ptr = new my_collector<element_type>(new_ptr, d, std::allocator<Y>{});
    collector_ptr->increment_counts();
}

template <typename T>
template <typename Y, typename Deleter, typename Allocator>
void my_shared_ptr<T>::reset(Y* new_ptr, Deleter d, Allocator alloc)
{
    if (stored_ptr){
        collector_ptr->decrement_count();
    }

    stored_ptr = new_ptr;

    collector_ptr = new my_collector<element_type>(new_ptr, d, alloc);
    collector_ptr->increment_counts();
}

template <typename T>
void my_shared_ptr<T>::swap(my_shared_ptr<T> &other)
{
    {
       auto temp_ptr {stored_ptr};
        stored_ptr = other.stored_ptr;
        other.stored_ptr = temp_ptr;
    }

    if (collector_ptr){

        if (other.collector_ptr){
            auto temp_collector {get_collector()};
            set_collector(other.get_collector());
            other.set_collector(temp_collector);
        }
        else{
            collector_ptr->decrement_count();
            collector_ptr = nullptr;
        }

    }
    else{

        if (other.collector_ptr){
            set_collector(other.collector_ptr);
        }
        else{
            //nothing
        }

    }
}
