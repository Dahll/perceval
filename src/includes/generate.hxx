//
// Created by adrien on 23/05/19.
//

#pragma once

#include <optional>
#include <iostream>
#include <array>
#include <sstream>
#include <fstream>
#include <string>
#include <random>
#include <iostream>


constexpr array_64 generate_list()
{
    auto list = std::array<uint64, 64>();
    list[0] = 1;
    uint64 num = 2ull;

    for (int i = 1; i < 64; ++i)
    {
        list[i] = num;
        num *= 2ull;
    }
    return list;
}

/* Romain */

constexpr void rec_input_tower(const std::array<uint64, 64>& list, int x, int y, int n,
                               int s, int o, int e, int pos, uint64 output, array_optional& output_l, int& i)
{
    if (pos < o)
    {
        uint64 str1 = add_byte(output, list[y * 8 + x + pos + 1]);
        rec_input_tower(list, x, y, n, s, o, e, pos + 1, output, output_l, i);
        rec_input_tower(list, x, y, n, s, o, e, pos + 1, str1, output_l, i);

    }
    else if (pos < o + n)
    {
        uint64 str1 = add_byte(output, list[(y + pos - o + 1) * 8 + x ]);
        rec_input_tower(list, x, y, n, s, o, e, pos + 1, output, output_l, i);
        rec_input_tower(list, x, y, n, s, o, e, pos + 1, str1, output_l, i);
    }
    else if (pos < o + n + e)
    {
        uint64 str1 = add_byte(output, list[y * 8 + x - (pos - n - o + 1)]);
        rec_input_tower(list, x, y, n, s, o, e, pos + 1, output, output_l, i);
        rec_input_tower(list, x, y, n, s, o, e, pos + 1, str1, output_l, i);
    }
    else if (pos < o + n + e + s)
    {
        uint64 str1 = add_byte(output, list[(y - (pos - n - e - o + 1)) * 8 + x]);
        rec_input_tower(list, x, y, n, s, o, e, pos + 1, output, output_l, i);
        rec_input_tower(list, x, y, n, s, o, e, pos + 1, str1, output_l, i);
    }
    else
        output_l[i++] = output;
}

constexpr void return_tab(array_array& ret)
{
    auto list1 = generate_list();

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            int nmax = 7 - y;
            if (nmax != 0) --nmax;

            int smax = y;
            if (smax != 0) --smax;

            int omax = 7 - x;
            if (omax != 0) --omax;

            int emax = x;
            if (emax != 0) --emax;

            int i = 0;
            rec_input_tower(list1, x, y, nmax, smax, omax, emax, 0, 0ull, ret[y * 8 + x + 1], i);

            while (i != 4096)
            {
                ret[y * 8 + x + 1][i] = -1ull;
                ++i;
            }
        }
    }
}

/* end Romain */




constexpr uint64 add_byte(uint64 b, uint64 param) {
    return b | param;
}

constexpr void rec_input_bishop(const std::array<uint64, 64>& list, int x, int y, int no,
                      int ne, int so, int se, int pos, uint64 output, array_optional& output_l, int& i)
{
    if (pos < so)
    {
        uint64 str1 = add_byte(output, list[(y - pos - 1) * 8 + x + pos + 1]);
        rec_input_bishop(list, x, y, no, ne, so, se, pos + 1, output, output_l, i);
        rec_input_bishop(list, x, y, no, ne, so, se, pos + 1, str1, output_l, i);

    }
    else if (pos < so + no)
    {
        uint64 str1 = add_byte(output, list[(y + pos - so + 1) * 8 + x + pos - so + 1]);
        rec_input_bishop(list, x, y, no, ne, so, se, pos + 1, output, output_l, i);
        rec_input_bishop(list, x, y, no, ne, so, se, pos + 1, str1, output_l, i);
    }
    else if (pos < so + no + se)
    {
        uint64 str1 = add_byte(output, list[(y - pos + so + no - 1) * 8 + x - (pos - no - so + 1)]);
        rec_input_bishop(list, x, y, no, ne, so, se, pos + 1, output, output_l, i);
        rec_input_bishop(list, x, y, no, ne, so, se, pos + 1, str1, output_l, i);
    }
    else if (pos < so + no + se + ne)
    {
        uint64 str1 = add_byte(output, list[(y + (pos - no - se - so + 1)) * 8 + x - pos + no + se + so -1]);
        rec_input_bishop(list, x, y, no, ne, so, se, pos + 1, output, output_l, i);
        rec_input_bishop(list, x, y, no, ne, so, se, pos + 1, str1, output_l, i);
    }
    else
        output_l[i++] = output;
}

