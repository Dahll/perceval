# perceval

TODO : Rewrite and refactor the search of the IA.

The code need to be as close as possible of the pseudo code + add opening window
----------------------------------------------------------------------------------------------------------------------------

function negamax(node, depth, α, β, color) is
    alphaOrig := α
    (* Transposition Table Lookup; node is the lookup key for ttEntry *)
    ttEntry := transpositionTableLookup(node)
    if ttEntry is valid and ttEntry.depth ≥ depth then
        if ttEntry.flag = EXACT then
            return ttEntry.value
        else if ttEntry.flag = LOWERBOUND then
            α := max(α, ttEntry.value)
        else if ttEntry.flag = UPPERBOUND then
            β := min(β, ttEntry.value)
        if α ≥ β then
            return ttEntry.value
    if depth = 0 or node is a terminal node then
        return color × the heuristic value of node
    childNodes := generateMoves(node)
    childNodes := orderMoves(childNodes)
    value := −∞
    for each child in childNodes do
        value := max(value, −negamax(child, depth − 1, −β, −α, −color))
        α := max(α, value)
        if α ≥ β then
            break
    (* Transposition Table Store; node is the lookup key for ttEntry *)
    ttEntry.value := value
    if value ≤ alphaOrig then
        ttEntry.flag := UPPERBOUND
    else if value ≥ β then
        ttEntry.flag := LOWERBOUND
    else
        ttEntry.flag := EXACT
    ttEntry.depth := depth	
    transpositionTableStore(node, ttEntry)
    return value
    
--------------------------------------------------------------------------------------------------------------------

- No color management in search.
- If possible not calling function for the search

