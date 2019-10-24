#!/bin/sh

if [ -z "$CHESS_BIN" ]; then
    echo "CHESS_BIN is not exported. Exiting."
    exit
else
    echo "PGN tests using:"
    echo "$CHESS_BIN"
fi

perft () {
    printf "$1 "
    echo "$1" > tmp
    result=$(time $CHESS_BIN --perft tmp)
    if [ "$result" = "$2" ] ; then
        printf "\e[1;32mOK\e[0m ($2)\n"
    else
        printf "\e[1;31mKO got $result expected $2\e[0m\n"
    fi

}


# Basic board
perft "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 6" "119060324"
perft "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 0" "1"
perft "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 1" "20"
perft "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 2" "400"
perft "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 3" "8902"
perft "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 4" "197281"
perft "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 5" "4865609"
#perft "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 7" "3195901860"
#perft "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 8" "84998978956"
#perft "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 9" "2439530234167"


# Given files
perft "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 1 1 1" "48"
perft "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 1 1" "14"
perft "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1 3" "9467"
perft "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 3" "62379"
perft "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 3" "89890"
perft "rnbqkbnr/ppp1pppp/8/3p4/8/2P5/PP1PPPPP/RNBQKBNR w KQkq - 0 2 3" "13407"
perft "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1 4" "314346"

# Custom

perft "B4Qn1/8/4Pp2/B1P2k2/8/7N/PPP2PPP/RN2K2R w KQ - 1 20 1" "47"
perft "B4Qn1/8/4Pp2/B1P2k2/8/7N/PPP2PPP/RN2K2R w KQ - 1 20 2" "256"
perft "B4Qn1/8/4Pp2/B1P2k2/8/7N/PPP2PPP/RN2K2R w KQ - 1 20 3" "12063"

perft "B4Qn1/8/4Ppk1/B1P5/8/7N/PPP2PPP/RN2K2R b KQ - 0 19 1" "6"
perft "B4Qn1/8/4Ppk1/B1P5/8/7N/PPP2PPP/RN2K2R b KQ - 0 19 2" "283"
perft "B4Qn1/8/4Ppk1/B1P5/8/7N/PPP2PPP/RN2K2R b KQ - 0 19 3" "1718"
perft "B4Qn1/8/4Ppk1/B1P5/8/7N/PPP2PPP/RN2K2R b KQ - 0 19 4" "81724"


perft "B5Q1/8/4Pp2/B1P2k2/8/7N/PPP2PPP/RN2K2R b KQ - 0 20 1" 	"1"
perft "B5Q1/8/4Pp2/B1P2k2/8/7N/PPP2PPP/RN2K2R b KQ - 0 20 2" 	"49"
perft "B5Q1/8/4Pp2/B1P2k2/8/7N/PPP2PPP/RN2K2R b KQ - 0 20 3" 	"149"
perft "B5Q1/8/4Pp2/B1P2k2/8/7N/PPP2PPP/RN2K2R b KQ - 0 20 4"	"7335"
perft "B5Q1/8/4Pp2/B1P2k2/8/7N/PPP2PPP/RN2K2R b KQ - 0 20 5"	"21809"
perft "B5Q1/8/4Pp2/B1P2k2/8/7N/PPP2PPP/RN2K2R b KQ - 0 20 6"	"1061489"

#p
perft "r1b1kb1r/pppp1ppp/2n1p3/1Q1n4/6Pq/2P2P1N/PP1PP2P/RNBK1B1R b kq - 0 0 1" "45"
perft "r1b1kb1r/pppp1ppp/2n1p3/1Q1n4/6Pq/2P2P1N/PP1PP2P/RNBK1B1R b kq - 0 0 2" "1332"
perft "r1b1kb1r/pppp1ppp/2n1p3/1Q1n4/6Pq/2P2P1N/PP1PP2P/RNBK1B1R b kq - 0 0 3" "56557"
perft "r1b1kb1r/pppp1ppp/2n1p3/1Q1n4/6Pq/2P2P1N/PP1PP2P/RNBK1B1R b kq - 0 0 4" "1716987"
#perft "r1b1kb1r/pppp1ppp/2n1p3/1Q1n4/6Pq/2P2P1N/PP1PP2P/RNBK1B1R b kq - 0 0 5" "70505448"

perft "r1b1kb1r/pppp1ppp/2n5/1Q1n4/4p1Pq/2P2P1N/PP1PP1BP/RNBK3R b kq - 0 0 1" "46"
perft "r1b1kb1r/pppp1ppp/2n5/1Q1n4/4p1Pq/2P2P1N/PP1PP1BP/RNBK3R b kq - 0 0 2" "1450"
perft "r1b1kb1r/pppp1ppp/2n5/1Q1n4/4p1Pq/2P2P1N/PP1PP1BP/RNBK3R b kq - 0 0 3" "62937"
perft "r1b1kb1r/pppp1ppp/2n5/1Q1n4/4p1Pq/2P2P1N/PP1PP1BP/RNBK3R b kq - 0 0 4" "2013973"


