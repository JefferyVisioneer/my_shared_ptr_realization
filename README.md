# my_shared_ptr_realization
Copy of shared ptr in C++. Not as good as it might be, i.e. couldn't make weak ptr yet and couldn't make full type-erased control block.
Files compiled in this way: 'my_shared_collector.h' -> 'my_shared_collector.cpp' -> 'my_ptr.h' -> 'my_ptr.cpp' -> 'my_shared_ptr.h'
What i failed to do:
1) Construction of ptr using my_shared_ptr::my_shared_ptr(const my_shared_ptr<Y>&, element_type*) constructor. The reason is that I couldn't make fully type-erased control block, so every shared_ptr contains pointer to control block, specified by 'element_type' parameter. That helped me to, by example, save 'int[]' pointers to 'int' ptrs and delete it by array deleter.
2) Weak_ptr. I think to try to write it when i will make fully type-erased control block.
3) Support of multi-thread operations. I just don't have enough knowledge to do this.
But in general it has like 80% support of standart operations that are used by people. 
