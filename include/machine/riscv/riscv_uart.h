// EPOS RISC-V UART Mediator Declarations

#ifndef __riscv_uart_h
#define __riscv_uart_h

#include <architecture/cpu.h>
#include <machine/uart.h>
#include <system/memory_map.h>

__BEGIN_SYS

class UART: private UART_Common
{
private:
    typedef CPU::Reg32 Reg32;

    static const unsigned int UNIT = Traits<UART>::DEF_UNIT;
    static const unsigned int BAUD_RATE = Traits<UART>::DEF_BAUD_RATE;
    static const unsigned int DATA_BITS = Traits<UART>::DEF_DATA_BITS;
    static const unsigned int PARITY = Traits<UART>::DEF_PARITY;
    static const unsigned int STOP_BITS = Traits<UART>::DEF_STOP_BITS;

    // UART registers offsets from UART_BASE
    enum {
        TXDATA = 0x00,
        RXDATA = 0x04,
        TXCTRL = 0x08,
        RXCTRL = 0x0C,
        IE     = 0x10,
        IP     = 0x14,
        DIV    = 0x18,
    };

    // Useful bits from multiple registers
    enum {
        TXDATA_FULL     = 1 << 31,
        TXCTRL_TXEN     = 0,
        TXCTRL_NSTOP    = 1,
        TXCTRL_TXCNT    = 16,

        RXDATA_EMPTY    = 1 << 31,
        RXCTRL_RXEN     = 0,
        RXCTRL_RXCNT    = 16,

        IP_TXWM         = 0,
        IP_RXWM         = 1,

        IE_TXWM         = 0,
        IE_RXWM         = 1,
    };

public:
    using UART_Common::NONE;
    using UART_Common::EVEN;
    using UART_Common::ODD;

public:
    UART(unsigned int unit = UNIT, unsigned int baud_rate = BAUD_RATE, unsigned int data_bits = DATA_BITS, unsigned int parity = PARITY, unsigned int stop_bits = STOP_BITS) {
        config(baud_rate, data_bits, parity, stop_bits);
    }

    void config(unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits) {
        Reg32 br = Traits<UART>::CLOCK / (reg(DIV) + 1);
        Reg32 div = br >> 16;
        reg(DIV) = div;
        reg(IE) = Reg32(4);

        if (stop_bits == 1)
        {
            reg(TXCTRL) &= ~(1 << TXCTRL_NSTOP);
        } else if (stop_bits == 2)
        {
            reg(TXCTRL) |= (1 << TXCTRL_NSTOP);
        }

        reg(TXCTRL) |= (1 << TXCTRL_TXEN);
        reg(RXCTRL) |= (1 << RXCTRL_RXEN);
    }

    void config(unsigned int * baud_rate, unsigned int * data_bits, unsigned int * parity, unsigned int * stop_bits) {
        *baud_rate = Traits<UART>::CLOCK / (reg(DIV) + 1);
        
        if (reg(TXCTRL) & TXCTRL_NSTOP) {
            *stop_bits = 2;
        } else {
            *stop_bits = 1;
        }

    }

    Reg32 rxd() {
        return reg(RXDATA);
    }

    void txd(Reg32 c) {
        reg(TXDATA) = c;
    }

    bool rxd_empty() {
        return reg(RXDATA) & RXDATA_EMPTY;
    }

    bool txd_full() {
        return reg(TXDATA) & TXDATA_FULL;
    }

    bool rxd_full() { return false; } //still not defined

    bool txd_empty() 
    {
        return (reg(IP) & (1 << IP_TXWM));
    }

    bool busy() {
        return false; // not applicable
    }

    char get() {
        while (!ready_to_get());
        return rxd();
    }

    void put(char c) {
        while (!txd_full());
        txd(c);
    }

    void flush() { while(!txd_empty()); }
    bool ready_to_get() { return !rxd_empty(); }
    bool ready_to_put() { return !txd_full(); }

    void reset() {
        // Reconfiguring the UART implicitly resets it
        unsigned int b, db, p, sb;
        config(&b, &db, &p, &sb);
        config(b, db, p, sb);
    }

    void power(const Power_Mode & mode) {}

private:
    static void init() {}

    static volatile CPU::Reg32& reg(unsigned int o) {
        return reinterpret_cast<volatile CPU::Reg32*>(Memory_Map::UART_BASE)[o / sizeof(CPU::Reg32)];
    }
};

__END_SYS

#endif
