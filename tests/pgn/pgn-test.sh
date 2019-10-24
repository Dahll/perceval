#!/bin/sh

if [ -z "$CHESS_BIN" ]; then
    echo "CHESS_BIN is not exported. Exiting."
    exit
else
    echo "PGN tests using:"
    echo "$CHESS_BIN"
fi

pgn () {
    $CHESS_BIN --pgn "$1".pgn -l ./libbasic-output.so > "$1".output
    DIFF=$(diff $1.good $1.output)
    if [ "$DIFF" = "" ] ; then
        printf "$1 \e[1;32mOK\e[0m\n"
    else
        printf "$1 \e[1;31mKO\e[0m\n"
    fi

}

for file in $(find . -name "*pgn"); do
    pgn "${file%.pgn}"
done