constexpr void return_tab_bishop(array_array& ret)
{
    auto list1 = generate_list();

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            int nomax = std::min(7 - y, 7 - x);
            if (nomax != 0) --nomax;

            int nemax = std::min(7 - y, x);
            if (nemax != 0) --nemax;

            int somax =std::min(7 - x, y);
            if (somax != 0) --somax;

            int semax = std::min(x,y);
            if (semax != 0) --semax;

            int i = 0;
            rec_input_bishop(list1, x, y, nomax, nemax, somax, semax, 0, 0ull, ret[y * 8 + x + 1], i);

            while (i != 4096)
            {
                ret[y * 8 + x + 1][i] = -1ull;
                ++i;
            }
        }
    }
}

constexpr bool is_occupied(uint64 pos, uint64 index, std::array<uint64, 64>& list)
{
    return (pos & list[index]) == list[index];
}

constexpr uint64 find_int_tower_bis(uint64 x,uint64 y,std::array<uint64, 64>& list,uint64 n,uint64 s,uint64 o,uint64 e)
{
    uint64 b = 0;
    uint64 x1 = 0;

    while (x1 < o) {
        b = add_byte(b, list[y * 8 + x1 + x + 1]);
        x1 += 1;
    }

    x1 = 0;
    while (x1 < e) {
        b = add_byte(b, list[y * 8 - x1 + x - 1]);
        x1 += 1;
    }


    uint64 y1 = 0;

    while (y1 < n) {
        b = add_byte(b, list[(y1 + y + 1) * 8 + x]);
        y1 += 1;
    }
    y1 = 0;
    while (y1 < s) {
        b = add_byte(b, list[(y - y1 - 1) * 8 + x]);
        y1 += 1;
    }
    return b;
}

constexpr uint64 input_to_output(uint64 pos, int index) {
    auto list = generate_list();
    index = index - 1;
    int x = index % 8;
    int y = index / 8;
    int n = 0;
    while (0 <= y + n + 1 && y + n + 1 < 8 && !is_occupied(pos, (y + n + 1) * 8 + x, list))
    {
        n += 1;
    }
    if (0 <= y + n + 1 && y + n + 1 < 8 && is_occupied(pos, (y + n + 1) * 8 + x, list))
        n += 1;
    int s = 0;
    while (0 <= y - s - 1 && y - s - 1  < 8 && !is_occupied(pos, (y - s - 1) * 8 + x, list))
        s += 1;
    if (0 <= y - s - 1 && y - s - 1  < 8 && is_occupied(pos, (y - s - 1) * 8 + x, list))
        s += 1;
    int o = 0;
    while (0 <= x + o + 1 && x + o + 1 < 8 && !is_occupied(pos, y * 8 + x + o + 1, list))
        o += 1;
    if (0 <= x + o + 1 && x + o + 1 < 8 && is_occupied(pos, y * 8 + x + o + 1, list))
        o += 1;
    int e = 0;
    while (0 <= x - e - 1 && x - e - 1 < 8 && !is_occupied(pos, y * 8 + x - e - 1, list))
        e += 1;
    if (0 <= x - e - 1 && x - e - 1 < 8 && is_occupied(pos, y * 8 + x - e - 1, list))
        e += 1;
    return find_int_tower_bis(x, y, list, n, s, o, e);
}


