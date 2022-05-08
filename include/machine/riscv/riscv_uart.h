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
        DATA_READY          = 1 << 0,
        THOLD_REG           = 1 << 5,
        TEMPTY_REG          = 1 << 6,
        DATA_BITS_MASK      = 1 << 1 | 1 << 0,
        PARITY_MASK         = 1 << 3,
        DLAB_ENABLE         = 1 << 7,
        STOP_BITS_MASK      = 1 << 2,
        LOOPBACK_MASK       = 1 << 4,
        FIFO_ENABLE         = 1 << 0,
        DEFAULT_DATA_BITS   = 5,
        FULL_TXDATA         = 1 << 31,
        EMPTY_RXDATA        = 1 << 31,
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

        reg(TXCTRL) = Reg32(0);
    }

    void config(unsigned int * baud_rate, unsigned int * data_bits, unsigned int * parity, unsigned int * stop_bits) {
        *baud_rate = Traits<UART>::CLOCK / (reg(DIV) + 1);
        *parity = (reg(LCR) & PARITY_MASK) >> PARITY_MASK
        *stop_bits = (reg(LCR) & STOP_BITS_MASK) >> STOP_BITS_MASK + 1;
    }

    Reg32 rxd() {
        return reg(RXDATA);
    }

    void txd(Reg32 c) {
        reg(TXDATA) = c;
    }

    bool rxd_ok() {
        return reg(RXDATA) & EMPTY_RXDATA;
    }

    bool txd_ok() {
        return reg(TXDATA) & FULL_TXDATA;
    }

    bool rxd_full() { return false; } //still not defined

    bool txd_empty() {
        return (reg(LSR) & TEMPTY_REG);
    }

    bool busy() {
        return false; // not applicable
    }

    char get() {
        while (rxd_ok()) {
            return rxd();
        }
    }

    void put(char c) {
        while (txd_ok()) {
            txd(c);
        }
    }

    void flush() { while(!txd_empty()); }
    bool ready_to_get() { return rxd_ok(); }
    bool ready_to_put() { return txd_ok(); }

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
