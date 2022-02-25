//
//  processor.cpp
//  cpu-emulator
//
//  Created by Connor Link on 2/12/22.
//

#include "processor.hpp"

#include <chrono>
#include <limits>
#include <thread>
#include <ratio>

Processor::Processor(void) noexcept
{
    memory.resize(std::numeric_limits<std::uint16_t>::max() + 1);
    
    for (auto i = 0; i < memory.size(); ++i)
    {
        memory[i] = 0;
    }
}

void Processor::wait(int time) const noexcept
{
    for (auto i = 0; i < time; ++i)
    {
        std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1, 1>>(1.0 / static_cast<double>(clockSpeed)));
    }
}

std::uint8_t Processor::fetch8(void) noexcept
{
    wait(1);
    auto val = memory[ip];
    ++ip;
    return val;
}

std::uint16_t Processor::fetch16() noexcept
{
    auto val1 = fetch8();
    auto val2 = fetch8();
    return std::uint16_t(val1 | (val2 << 8));
}

void Processor::OnClock(void) noexcept
{
#define Check(expr, change) if (expr) { change = 1; } else { change = 0; }
#define CheckZero(reg) Check(reg == 0, Zero)
#define CheckCarry(reg) Check(reg < reg##copy, Carry)
#define CheckBorrow(reg) Check(reg > reg##copy, Borrow)
    Halt = memory[0xFFFF];

    if (!Halt)
    {
    switch (state)
    {
    case FetchInstruction:
    {
        insn = fetch8();
        state = FetchData;
    } break;

    case FetchData:
    {
        switch (insn & 0b11000000)
        {
        //register only operations
        case 0:
        {
            auto Acopy = A;
            auto Bcopy = B;

            switch (insn)
            {
#define MVB_INSN(d, t) case MVB_##d##_##t: d = t; break;
                MVB_INSN(A, B) MVB_INSN(A, H) MVB_INSN(A, L)
                MVB_INSN(B, A)                MVB_INSN(B, H) MVB_INSN(B, L)
                MVB_INSN(H, A) MVB_INSN(H, B)                 MVB_INSN(H, L)
                MVB_INSN(L, A) MVB_INSN(L, B) MVB_INSN(L, H)
#undef MVB_INSN

#define ADD_INSN(d, t) case ADC_##d##_##t: d += t; wait(2); CheckCarry(d) CheckZero(d) break;
                ADD_INSN(A, B) ADD_INSN(A, H) ADD_INSN(A, L)
                ADD_INSN(B, A) ADD_INSN(B, H) ADD_INSN(B, L)
#undef ADD_INSN
                    
#define SUB_INSN(d, t) case SBB_##d##_##t: d -= t; wait(2); CheckBorrow(d) CheckZero(d) break;
                SUB_INSN(A, B) SUB_INSN(A, H) SUB_INSN(A, L)
                SUB_INSN(B, A) SUB_INSN(B, H) SUB_INSN(B, L)
#undef SUB_INSN


#define NOR_INSN(d, t) case NOR_##d##_##t: d = ~(d | t); wait(2); CheckZero(d); break;
                NOR_INSN(A, B) NOR_INSN(A, H) NOR_INSN(A, L)
                NOR_INSN(B, A) NOR_INSN(B, H) NOR_INSN(B, L)
#undef NOR_INSN

#define LFG_INSN(d) case LFG_##d: d = F; break;
                LFG_INSN(A) LFG_INSN(B)
#undef LFG_INSN

            case JEZ_HL: if (Zero) { ip = HL; wait(1); } break;
            }
        } break;

        //next two bytes are memory address
        case 64:
        {
            wait(1);
            switch (insn)
            {
#define LDB_INSN(reg) case LDB_##reg##_mem8: reg = memory[fetch16()]; break;
                LDB_INSN(A) LDB_INSN(B)
                LDB_INSN(H) LDB_INSN(L)
#undef LDB_INSN

#define STB_INSN(reg) case STB_mem8_##reg: memory[fetch16()] = reg; break;
                STB_INSN(A) STB_INSN(B)
                STB_INSN(H) STB_INSN(L)
#undef STB_INSN

            case JEZ_mem8: if (Zero) { ip = fetch16(); wait(1); } break;
            }
        } break;

        //next byte is an immediate
        case 128:
        {
            auto Acopy = A;
            auto Bcopy = B;

            switch (insn)
            {
#define MATH_INSN(op, reg, s, c, t) case op##_##reg##_imm8: reg s; wait(t); c break;
#define ADC_INSN(reg) MATH_INSN(ADC, reg, += fetch8(), CheckCarry(reg) CheckZero(reg), 2)
                ADC_INSN(A)
                ADC_INSN(B)
#undef ADC_INSN
#define SBB_INSN(reg) MATH_INSN(SBB, reg, -= fetch8(), CheckBorrow(reg) CheckZero(reg), 2)
                SBB_INSN(A)
                SBB_INSN(B)
#undef SBB_INSN
#define LDB_INSN(reg) MATH_INSN(LDB, reg, = fetch8(), , 1)
                LDB_INSN(A) LDB_INSN(B)
                LDB_INSN(H) LDB_INSN(L)
#undef LDB_INSN
#define NOR_INSN(reg) MATH_INSN(NOR, reg, = ~(reg | fetch8()), CheckZero(reg), 2)
                NOR_INSN(A)
                NOR_INSN(B)
#undef NOR_INSN
#undef MATH_INSN
            }
        } break;

        //next two bytes are memory addresses, following is an immediate
        case 192:
        {
            memory[fetch16()] = fetch8();
            wait(1);
        } break;
        }
        state = FetchInstruction;
    } break;
    }
    }
#undef Check
#undef CheckZero
#undef CheckCarry
#undef CheckBorrow
}
