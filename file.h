//
//  file.h
//  cpu-emulator
//
//  Created by Connor Link on 2/22/22.
//

#ifndef file_h
#define file_h

#include <fstream>
#include <filesystem>
#include <cassert>

struct File
{
public:
    //currently assumes that target is of the correct size, 65536 bytes
    //TODO: make this function not depend on that fact
    File(std::vector<std::uint8_t>& target, const char* filepath) noexcept
    {
        auto filesize = std::filesystem::file_size(filepath);
        
        //assert(filesize == (std::numeric_limits<std::uint16_t>::max() + 1));
        
        
        //auto* buffer = new std::uint8_t[filesize];
        
        std::ifstream file(filepath, std::ios::in | std::ios::binary);
        
        std::vector<std::uint8_t> buffer(filesize, 0x00);
        file.read((char*)buffer.data(), filesize);
        
        //file.read(buffer.data(), filesize);
        
        for (auto i = 0; i < filesize; ++i)
        {
            target[i] = static_cast<std::uint8_t>(static_cast<std::uint16_t>(buffer[i]));
        }
        
        
        //std::ifstream(filepath).read((char*)target, filesize);
        
        //auto file = std::ifstream(filepath);
        
        //file.read(target, filesize);
    }
};


#endif /* file_h */
