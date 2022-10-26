#ifndef MY_SHARED_COLLECTOR_H_INCLUDED
#define MY_SHARED_COLLECTOR_H_INCLUDED

#include <cstddef>

template <typename T>
class my_collector{
private:
    //to type-erase deleter and allocator
    struct base_holder{
        virtual ~base_holder() {}
        virtual void operator()(T*) = 0;
        virtual void* allocate(size_t) = 0;
    };

    template <typename Deleter>
    struct deleter_holder : public base_holder{
        deleter_holder(const Deleter &func) : f_{func} {}
        ~deleter_holder() {}

        void operator()(T* used_ptr) { f_(used_ptr); }
        void* allocate(size_t) { return nullptr; }

        Deleter f_;
    };

    template <typename Allocator>
    struct allocator_holder : public base_holder{
        allocator_holder(const Allocator &func) : f_{func} {}
        ~allocator_holder() {}

        void operator()(T*) {}
        void* allocate(size_t size_of) {
            return f_.allocate(size_of);
        }

        Allocator f_;
    };

    T* stored_ptr;
    base_holder* deleter;
    base_holder* allocator_;
    size_t ptr_refs;
public:

    //all constructors
    my_collector();
    my_collector(T*);

    template <typename Deleter>
    my_collector(T*, Deleter);

    template <typename Deleter, typename Allocator>
    my_collector(T*, Deleter, Allocator);

    //to use custom deleter and allocator
    void delete_ptr(T* = nullptr);
    T* create_ptr(size_t = 0);

    //all getters and setters

    void set_ptr(T* new_ptr) { stored_ptr = new_ptr; }

    size_t get_counts() const { return ptr_refs; }
    void set_counts(size_t new_counts) {
        ptr_refs = new_counts;
        if (ptr_refs == 0){
            delete this;
        }
    }

    void increment_counts() { ptr_refs++; }
    void decrement_count() {
        ptr_refs--;
        if (ptr_refs == 0){
            delete this;
        }
    }

    base_holder* get_delete_holder() const { return deleter; }
    base_holder* get_allocate_holder() const { return allocator_; }

    void set_delete_holder(base_holder* new_holder) { deleter = new_holder; }
    void set_allocate_holder(base_holder* new_holder) { allocator_ = new_holder; }

    //to delete private structures from the shared_ptr

    void delete_delete_holder() { if (deleter) { delete deleter; deleter = nullptr; } }
    void delete_allocate_holder() { if (allocator_) { delete allocator_; allocator_ = nullptr; } }

    //to make private structures in shared_ptr's functions

    template <typename Deleter>
    base_holder* make_deleter_holder(Deleter);

    template <typename Allocator>
    base_holder* make_allocator_holder(Allocator);

    ~my_collector(){
        delete_ptr(stored_ptr);
        delete_delete_holder();
        delete_allocate_holder();
    }
};

#endif // MY_SHARED_COLLECTOR_H_INCLUDED
