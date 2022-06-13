#include <process.h>
#include <time.h>
#include <utility/ostream.h>

using namespace EPOS;

OStream cout;

Thread* threads[100000];

int io_thread()
{
    cout << "Starting execution of IO-bound thread" << endl;
    Alarm::delay(30); // simulating IO bound thread with delays
    cout << "Finished execution of IO-bound thread" << endl;
    return 0;
}

int regular_thread()
{
    cout << "Starting execution of CPU-bound thread" << endl;
    int j = 2;
    for (int i = 0; i < 10000; i++){
        j = j + j;
        if (j < 0)
            j = 1;
    }
    cout << "Finished execution of CPU-bound thread" << endl;
    return 0;
}

int main()
{
    cout << "P1 Scheduler priority test" << endl;
    for (int i = 0; i < 100; i++) {
        auto fn = i % 2 == 0 ? &io_thread : &regular_thread;
        Thread* t = new Thread(fn);
        threads[i] = t;
    }
    for (int i = 0; i < 100; i++) {
        threads[i]->join();
    }
    return 0;
}
