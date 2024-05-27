// Lib standard
#include <array>
#include <algorithm>
#include <cassert>
#include <utility>
#include <functional>
#include <cmath>
#include <optional>
#include <stdexcept>

// Inclusion projet
#include <Piece.hpp>
#include <CMatrix.hpp>
#include <Unitary.hpp>
#include <TypePiece.hpp>
#include <math_utility.hpp>
#include <Constexpr.hpp>
#include <Move.hpp>
#include "Board.hpp"

template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::king_side_castle(Coord const &k,
                                             Coord const &r,
                                             std::optional<bool> val_mes)
{
    std::size_t king = offset(k.n, k.m);
    std::size_t new_king = offset(k.n, k.m + 2);
    std::size_t rook = offset(r.n, r.m);
    std::size_t new_rook = offset(r.n, r.m - 2);
    if (mesure_castle(k, r, val_mes))
    {
        for (std::size_t i{0}; i < std::size(m_board); i++)
        {
            move_1_instance(
                std::array<bool, 2>{true, false}, i,
                MATRIX_ISWAP,
                std::array<std::size_t, 2>{king, new_king});
            move_1_instance(
                std::array<bool, 2>{true, false}, i,
                MATRIX_ISWAP,
                std::array<std::size_t, 2>{rook, new_rook});
        }
        m_piece_board[new_king] = std::move(m_piece_board[king]);
        m_piece_board[new_rook] = std::move(m_piece_board[rook]);
        m_piece_board[king] = Piece();
        m_piece_board[rook] = Piece();
    }
}
template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::queen_side_castle(Coord const &k,
                                              Coord const &r,
                                              std::optional<bool> val_mes)
{
    std::size_t king = offset(k.n, k.m);
    std::size_t new_king = offset(k.n, k.m - 2);
    std::size_t rook = offset(r.n, r.m);
    std::size_t new_rook = offset(r.n, r.m + 3);
    if (mesure_castle(k, r, val_mes))
    {
        for (std::size_t i{0}; i < std::size(m_board); i++)
        {
            move_1_instance(
                std::array<bool, 2>{true, false}, i,
                MATRIX_ISWAP,
                std::array<std::size_t, 2>{king, new_king});
            move_1_instance(
                std::array<bool, 2>{true, false}, i,
                MATRIX_ISWAP,
                std::array<std::size_t, 2>{rook, new_rook});
        }
        m_piece_board[new_king] = std::move(m_piece_board[king]);
        m_piece_board[new_rook] = std::move(m_piece_board[rook]);
        m_piece_board[king] = Piece();
        m_piece_board[rook] = Piece();
    }
}
template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::move_classic_jump(Coord const &s, Coord const &t,
                               std::optional<bool> val_mes)
{
    std::size_t source = offset(s.n, s.m);
    std::size_t target = offset(t.n, t.m);

    if (m_piece_board[target].get_type() == TypePiece::EMPTY ||
        (m_piece_board[target].get_type() == m_piece_board[source].get_type() &&
         m_piece_board[target].get_color() == m_piece_board[source].get_color()))
    {
        std::size_t const size_board{std::size(m_board)};
        for (std::size_t i{0}; i < size_board; i++)
        {
            move_1_instance(
                std::array<bool, 2>{m_board[i].first[source], false}, i,
                MATRIX_ISWAP,
                std::array<std::size_t, 2>{source, target});
        }
        m_piece_board[target] = std::move(m_piece_board[source]);
        m_piece_board[source] = Piece();
    }
    else
    {
        if (m_piece_board[source].same_color(m_piece_board[target]))
        {
            std::optional<bool> negation ;
            if(val_mes.has_value())
            {
                negation = !val_mes.value();
            }
            if (!mesure(t, negation))
            {
                for (std::size_t i{0}; i < std::size(m_board); i++)
                {
                    move_1_instance(
                        std::array<bool, 2>{m_board[i].first[source], false},
                        i, MATRIX_ISWAP,
                        std::array<std::size_t, 2>{source, target});
                }
                m_piece_board[target] = std::move(m_piece_board[source]);
                m_piece_board[source] = Piece();
            }
        }
        else
        {
            if (mesure(s, val_mes))
            {
                for (std::size_t i{0}; i < std::size(m_board); i++)
                {
                    CONSTEXPR CMatrix<8> A{
                        MATRIX_JUMP
                            .tensoriel_product(
                                CMatrix<2>::identity()) *
                        CMatrix<2>::identity()
                            .tensoriel_product(MATRIX_JUMP)};

                    move_1_instance(
                        std::array<bool, 3>{m_board[i].first[source],
                                            m_board[i].first[target],
                                            false},
                        i,
                        A,
                        std::array<std::size_t, 3>{source,
                                                   target,
                                                   N * M + 1});
                }
                m_piece_board[target] = std::move(m_piece_board[source]);
                m_piece_board[source] = Piece();
            }
        }
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR bool
Board<N, M>::move_pawn_one_step(Coord const &s, Coord const &t,
                                std::optional<bool> val_mes)
{
    std::size_t source = offset(s.n, s.m);
    std::size_t target = offset(t.n, t.m);

    if (m_piece_board[target].get_type() == TypePiece::EMPTY ||
        (m_piece_board[target].get_type() == m_piece_board[source].get_type() &&
         m_piece_board[target].get_color() == m_piece_board[source].get_color()))
    {
        std::size_t const size_board{std::size(m_board)};
        for (std::size_t i{0}; i < size_board; i++)
        {
            move_1_instance(
                std::array<bool, 2>{m_board[i].first[source], false},
                i, MATRIX_ISWAP,
                std::array<std::size_t, 2>{source, target});
        }
        m_piece_board[target] = std::move(m_piece_board[source]);
        m_piece_board[source] = Piece();
        return true;
    }
    else
    {
        std::optional<bool> negation ;
            if(val_mes.has_value())
            {
                negation = !val_mes.value();
            }
        if (!mesure(t, negation))
        {
            for (std::size_t i{0}; i < std::size(m_board); i++)
            {
                move_1_instance(
                    std::array<bool, 2>{m_board[i].first[source], false},
                    i, MATRIX_ISWAP,
                    std::array<std::size_t, 2>{source, target});
            }
            m_piece_board[target] = std::move(m_piece_board[source]);
            m_piece_board[source] = Piece();
            return true;
        }
    }
    return false;
}

template <std::size_t N, std::size_t M>
CONSTEXPR bool
Board<N, M>::move_pawn_two_step(Coord const &s, Coord const &t,
                                std::optional<bool> val_mes)

{
    std::size_t source = offset(s.n, s.m);
    std::size_t target = offset(t.n, t.m);
    if (m_piece_board[target].get_type() == TypePiece::EMPTY ||
        (m_piece_board[target].get_type() == m_piece_board[source].get_type() &&
         m_piece_board[target].get_color() == m_piece_board[source].get_color()))
    {
        std::size_t const size_board{std::size(m_board)};
        for (std::size_t i{0}; i < size_board; i++)
        {
            move_1_instance(
                std::array<bool, 3>{!check_path_straight_1_instance(
                                        *this, s, t, i, std::nullopt),
                                    false,
                                    m_board[i].first[source]},
                i, MATRIX_SLIDE,
                std::array<std::size_t, 3>{N * M + 1, target, source});
        }
        m_piece_board[target] = m_piece_board[source];
        update_case(source);
        return true;
    }
    else
    {
        std::optional<bool> negation ;
            if(val_mes.has_value())
            {
                negation = !val_mes.value();
            }
        if (!mesure(t, negation))
        {
            for (std::size_t i{0}; i < std::size(m_board); i++)
            {
                move_1_instance(
                    std::array<bool, 3>{!check_path_straight_1_instance(
                                            *this, s, t, i, std::nullopt),
                                        false,
                                        m_board[i].first[source]},
                    i, MATRIX_SLIDE,
                    std::array<std::size_t, 3>{N * M + 1, target, source});
            }
            m_piece_board[target] = std::move(m_piece_board[source]);
            update_case(source);
            return true;
        }
    }
    return false;
}

template <std::size_t N, std::size_t M>
CONSTEXPR bool Board<N, M>::capture_pawn(
    Coord const &s,
    Coord const &t,
    std::optional<bool> val_mes)
{
    std::size_t source = offset(s.n, s.m);
    std::size_t target = offset(t.n, t.m);
    if (mesure(s, val_mes))
    {
        for (std::size_t i{0}; i < std::size(m_board); i++)
        {
            if (m_board[i].first[target])
            {
                CONSTEXPR CMatrix<8> A{
                    MATRIX_JUMP
                        .tensoriel_product(
                            CMatrix<2>::identity()) *
                    CMatrix<2>::identity()
                        .tensoriel_product(MATRIX_JUMP)};

                move_1_instance(
                    std::array<bool, 3>{
                        m_board[i].first[source],
                        m_board[i].first[target],
                        false},
                    i, A,
                    std::array<std::size_t, 3>{source, target, N * M + 1});
            }
        }
        m_piece_board[target] = m_piece_board[source];
        update_case(source);
        return true;
    }
    return false;
}

template <std::size_t N, std::size_t M>
CONSTEXPR bool
Board<N, M>::move_enpassant(Coord const &s, Coord const &t, Coord const &ep,
                            std::optional<bool> val_mes)
{
    std::size_t source = offset(s.n, s.m);
    std::size_t target = offset(t.n, t.m);
    std::size_t enpassant = offset(ep.n, ep.m);

    if (m_piece_board[target].get_type() == TypePiece::EMPTY ||
        (m_piece_board[target].get_type() == m_piece_board[source].get_type() &&
         m_piece_board[target].get_color() == m_piece_board[source].get_color()))
    {

        std::size_t const size_board{std::size(m_board)};
        for (std::size_t i{0}; i < size_board; i++)
        {
            if (m_board[i].first[enpassant])
            {
                // permet d'enlever le pion pris en passant
                move_1_instance(
                    std::array<bool, 2>{m_board[i].first[enpassant], false},
                    i, MATRIX_ISWAP,
                    std::array<std::size_t, 2>{enpassant, N * M + 1});
                move_1_instance(
                    std::array<bool, 2>{m_board[i].first[source], false},
                    i, MATRIX_ISWAP,
                    std::array<std::size_t, 2>{source, target});
            }
        }
        m_piece_board[target] = std::move(m_piece_board[source]);
        m_piece_board[source] = Piece();
        m_piece_board[enpassant] = Piece();
        return true;
    }
    else
    {
        if (m_piece_board[source].same_color(m_piece_board[target]))
        {
            std::optional<bool> negation ;
            if(val_mes.has_value())
            {
                negation = !val_mes.value();
            }
            if (!mesure(t, negation))
            {

                for (std::size_t i{0}; i < std::size(m_board); i++)
                {
                    if (m_board[i].first[enpassant])
                    {
                        // permet d'enlever le pion pris en passant
                        move_1_instance(
                            std::array<bool, 2>{
                                m_board[i].first[enpassant],
                                false},
                            i, MATRIX_ISWAP,
                            std::array<std::size_t, 2>{enpassant, N * M + 1});

                        move_1_instance(
                            std::array<bool, 2>{
                                m_board[i].first[source],
                                false},
                            i, MATRIX_ISWAP,
                            std::array<std::size_t, 2>{source, target});
                    }
                }
                m_piece_board[target] = m_piece_board[source];
                m_piece_board[source] = Piece();
                return true;
            }
            return false;
        }
        else
        {
            if (mesure(s, val_mes))
            {
                for (std::size_t i{0}; i < std::size(m_board); i++)
                {
                    if (m_board[i].first[enpassant] ||
                        m_board[i].first[target])
                    {
                        // permet d'enlever le pion pris en passant
                        move_1_instance(
                            std::array<bool, 2>{
                                m_board[i].first[enpassant], false},
                            i, MATRIX_ISWAP,
                            std::array<std::size_t, 2>{
                                enpassant, N * M + 1});
                        // ces deux instructions représentent un mouvement
                        // de capture jump
                        move_1_instance(
                            std::array<bool, 2>{
                                m_board[i].first[target], false},
                            i, MATRIX_ISWAP,
                            std::array<std::size_t, 2>{
                                target, N * M + 1});

                        move_1_instance(
                            std::array<bool, 2>{
                                m_board[i].first[source], false},
                            i, MATRIX_ISWAP,
                            std::array<std::size_t, 2>{
                                source, target});
                    }
                }
                m_piece_board[target] = std::move(m_piece_board[source]);
                m_piece_board[source] = Piece();
                m_piece_board[enpassant] = Piece();
                return true;
            }
        }
    }
    return false;
}

template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::move_split_jump(Coord const &s, Coord const &t1, Coord const &t2)
{
    std::size_t const source = offset(s.n, s.m);
    std::size_t const target1 = offset(t1.n, t1.m);
    std::size_t const target2 = offset(t2.n, t2.m);
    std::size_t const size_board{std::size(m_board)};
    for (std::size_t i{0}; i < size_board; i++)
    {
        move_1_instance(
            std::array<bool, 3>{
                m_board[i].first[target2],
                m_board[i].first[target1],
                m_board[i].first[source]},
            i, MATRIX_SPLIT,
            std::array<std::size_t, 3>{target2, target1, source});
    }
    m_piece_board[target1] = m_piece_board[source];

    if (m_piece_board[target1].get_type() == TypePiece::EMPTY &&
        m_piece_board[target2].get_type() == TypePiece::EMPTY)
    {
        m_piece_board[target2] = std::move(m_piece_board[source]);
        m_piece_board[source] = Piece();
        update_case(target1);
        update_case(target2);
    }
    else
    {
        m_piece_board[target2] = m_piece_board[source];
        update_case(source);
        update_case(target1);
        update_case(target2);
        update_board();
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::move_merge_jump(Coord const &s1, Coord const &s2, Coord const &t)
{
    std::size_t source1 = offset(s1.n, s1.m);
    std::size_t source2 = offset(s2.n, s2.m);
    std::size_t target = offset(t.n, t.m);
    std::size_t const size_board{std::size(m_board)};
    for (std::size_t i{0}; i < size_board; i++)
    {
        move_1_instance(
            std::array<bool, 3>{
                m_board[i].first[source1],
                m_board[i].first[source2],
                m_board[i].first[target]},
            i, MATRIX_MERGE,
            std::array<std::size_t, 3>{source1, source2, target});
    }
    m_piece_board[target] = m_piece_board[source1];
    update_board();
    update_case(target);
    update_case(source1);
    update_case(source2);
}

template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::move_classic_slide(
    Coord const &s,
    Coord const &t,
    std::function<
        bool(
            Board<N, M> const &,
            Coord const &,
            Coord const &,
            std::size_t,
            std::optional<Coord>)>
        check_path,
    std::optional<bool> val_mes)
{
    std::size_t source = offset(s.n, s.m);
    std::size_t target = offset(t.n, t.m);
    if (m_piece_board[target].get_type() == TypePiece::EMPTY ||
        (m_piece_board[target].get_type() == m_piece_board[source].get_type() &&
         m_piece_board[target].get_color() == m_piece_board[source].get_color()))
    {
        std::size_t const size_board{std::size(m_board)};
        for (std::size_t i{0}; i < size_board; i++)
        {
            move_1_instance(
                std::array<bool, 3>{
                    !check_path(
                        *this, s, t, i, std::nullopt),
                    false,
                    m_board[i].first[source]},
                i, MATRIX_SLIDE,
                std::array<std::size_t, 3>{N * M + 1, target, source});
        }
        m_piece_board[target] = m_piece_board[source];
        update_case(source);
    }
    else
    {
        if (m_piece_board[source].same_color(m_piece_board[target]))
        {
            std::optional<bool> negation ;
            if(val_mes.has_value())
            {
                negation = !val_mes.value();
            }
            if (!mesure(t, negation))
            {
                for (std::size_t i{0}; i < std::size(m_board); i++)
                {
                    move_1_instance(
                        std::array<bool, 3>{
                            !check_path(*this, s, t, i, std::nullopt),
                            false,
                            m_board[i].first[source]},
                        i, MATRIX_SLIDE,
                        std::array<std::size_t, 3>{N * M + 1, target, source});
                }
                m_piece_board[target] = std::move(m_piece_board[source]);
                m_piece_board[source] = Piece();
            }
        }
        else
        {
            if (mesure_capture_slide(s, t, check_path, val_mes))
            {
                for (std::size_t i{0}; i < std::size(m_board); i++)
                {
                    auto const A{
                        MATRIX_ISWAP.tensoriel_product(
                            CMatrix<2>::identity()) *
                        CMatrix<2>::identity()
                            .tensoriel_product(MATRIX_ISWAP)};

                    move_1_instance(
                        std::array<bool, 3>{
                            m_board[i].first[source],
                            m_board[i].first[target], false},
                        i, A,
                        std::array<std::size_t, 3>{source, target, N * M + 1});
                }
                m_piece_board[target] = std::move(m_piece_board[source]);
                m_piece_board[source] = Piece();
            }
        }
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::move_split_slide(
    Coord const &s,
    Coord const &t1,
    Coord const &t2,
    std::function<
        bool(
            Board<N, M> const &,
            Coord const &,
            Coord const &,
            std::size_t,
            std::optional<Coord>)>
        check_path)
{
    std::size_t source = offset(s.n, s.m);
    std::size_t target1 = offset(t1.n, t1.m);
    std::size_t target2 = offset(t2.n, t2.m);
    std::size_t const size_board{std::size(m_board)};
    for (std::size_t i{0}; i < size_board; i++)
    {
        move_1_instance(
            std::array<bool, 5>{
                !check_path(*this, s, t2, i, t1),
                !check_path(*this, s, t1, i, t2),
                m_board[i].first[target2],
                m_board[i].first[target1],
                m_board[i].first[source]},
            i, MATRIX_SPLIT_SLIDE,
            std::array<std::size_t, 5>{
                N * M + 1,
                N * M + 1,
                target2,
                target1,
                source});
        /*La matrice split slide est créée pour
          être utlisé sans appliquer de porte cnot a
          un chemin, nos fonctions check_path renvoie
          un résultat où l'on a appliquer la porte cnot */
    }
    if (m_piece_board[target1].get_type() == TypePiece::EMPTY &&
        m_piece_board[target2].get_type() == TypePiece::EMPTY)
    {
        m_piece_board[target1] = m_piece_board[source];
        m_piece_board[target2] = m_piece_board[source];
        update_case(source);
        update_case(target1);
        update_case(target2);
    }
    else
    {
        m_piece_board[target1] = m_piece_board[source];
        m_piece_board[target2] = m_piece_board[source];
        update_case(source);
        update_case(target1);
        update_case(target2);
        update_board();
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::move_merge_slide(
    Coord const &s1,
    Coord const &s2,
    Coord const &t,
    std::function<
        bool(
            Board<N, M> const &,
            Coord const &,
            Coord const &,
            std::size_t,
            std::optional<Coord>)>
        check_path)
{
    std::size_t const source1 = offset(s1.n, s1.m);
    std::size_t const source2 = offset(s2.n, s2.m);
    std::size_t const target = offset(t.n, t.m);
    std::size_t const size_board{std::size(m_board)};
    for (std::size_t i{0}; i < size_board; i++)
    {
        move_1_instance(
            std::array<bool, 5>{
                !check_path(*this, s2, t, i, s1),
                !check_path(*this, s1, t, i, s2),
                m_board[i].first[source1],
                m_board[i].first[source2],
                m_board[i].first[target]},
            i, MATRIX_MERGE_SLIDE,
            std::array<std::size_t, 5>{
                N * M + 1,
                N * M + 1,
                source1,
                source2,
                target});
        /* La matrice merge slide est créée pour
           être utlisé sans appliquer de porte cnot au chemin,
           nos fonctions check_path renvoie un résultat où l'on
           a appliquer la porte cnot */
    }
    m_piece_board[target] = m_piece_board[source1];
    update_board();
    update_case(target);
    update_case(source1);
    update_case(source2);
}

template <std::size_t N, std::size_t M>
CONSTEXPR bool Board<N, M>::move_pawn(
    Coord const &s,
    Coord const &t,
    std::optional<bool> val_mes) noexcept
{
    bool move_exec{false};
    auto abs_diff{[](std::size_t x, std::size_t y) -> std::size_t
                  {
                      return (x >= y) ? x - y : y - x;
                  }};
    std::size_t diff_line{abs_diff(s.n, t.n)};
    if (diff_line == 2)
    {
        move_exec = move_pawn_two_step(s, t, val_mes);
        m_ep = Coord((s.n + t.n) / 2, s.m);
    }
    else if (diff_line == 1)
    {
        std::size_t diff_col{abs_diff(s.m, t.m)};
        if (diff_col == 1)
        {

            if (m_ep != std::nullopt && m_ep == t)
            {
                int step{
                    ((*this)(s.n, s.m).get_color() ==
                     Color::WHITE)
                        ? -1
                        : 1};
                Coord ep;
                ep.m = m_ep->m;
                ep.n = m_ep->n - step;
                move_exec = move_enpassant(s, t, ep, val_mes);
            }
            else
            {
                move_exec = capture_pawn(s, t, val_mes);
            }
        }
        else
        {
            move_exec = move_pawn_one_step(s, t, val_mes);
        }
        m_ep = std::nullopt;
    }
    return move_exec;
}

template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::move_promotion(
    Move const &move,
    std::optional<bool> val_mes)
{
    TypePiece p{move.promote.piece};
    Coord s{move.promote.src};
    Coord t{move.promote.arv};

    if (p == TypePiece::QUEEN ||
        p == TypePiece::KNIGHT ||
        p == TypePiece::BISHOP ||
        p == TypePiece::ROOK)
    {
        if (move_pawn(s, t, val_mes))
        {
            Piece piece{p, m_piece_board[offset(t.n, t.m)].get_color()};
            m_piece_board[offset(t.n, t.m)] = std::move(piece);
        }
    }
    else
    {
        throw std::runtime_error("Piece non valide pour faire une promotion");
    }
}

template <std::size_t N, std::size_t M>
CONSTEXPR void
Board<N, M>::move_classic(
    Coord const &s,
    Coord const &t,
    std::optional<bool> val_mes)
{
    TypePiece piece{(*this)(s.n, s.m).get_type()};
    switch (piece)
    {
    case TypePiece::KING:
        if constexpr (N == 8 && M == 8)
        {
            auto abs_diff{[](std::size_t x, std::size_t y) -> std::size_t
                          {
                              return (x >= y) ? x - y : y - x;
                          }};
            std::size_t diff_colum{abs_diff(s.m, t.m)};
            if (diff_colum == 2)
            {
                if (s.m > t.m)
                {
                    queen_side_castle(s, Coord(s.n, t.m - 2), val_mes);
                }
                else
                {
                    king_side_castle(s, Coord(s.n, t.m + 1), val_mes);
                }
            }
            else
            {
                move_classic_jump(s, t, val_mes);
            }
        }
        else
        {
            move_classic_jump(s, t, val_mes);
        }
        break;
    case TypePiece::KNIGHT:
        move_classic_jump(s, t, val_mes);
        break;
    case TypePiece::BISHOP:
        move_classic_slide(
            s, t,
            &check_path_diagonal_1_instance<N, M>,
            val_mes);
        break;
    case TypePiece::ROOK:
        move_classic_slide(
            s, t,
            &check_path_straight_1_instance<N, M>,
            val_mes);
        break;
    case TypePiece::QUEEN:
        move_classic_slide(
            s, t,
            &check_path_queen_1_instance<N, M>,
            val_mes);
        break;
    case TypePiece::PAWN:
        move_pawn(s, t, val_mes);
        return;
    case TypePiece::EMPTY:
    default:
        break;
    }
    m_ep = std::nullopt;
}

template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::move_split(Coord const &s,
                                       Coord const &t1,
                                       Coord const &t2)
{
    TypePiece piece{(*this)(s.n, s.m).get_type()};
    switch (piece)
    {
    case TypePiece::KING:
    case TypePiece::KNIGHT:
        move_split_jump(s, t1, t2);
        break;
    case TypePiece::BISHOP:
        move_split_slide(s, t1, t2, &check_path_diagonal_1_instance<N, M>);
        break;
    case TypePiece::ROOK:
        move_split_slide(s, t1, t2, &check_path_straight_1_instance<N, M>);
        break;
    case TypePiece::QUEEN:
        move_split_slide(s, t1, t2, &check_path_queen_1_instance<N, M>);
        break;
    case TypePiece::PAWN:
    case TypePiece::EMPTY:
    default:
        break;
    }
    m_ep = std::nullopt;
}

template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::move_merge(Coord const &s1,
                                       Coord const &s2,
                                       Coord const &t)
{
    TypePiece piece1{(*this)(s1.n, s1.m).get_type()};
    TypePiece piece2{(*this)(s2.n, s2.m).get_type()};
    if (piece1 != piece2)
    {
        return;
    }
    switch (piece1)
    {
    case TypePiece::KING:
    case TypePiece::KNIGHT:
        move_merge_jump(s1, s2, t);
        break;
    case TypePiece::BISHOP:
        move_merge_slide(s1, s2, t, &check_path_diagonal_1_instance<N, M>);
        break;
    case TypePiece::ROOK:
        move_merge_slide(s1, s2, t, &check_path_straight_1_instance<N, M>);
        break;
    case TypePiece::QUEEN:
        move_merge_slide(s1, s2, t, &check_path_queen_1_instance<N, M>);
        break;
    case TypePiece::PAWN:
    case TypePiece::EMPTY:
    default:
        break;
    }
    m_ep = std::nullopt;
}

template <std::size_t N, std::size_t M>
CONSTEXPR void Board<N, M>::move(Move const &movement, std::optional<bool> val_mes)
{
    if (std::empty(m_board))
    {
        throw std::runtime_error("Le plateau est vide impossible de réaliser l'opération move");
    }
    switch (movement.type)
    {
    case TypeMove::NORMAL:
        move_classic(movement.normal.src, movement.normal.arv, val_mes);
        break;
    case TypeMove::SPLIT:
        move_split(movement.split.src,
                   movement.split.arv1,
                   movement.split.arv2);
        break;
    case TypeMove::MERGE:
        move_merge(movement.merge.src1,
                   movement.merge.src2,
                   movement.merge.arv);
        break;
    case TypeMove::PROMOTE:
        move_promotion(movement, val_mes);
        break;
    default:
        break;
    }
    update_board_classic();
    //update_board();
}