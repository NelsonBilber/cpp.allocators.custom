#include <iostream>
#include <limits>
#include <memory>
#include <vector>

template <class T>
class TrackingAllocator
{
public:
    
    using value_type = T;
    
    //optionals
    using pointer = T *;
    using const_pointer = const T *;
    using void_pointer = void *;
    using const_void_pointer = const void *;
    using size_type = size_t;
    using diference_type = std::ptrdiff_t;
    
    //Allocate independent types
    template<class U>
    struct rebind
    {
        using other = TrackingAllocator<U>;
    };
    
    TrackingAllocator() = default;
    
    template<class U>
    TrackingAllocator(const TrackingAllocator<U> &other){}
    
    ~TrackingAllocator() = default;
    
    pointer allocate(size_type numObjects)
    {
        mAllocations += numObjects;

        return static_cast<pointer>(operator new (sizeof(T) * numObjects));
    }
    
    //locality heap to improve cache preformance
    //pointer to previous allocated element
    pointer allocate(size_type numObject, const_void_pointer hint)
    {
        return allocate(numObject);
    }
    
    void deallocate(pointer p , size_type numObjects)
    {
        operator delete(p);
    }
    
    //max objects allow to allocate
    size_type max_size() const
    {
        return std::numeric_limits<size_type>::max();
    }
    
    //constructor function
    template<class U, class ... Args>
    void construct (U *p, Args && ...args)
    {
        new (p) U(std::forward<Args>(args)...);
    }
    
    
    //destroy object
    template < class U>
    void destroy(U *p)
    {
        p->~U();
    }
    
    size_type get_allocations() const
    {
        return mAllocations;
    }
    
private:
    static size_type mAllocations;
    
};

template<class T>
 typename TrackingAllocator<T>::size_type TrackingAllocator<T>::mAllocations = 0;


int main()
{
   // using TAint = TrackingAllocator<int>;
   // using TAdouble = TAint::rebind<double>::other;
    
    std:: vector<int, TrackingAllocator<int> > v(5);
    std::cout << v.get_allocator().get_allocations() << std::endl;
    return 0;
}
