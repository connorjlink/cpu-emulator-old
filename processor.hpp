//
//  processor.hpp
//  cpu-emulator
//
//  Created by Connor Link on 2/12/22.
//

#ifndef processor_hpp
#define processor_hpp

#include <cstdint>
#include <vector>

enum Opcode : std::uint8_t
{
    //no operation
    NOP = 0, /**/

    //adc
    ADC_A_imm8 = 1 + 128 /**/, ADC_A_B = 2/**/, ADC_A_H = 3/**/, ADC_A_L = 4/**/,
    ADC_B_imm8 = 5 + 128 /**/, ADC_B_A = 6/**/, ADC_B_H = 7/**/, ADC_B_L = 8/**/,

    //sbb
    SBB_A_imm8 = 9  + 128 /**/, SBB_A_B = 10/**/, SBB_A_H = 11/**/, SBB_A_L = 12/**/,
    SBB_B_imm8 = 13 + 128 /**/, SBB_B_A = 14/**/, SBB_B_H = 15/**/, SBB_B_L = 16/**/,

    //mvb
    MVB_A_B = 17 /**/, MVB_A_H = 18 /**/, MVB_A_L = 19 /**/,
    MVB_B_A = 20 /**/, MVB_B_H = 21 /**/, MVB_B_L = 22 /**/,
    MVB_H_A = 23 /**/, MVB_H_B = 24 /**/, MVB_H_L = 25 /**/,
    MVB_L_A = 26 /**/, MVB_L_B = 27 /**/, MVB_L_H = 28 /**/,

    //ldb
    LDB_A_imm8 = 29 + 128 /**/, LDB_A_mem8 = 30 + 64, /**/
    LDB_B_imm8 = 31 + 128 /**/, LDB_B_mem8 = 32 + 64, /**/
    LDB_H_imm8 = 33 + 128 /**/, LDB_H_mem8 = 34 + 64, /**/
    LDB_L_imm8 = 35 + 128 /**/, LDB_L_mem8 = 36 + 64, /**/

    //stb
    STB_mem8_A    = 37 + 64, /**/
    STB_mem8_B    = 38 + 64, /**/
    STB_mem8_H    = 39 + 64, /**/
    STB_mem8_L    = 40 + 64, /**/
    STB_mem8_imm8 = 41 + 64 + 128, /**/

    //jez
    JEZ_mem8 = 42 + 64, /**/
    JEZ_HL   = 43, /**/

    //nor
    NOR_A_imm8 = 46 + 128 /**/, NOR_A_B = 47, NOR_A_H = 48, NOR_A_L = 49,
    NOR_B_imm8 = 50 + 128 /**/, NOR_B_A = 51, NOR_B_H = 52, NOR_B_L = 53,

    //lfg
    LFG_A = 54,    /**/
    LFG_B = 55,    /**/
};


struct Processor
{
    std::uint16_t ip{ 0x0000 };
    std::vector<std::uint8_t> memory;
    int clockSpeed = 65536u;
    
    struct
    {
        union
        {
            struct
            {
                std::uint8_t H, L;
            };
            std::uint16_t HL{ 0x0000 };
        };
        
        union
        {
            struct
            {
                std::uint8_t A, B;
            };
            std::uint16_t AB{ 0x0000 };
        };
        
        union
        {
            std::uint8_t F{ 0x00 };
            
            struct
            {
                std::uint8_t Zero   : 1,
                             Carry  : 1,
                             Borrow : 1,
                             Halt   : 1;
            };
        };
    };
    
public:
    Processor(void) noexcept;
    
//private:
public:
    enum
    {
        FetchInstruction,
        FetchData
    } state = FetchInstruction;
    
    std::uint8_t insn = 0x00;
    
    void wait(int time) const noexcept;
    
    std::uint8_t fetch8(void) noexcept;
    std::uint16_t fetch16(void) noexcept;
    
public:
    void OnClock(void) noexcept;
    
};


#endif /* processor_hpp */
