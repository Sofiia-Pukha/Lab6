#include <iostream>
#include <coroutine>
#include <random>
#include <ctime>

using namespace std;

struct Task 
{
    struct promise_type;
    using handle_type = coroutine_handle<promise_type>;
    handle_type coro_handle;
    Task(handle_type h) : coro_handle(h) {}
     ~Task() 
    {
        if (coro_handle) coro_handle.destroy();
    }

    struct promise_type 
  {
        Task get_return_object() 
    {
            return Task{handle_type::from_promise(*this)};
    }
        
        
        suspend_always initial_suspend() { return {};}
        
        
        suspend_always final_suspend() noexcept { return {}; }
        
        void return_void() {}
        
        void unhandled_exception() 
        {
            exit(1); 
        }
    };

    bool await_ready() { return false; }

    void await_suspend(coroutine_handle<> caller) 
    {
        coro_handle.resume();
    }

    void await_resume() {}
};

int main() 
{
    srand(time(0));
    
    cout << "Laboratory work started..." << endl;
    
    return 0;
}
