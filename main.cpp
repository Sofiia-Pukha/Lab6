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
        
        
        suspend_always initial_suspend() { return {}; }
        
        
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
        caller.resume();
    }

    void await_resume() {}
};

Task CoroutineA(int number) 
{
    cout << "[Coroutine A] Even number: " << number << endl;
    co_return;
}

Task CoroutineB(int number) 
{
    int square = number * number;
    cout << "[Coroutine B] Odd number(squared): " << square << endl;
    co_return;
}

Task MainGenerator(int count) 
{
    cout << "Start generating sequence..." << endl;

    for (int i = 0; i < count; ++i) 
    {
        int num = rand() % 256 + 1;
        cout << "Generated: " << num << " -> ";

        if (num % 2 == 0) 
        {
            co_await CoroutineA(num);
        } 
        else 
        {
            co_await CoroutineB(num);
        }
    }
    cout << "Generation finished." << endl;
    co_return;
}

int main() 
{
    srand(time(0));
    Task app = MainGenerator(10);
    
    if (app.coro_handle) 
    {
        app.coro_handle.resume();
    }
    return 0;
}
