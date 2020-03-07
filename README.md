                                                        PERCEVAL

WHAT IS IMPLMENTED ?


Board Representation

    Bitboards with Little-Endian Rank-File Mapping (LERF)
    Magic Bitboards

Search

    Iterative Deepening
    Principal Variation Search
    Transposition Table
        Depth-preferred Replacement Strategy
    Move Ordering
        PV
        Hash
        Capture
        Other
    Selectivity
        Quiescence Search
            None 
            
Evaluation
    
    Default values pieces
    Default values positions
    

TODO

Board Representation

    Piece-Lists [22]

Search

    Aspiration Windows
    Parallel Search using Threads
        Lazy SMP
    Principal Variation Search
    Transposition Table
        Shared Hash Table
        10 Bytes per Entry, 3 Entries per Cluster
        Depth-preferred Replacement Strategy
    Move Ordering
        Countermove Heuristic
        Counter Moves History
        History Heuristic
        Internal Iterative Deepening
        Killer Heuristic
        MVV/LVA
        SEE
    Selectivity
        Extensions
            Check Extensions if SEE >= 0
        Pruning
            Futility Pruning
            Move Count Based Pruning
            Null Move Pruning
                Dynamic Depth Reduction based on depth and value
                Static Null Move Pruning
                Verification search at high depths
            ProbCut
            SEE Pruning
        Quiescence Search
            King is in check
            SEE

Evaluation

    Tapered Eval
    Material
        Point Values
            Midgame: 198, 817, 836, 1270, 2521
            Endgame: 258, 846, 857, 1278, 2558
        Bishop Pair
    Pawn Structure
    King Safety

Data Base

    Syzygy
    Openning
