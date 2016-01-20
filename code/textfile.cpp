#include <vee/code/textfile.h>
#include <sys/stat.h>
#include <fstream>
#include <array>

namespace vee {

std::pair<charset, uint32_t> detect_charset(const char* filepath) throw(...)
{
    using namespace std;

    long size = 0;

    // Getting the size of a file
    {
        struct stat results;
        if (stat(filepath, &results) == NULL)
        {
            // The size of the file in bytes is in results.st_size
            size = results.st_size;
        }
        else
        {
            // An error occurred
            throw exceptions::file_open_failed();
        }
    }

    if (size < 2)
        return make_pair(charset::ascii, 0);

    // Opening a file
    array<unsigned char, 5> bom;
    try
    {
        ifstream file(filepath, ios::in | ios::binary);

        if (size > 4)
        {
            //file.seekg(ios::beg);
            bom.fill(0);
            file.read((char*)bom.data(), 4);
            if (bom[0] == 0x00 &&
                bom[1] == 0x00 &&
                bom[2] == 0xFE &&
                bom[3] == 0xFF)
                return make_pair(charset::utf32_big_endian, 4);
            else if (bom[0] == 0xFF &&
                     bom[1] == 0xFE &&
                     bom[2] == 0x00 &&
                     bom[3] == 0x00)
                return make_pair(charset::utf32_little_endian, 4);
            else if (bom[0] == 0xDD &&
                     bom[1] == 0x73 &&
                     bom[2] == 0x66 &&
                     bom[3] == 0x73)
                return make_pair(charset::utf_ebcdic, 4);
            else if (bom[0] == 0x84 &&
                     bom[1] == 0x31 &&
                     bom[2] == 0x95 &&
                     bom[3] == 0x33)
                     return make_pair(charset::gb_18030, 4);
            else if (bom[0] == 0x2B &&
                     bom[1] == 0x2F &&
                     bom[2] == 0x76)
            {
                switch (bom[4])
                {
                case 0x39:
                case 0x2B:
                case 0x2F:
                    return make_pair(charset::utf7, 4);
                case 0x38:
                    if (size > 5)
                    {
                        file.read((char*)bom.data() + 4, 1);
                        if (bom[4] == 0x2D)
                            return make_pair(charset::utf7, 5);
                    }
                    return make_pair(charset::utf7, 4);
                }
            }
        }

        if (size > 3)
        {
            file.seekg(ios::beg);
            bom.fill(0);
            file.read((char*)bom.data(), 3);
            if (bom[0] == 0xEF &&
                bom[1] == 0xBB &&
                bom[2] == 0xBF)
                return make_pair(charset::utf8, 3);
            else if (bom[0] == 0xF7 &&
                     bom[1] == 0x64 &&
                     bom[2] == 0x4C)
                     return make_pair(charset::utf1, 3);
            else if (bom[0] == 0x0E &&
                     bom[1] == 0xFE &&
                     bom[2] == 0xFF)
                     return make_pair(charset::scsu, 3);
            else if (bom[0] == 0xFB &&
                     bom[1] == 0xEE &&
                     bom[2] == 0x28)
                     return make_pair(charset::bocu_1, 3);
        }

        if (size > 2)
        {
            file.seekg(ios::beg);
            bom.fill(0);
            file.read((char*)bom.data(), 2);
            if (bom[0] == 0xFE &&
                bom[1] == 0xFF)
                return make_pair(charset::utf16_big_endian, 2);
            else if (bom[0] == 0xFF &&
                     bom[1] == 0xFE)
                     return make_pair(charset::utf16_little_endian, 2);
        }

        return make_pair(charset::ascii, 0);
    }
    catch (ifstream::failure&)
    {
        throw exceptions::file_open_failed();
    }
}

void textfile::read_from_file(const char* filepath) throw(...)
{
    
}

} // !namespace vee