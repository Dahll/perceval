//
// Created by adrien on 21/05/19.
//

#include <optional>
#include <iostream>
#include <array>
#include <sstream>
#include "generate.hh"
#include <fstream>
#include <string>
#include <random>
#include <iostream>


std::string int_to_string(uint64 index) {
    std::string a;
    convertToBinary(index, a);
    int len = a.length();
    while (len < 64)
    {
        a.insert(0, "0");
        len += 1;
    }
    int i = 9;
    a.insert(0, "\n");
    while (i < 64)
    {
        a.insert(i, "\n");
        i += 9;
    }

    return a;
}











std::string print_list(const array_optional& list)
{
    std::ostringstream str1;
    str1 << "{";
    int max = list.size();
    int i = 0;
    while (i < max)
    {
        if (list[i] == -1ull)
            str1 << "-1ull,";
        else
        {
            auto a = list[i];
            str1 << a;
            str1 << "u,";
        }
        i += 1;
    }
    str1.seekp(-1, std::ostringstream::cur);
    str1 << "}\n";
    return str1.str();
}



std::string print_list_zobrist(const array_zobrist& list)
{
    std::ostringstream str1;
    str1 << "{";
    int max = list.size();
    int i = 0;
    while (i < max)
    {
        if (list[i] == -1ull)
            str1 << "-1ull,";
        else
        {
            auto a = list[i];
            str1 << a;
            str1 << "ull,";
        }
        i += 1;
    }
    str1.seekp(-1, std::ostringstream::cur);
    str1 << "}\n";
    return str1.str();
}




std::string print_magique(const array_65& list)
{
    std::ostringstream str1;
    str1 << "{";
    int max = list.size();
    int i = 0;
    while (i < max)
    {
        if (list[i] == -1ull)
            str1 << "-1ull,";
        else
        {
            auto a = list[i];
            str1 << a;
            str1 << "u,";
        }
        i += 1;
    }
    str1.seekp(-1, std::ostringstream::cur);
    str1 << "}\n";
    return str1.str();
}

std::string print_tab(const array_array& tab) {
    std::stringstream str1;
    str1 << "{";
    std::stringstream strbis;
    strbis << "{";
    int z = 0;
    for (const auto& list : tab)
    {
        if (z != 0)
        {
            strbis << list.size();
            strbis << ",";
            str1 << print_list(list);
            str1 << ",";
        }
        z += 1;
    }
    strbis.seekp(-1, std::stringstream::cur);
    str1.seekp(-1, std::stringstream::cur);
    strbis << "}";
    str1 << "}";

    return str1.str(); //+ strbis.str() + std::to_string(z);

}


void convertToBinary(uint64 n, std::string& str)
{
    if (n / 2ull != 0) {
        convertToBinary(n / 2ull, str);
    }
    uint64 a = n % 2ull;
    str.push_back(a + 48ull);
}








// Returns a unsigned 64 bit random number.

uint64 U64Rand() {
    return (uint64 (0xFFFF & rand()) << 48) |
           (uint64 (0xFFFF & rand()) << 32) |
           (uint64 (0xFFFF & rand()) << 16) |
           uint64 (0xFFFF & rand());
}

// Bias the random number to contain more 0 bits.
uint64 ZeroBitBiasedRandom() {
    return U64Rand() & U64Rand() & U64Rand();
}

array_array find_magique_tower()
{
    array_array input = {0};
    // array_65 array_magique = {0};
    return_tab(input);
    auto output = array_array();
    int i = 1;
    while (i < 65)
    {
        generate_new_ouput(magic_number_tower[i], output, input, i);
        i += 1;
    }
    return output;
}

array_array find_magique_bishop()
{
    array_array input = {0};
    // array_65 array_magique = {0};
    return_tab_bishop(input);
    auto output = array_array();
    int i = 1;
    while (i < 65) {
        generate_new_ouput_bishop(magic_number_bishop[i], output, input, i);
        i += 1;
    }
    return output;
}