constexpr bool generate_new_ouput(uint64 magique, array_array& output, const array_array& input, int pos)
{
    const auto& start = input[pos];
    auto& ret = output[pos];
    for (int i = 0; i < 4096;i++)
        ret[i] = -1ull;
    for (int j = 0; j < 4096 && start[j] != -1ull ;j++)
    {
        uint64 index = (start[j] * magique) >> 52ull;
        uint64 final_output = input_to_output(start[j], pos);
        if (ret[index] == -1ull)
            ret[index] = final_output;
        else if (ret[index] != final_output)
        {
            return false;
        }
    }
    return true;
}


constexpr uint64 input_to_output_bishop(uint64 pos, int index) {
    auto list = generate_list();
    index = index - 1;
    int x = index % 8;
    int y = index / 8;
    int xbis = 1;
    int ybis = 1;
    uint64 b = 0;

    while ((0 <= y + ybis && y + ybis < 8) && (0 <= x + xbis && x + xbis < 8) && !is_occupied(pos, (y + ybis) * 8 + x + xbis, list))
    {
        b = add_byte(b, list[(y + ybis) * 8 + x + xbis]);
        ybis += 1;
        xbis += 1;
    }
    if (0 <= y + ybis && y + ybis < 8 && 0 <= x + xbis && x + xbis < 8 && is_occupied(pos, (y + ybis) * 8 + x + xbis, list))
    {
        b = add_byte(b, list[(y + ybis) * 8 + x + xbis]);
    }

    xbis = 1;
    ybis = 1;
    while (0 <= y - ybis && y - ybis < 8 && 0 <= x + xbis && x + xbis < 8 && !is_occupied(pos, (y - ybis) * 8 + x + xbis, list))
    {
        b = add_byte(b, list[(y - ybis) * 8 + x + xbis]);
        ybis += 1;
        xbis += 1;
    }
    if (0 <= y - ybis && y - ybis < 8 && 0 <= x + xbis && x + xbis < 8 && is_occupied(pos, (y - ybis) * 8 + x + xbis, list))
    {
        b = add_byte(b, list[(y - ybis) * 8 + x + xbis]);
    }

    xbis= 1;
    ybis = 1;
    while (0 <= y + ybis && y + ybis < 8 && 0 <= x - xbis && x - xbis < 8 && !is_occupied(pos, (y + ybis) * 8 + x - xbis, list))
    {
        b = add_byte(b, list[(y + ybis) * 8 + x - xbis]);
        ybis += 1;
        xbis += 1;
    }
    if (0 <= y + ybis && y + ybis < 8 && 0 <= x - xbis && x - xbis < 8 && is_occupied(pos, (y + ybis) * 8 + x - xbis, list))
    {
        b = add_byte(b, list[(y + ybis) * 8 + x - xbis]);
    }


    xbis= 1;
    ybis = 1;
    while (0 <= y - ybis && y - ybis < 8 && 0 <= x - xbis && x - xbis < 8 && !is_occupied(pos, (y - ybis) * 8 + x - xbis, list))
    {
        b = add_byte(b, list[(y - ybis) * 8 + x - xbis]);
        ybis += 1;
        xbis += 1;
    }
    if (0 <= y - ybis && y - ybis < 8 && 0 <= x - xbis && x - xbis < 8 && is_occupied(pos, (y - ybis) * 8 + x - xbis, list))
    {
        b = add_byte(b, list[(y - ybis) * 8 + x - xbis]);
    }
    return b;
}

constexpr bool generate_new_ouput_bishop(uint64 magique, array_array& output, const array_array& input, int pos)
{
    const auto& start = input[pos];
    auto& ret = output[pos];
    for (int i = 0; i < 4096; ++i)
        ret[i] = -1ull;
    for (int j = 0; j < 4096 && start[j] != -1ull ; ++j)
    {
        uint64 index = (start[j] * magique) >> 52ull;
        uint64 final_output = input_to_output_bishop(start[j], pos);
        if (ret[index] == -1ull)
            ret[index] = final_output;
        else if (ret[index] != final_output)
        {
            return false;
        }
    }
    return true;
}