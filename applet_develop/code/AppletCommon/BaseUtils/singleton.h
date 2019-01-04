

#ifndef __SINGLETON_H__
#define __SINGLETON_H__ 

#include "mutex_lock.h"
#include "base_define.h"

using namespace base_utils;
namespace base_utils 
{

template <typename T> struct CreateUsingNew
{
    static T* Create (void)
    {
        return new T;
    }

    static void Destroy(T* p)
    {
        delete p;
    }
};

template <typename T, template <typename> class CreationPolicy = CreateUsingNew>
class HSingleton
{
public:
    static T* Instance (void);
    static void Destroy (void);

private:
    HSingleton (void);
    HSingleton (const HSingleton&);
    HSingleton& operator= (const HSingleton&);

private: 
    static T*       _instance;
    static CMutex   _mutex;
};

//implement
template <typename T, template <typename> class CreationPolicy> 
CMutex HSingleton<T, CreationPolicy>::_mutex;

template <typename T, template <typename> class CreationPolicy> 
T* HSingleton<T, CreationPolicy>::_instance = 0;


template <typename T, template <typename> class CreationPolicy> 
T* HSingleton<T, CreationPolicy>::Instance (void)
{
    if (0 == _instance)
    {
        CScopedLock guard(_mutex);

        if (0 == _instance)
        {
            T* tmp = CreationPolicy<T>::Create ();
            __sync_synchronize();
            _instance = tmp;
        }
    }

    return _instance;
}

template <typename T, template <typename> class CreationPolicy> 
void HSingleton<T, CreationPolicy>::Destroy (void)
{
    if(0 != _instance)
    {
        CScopedLock guard(_mutex);
        if(0 != _instance)
        {
            CreationPolicy<T>::Destroy (_instance);
            _instance = 0;
        }
    }

    return;
}

template <typename T, template <typename> class CreationPolicy = CreateUsingNew>
class CSingleton: public HSingleton<T, CreateUsingNew>
{};

}

#endif
