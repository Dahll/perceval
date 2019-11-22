//
// Created by adrien on 20/11/19.
//

#pragma once

namespace ai
{
    class Metadata
    {
    public:
        int hash = 0;
        int wtime = 0;
        int btime = 0;
        int winc = 0;
        int binc = 0;
        int running = false;
    };
    
    extern Metadata meta;
}
