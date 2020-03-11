#include "pgn-move.hh"

namespace yaka
{
    PgnMove::PgnMove(const Position& start, const Position& end,
                     PieceType piece, bool capture, ReportType report,
                     const PgnMove::opt_piece_t& promotion)
        : start_(start)
        , end_(end)
        , piece_(piece)
        , promotion_(promotion)
        , capture_(capture)
        , report_(report)
    {}

    PgnMove PgnMove::generate_castling(bool queen_side, Color color)
    {
        static const Position wking_pos{File::E, Rank::ONE};
        static const Position bking_pos{File::E, Rank::EIGHT};
        static const PgnMove w_small{wking_pos,
                                     {File::G, Rank::ONE},
                                     PieceType::KING,
                                     false,
                                     ReportType::NONE};
        static const PgnMove w_big{wking_pos,
                                   {File::C, Rank::ONE},
                                   PieceType::KING,
                                   false,
                                   ReportType::NONE};
        static const PgnMove b_small{bking_pos,
                                     {File::G, Rank::EIGHT},
                                     PieceType::KING,
                                     false,
                                     ReportType::NONE};
        static const PgnMove b_big{bking_pos,
                                   {File::C, Rank::EIGHT},
                                   PieceType::KING,
                                   false,
                                   ReportType::NONE};

        if (color == Color::WHITE)
            return queen_side ? w_big : w_small;

        return queen_side ? b_big : b_small;
    }

    bool PgnMove::is_castling() const
    {
        if (piece_ != PieceType::KING)
            return false;
        return start_.file_get() == File::E
            && (end_.file_get() == File::C || end_.file_get() == File::G);
    }

    bool PgnMove::is_castling_queenside() const
    {
        return end_.file_get() == File::C;
    }

    const Position& PgnMove::start_get() const
    {
        return start_;
    }

    const Position& PgnMove::end_get() const
    {
        return end_;
    }

    PieceType PgnMove::piece_get() const
    {
        return piece_;
    }

    const PgnMove::opt_piece_t& PgnMove::promotion_get() const
    {
        return promotion_;
    }

    bool PgnMove::capture_get() const
    {
        return capture_;
    }

    ReportType PgnMove::report_get() const
    {
        return report_;
    }

    void PgnMove::report_set(ReportType report)
    {
        report_ = report;
    }

    bool PgnMove::operator==(const PgnMove& other) const
    {
        return start_ == other.start_ && end_ == other.end_
            && piece_ == other.piece_ && promotion_ == other.promotion_;
    }

    bool PgnMove::operator!=(const PgnMove& other) const
    {
        return !(*this == other);
    }

} // namespace board
