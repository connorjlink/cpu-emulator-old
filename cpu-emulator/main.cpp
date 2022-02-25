//
//  main.cpp
//  cpu-emulator
//
//  Created by Connor Link on 2/12/22.
//

#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <fstream>

#include <filesystem>

#include "processor.hpp"
#include "file.h"

int main(int argc, const char** argv)
{
    auto processor = std::make_unique<Processor>();
    
    //auto* buffer = new std::byte[std::filesystem::file_size("./test.s")];
    
    
    
    
    File file(processor->memory, "./test.o");
    
    while (true)
    {
        processor->OnClock();
        
        //system("clear");
        
        std::clog << "A: " << std::to_string(processor->A) << '\n';
        std::clog << "B: " << std::to_string(processor->B) << '\n';
        std::clog << "H: " << std::to_string(processor->H) << '\n';
        std::clog << "L: " << std::to_string(processor->L) << '\n';
        //std::clog << "F: " << std::to_string(processor->F) << '\n';
        
        std::clog << "PC: " << std::to_string(processor->ip) << '\n';
        //std::clog << "Status: " << std::to_string(processor->state) << '\n';
        
        std::cin.get();
    }
    
    
    return 0;
}