perft "r1b1kb1r/pppp1ppp/2n5/1Q1n4/4p1Pq/2P2P1N/PP1PP1BP/RNBK3R w kq - 0 0 1" "33"
perft "r1b1kb1r/pppp1ppp/2n5/1Q1n4/4p1Pq/2P2P1N/PP1PP1BP/RNBK3R w kq - 0 0 2" "1503"
perft "r1b1kb1r/pppp1ppp/2n5/1Q1n4/4p1Pq/2P2P1N/PP1PP1BP/RNBK3R w kq - 0 0 3" "47531"

# test promotions
perft "k7/6p1/8/8/8/8/6p1/K7 w - - 1" "3"
perft "k7/6p1/8/8/8/8/6p1/K7 w - - 2" "27"
perft "k7/6p1/8/8/8/8/6p1/K7 w - - 3" "148"
perft "k7/6p1/8/8/8/8/6p1/K7 w - - 4" "1747"




perft "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP2PPP/R3QKR1 b - - 0 0 1" "46"

perft "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 0 2" "2039"



perft "r2qk1nr/ppp2ppp/2nbb3/3p4/4pP2/2P1P1PP/PP1P3R/RNB1KBN1 w Qkq - 2 9 1" "26"
perft "r2qk1nr/ppp2ppp/2nbb3/3p4/4pP2/2P1P1PP/PP1P3R/RNB1KBN1 w Qkq - 2 9 2" "1137"
perft "r2qk1nr/ppp2ppp/2nbb3/3p4/4pP2/2P1P1PP/PP1P3R/RNB1KBN1 w Qkq - 2 9 3" "28790"
perft "r2qk1nr/ppp2ppp/2nbb3/3p4/4pP2/2P1P1PP/PP1P3R/RNB1KBN1 w Qkq - 2 9 4" "1224053"

perft "r2q1rk1/ppp1bppp/4bn2/1B1pnP2/8/1PPQP1PP/P7/RNB1K1NR w KQ - 1 11 1"  "36"
perft "r2q1rk1/ppp1bppp/4bn2/1B1pnP2/8/1PPQP1PP/P7/RNB1K1NR w KQ - 1 11 2"  "1373"
perft "r2q1rk1/ppp1bppp/4bn2/1B1pnP2/8/1PPQP1PP/P7/RNB1K1NR w KQ - 1 11 3"	"48030"
perft "r2q1rk1/ppp1bppp/4bn2/1B1pnP2/8/1PPQP1PP/P7/RNB1K1NR w KQ - 1 11 4"	"1847640"



perft "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2 1" "30"
perft "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2 2" "652"
perft "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2 3"	"20035"
perft "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2 4" "482325"
perft "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2 5" "15265123"

perft "rnbqkbnr/pp1ppppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 2 1" "23"
perft "rnbqkbnr/pp1ppppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 2 2" "664"
perft "rnbqkbnr/pp1ppppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 2 3" "16611"
perft "rnbqkbnr/pp1ppppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 2 4" "485601"
perft "rnbqkbnr/pp1ppppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 2 5" "13397752"





perft "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 1" "48"
perft "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 2" "2039"
perft "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 3" "97862"
perft "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 4" "4085603"
#perft "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 5" "193690690"
#perft "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 6" "8031647685"

perft "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 1" "14"
perft "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 2" "191"
perft "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 3" "2812"
perft "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 4" "43238"
#perft "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 5" "674624"
#perft "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 6" "11030083"
#perft "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 7" "178633661"
#perft "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 8" "3009794393"

perft "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1 1" "6"
perft "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1 2" "264"
perft "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1 3" "9467"
perft "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1 4" "422333"
#perft "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1 5" "15833292"
#perft "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1 6" "706045033"

perft "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 1" "6"
perft "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 2" "264"
perft "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 3" "9467"
perft "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 4" "422333"
#perft "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 5" "15833292"
#perft "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 6" "706045033"

perft "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 1" "44"
perft "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 2" "1486"
perft "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 3" "62379"
perft "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 4" "2103487"
#perft "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8 5" "89941194"

perft "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 1" "46"
perft "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 2" "2079"
perft "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 3" "89890"
perft "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 4" "3894594"
#perft "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1 5" "15833292"
#perft "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1 6" "706045033"

perft "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1 5" "15833292"
#perft "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1 6" "706045033"

#perft "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 5" "164,075,551"
#perft "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 6" "6,923,051,137"
#perft "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 7" "287,188,994,746"
#perft "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 8" "11,923,589,843,526"
