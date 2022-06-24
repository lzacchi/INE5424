#include <utility/ostream.h>
#include <architecture/rv64/rv64_cpu.h>
#include <machine/riscv/riscv_timer.h>
#include <machine/riscv/riscv_ic.h>

using namespace EPOS;

OStream cout;

int main()
{
    cout << "===== P2 multicore preparation tests =====" << endl;
    cout << endl;

    cout << "Testing CPU::id:" << endl;
    cout << CPU::id() << endl;
    cout << "Testing CPU::cores:" << endl;
    cout << CPU::cores() << endl;

    cout << endl;

    cout << "=== Timer tests ===" << endl;
    cout << endl;
    cout << "Testing Timer::config calculating mtimecmp offsets" << endl;
    auto mtimecmp_value = Timer::reg(CLINT::MTIMECMP);
    cout << "Current mtimecmp is: " << mtimecmp_value << endl;
    auto offset = 1UL;
    Timer::config(offset);
    auto new_mtimecmp_value = Timer::reg(CLINT::MTIMECMP);
    cout << "New mtimecmp is: " << new_mtimecmp_value << endl;
    cout << "New value of mtimecmp must be larger than before" << endl;
    assert(new_mtimecmp_value > mtimecmp_value + offset);

    cout << endl;

    return 0;
}
