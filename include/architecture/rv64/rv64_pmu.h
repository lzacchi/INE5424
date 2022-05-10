// EPOS RISCV64 PMU Mediator Declarations

#ifndef __riscv64_pmu_h
#define __riscv64_pmu_h

#include <architecture/cpu.h>
#define __common_only__
#include <architecture/pmu.h>
#undef __common_only__

__BEGIN_SYS

class RISCV64_A_PMU: public PMU_Common
{
private:
    typedef CPU::Reg64 Reg64;
};

class PMU
{
    friend class CPU;

private:
    typedef RISCV64_A_PMU Engine;

public:
    PMU() {}

    // Instruction Commit Events, mhpmeventX[7:0]=0X0
    enum {
        EXCEPTION_TAKEN                    = 1 << 8,
        INT_LOAD_INSTRUCTION_RETIRED       = 1 << 9,
        INT_STORE_INSTRUCTION_RETIRED      = 1 << 10,
        ATOMIC_MEM_OPERATION_RETIRED       = 1 << 11,
        SYSTEM_INSTRUCTION_RETIRED         = 1 << 12,
        INT_ARITHMETIC_INSTRUCTION_RETIRED = 1 << 13,
        CONDITIONAL_BRANCH_RETIRED         = 1 << 14,
        JAL_INSTRUCTION_RETIRED            = 1 << 15,
        JALR_INSTRUCTION_RETIRED           = 1 << 16,
        INT_MUL_INSTRUCTION_RETIRED        = 1 << 17,
        INT_DIV_INSTRUCTION_RETIRED        = 1 << 18,
    };

    // Microarchitectural Events, mhpmeventX[7:0]=0x1
    enum {
        ADDRESS_GENERATION_INTERLOCK       = 1 << 8,
        LONG_LATENCY_INTERLOCK             = 1 << 9,
        CSR_READ_INTERLACK                 = 1 << 10,
        INSTRUCTION_CACHE_ITIM_BUSY        = 1 << 11,
        DATA_CACHE_DTIM_BUSY               = 1 << 12,
        BRANCH_DIRECTION_MISPREDICTION     = 1 << 13,
        BRANCH_JUMP_TARGET_MISPREDICTION   = 1 << 14,
        PIPELINE_FLUSH_FROM_CRS_WRITE      = 1 << 15,
        PIPELINE_FLUSH_FROM_OTHER_EVENT    = 1 << 16,
        INTEGER_MULTIPLICATION_INTERLOCK   = 1 << 17,
    };

    // Memory System Events, mhpmeventX[7:0]=0x2
    enum {
        INSTRUCTION_CACHE_MISS             = 1 << 8,
        MEMORY_MAPPED_IO_ACCESS            = 1 << 9,
    };

private:
    static void init() {}
};

__END_SYS

#endif
