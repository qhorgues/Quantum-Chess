#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include <vector>
#include <array>
#include <utility>
#include <optional>
#include <complex>
#include <cstddef>
#include <initializer_list>
#include <functional>
#include <CMatrix.hpp>
#include <Piece.hpp>
#include <TypePiece.hpp>
#include <Color.hpp>
#include <Coord.hpp>
#include <forward_list>
#include <observer_ptr.hpp>
#include <constexpr.hpp>

class Piece;

template <std::size_t N = 8, std::size_t M = N>
class Board final
{
public:
    // Constructeur
    CONSTEXPR Board();
    CONSTEXPR Board(std::initializer_list<std::initializer_list<observer_ptr<Piece const>>> const &board);

    // Copie
    CONSTEXPR Board(Board const &) = delete;
    CONSTEXPR Board &operator=(Board const &) = delete;

    // Mouvement
    CONSTEXPR Board(Board &&) = default;
    CONSTEXPR Board &operator=(Board &&) = default;

    // Destructeur
    CONSTEXPR ~Board() = default;

    CONSTEXPR static std::size_t numberLines() noexcept;
    CONSTEXPR static std::size_t numberColumns() noexcept;

    CONSTEXPR observer_ptr<Piece const> operator()(std::size_t n, std::size_t m) const noexcept;

    CONSTEXPR std::forward_list<Coord> get_list_normal_move(Coord const& pos) const;
    CONSTEXPR std::forward_list<Coord> get_list_split_move(Coord const& pos) const;
    CONSTEXPR bool move_is_legal(Move const &move) const;


    CONSTEXPR double get_proba(Coord const &pos) const noexcept;
    void update_after_merge() noexcept;

    // a basculer en private
    template <std::size_t Q>
    CONSTEXPR void move_1_instance(std::array<bool, Q> const &case_modif,
                                   std::size_t position, CMatrix<_2POW(Q)> const &matrix,
                                   std::array<std::size_t, Q> const &tab_positions);

    CONSTEXPR void move_split_jump(Coord const &s, Coord const &t1, Coord const &t2);

    CONSTEXPR void move_merge_jump(Coord const &s, Coord const &t1, Coord const &t2);
    CONSTEXPR void move_classic_slide(Coord const &s, Coord const &t,
                                      std::function<bool(Board<N, M> const&, Coord const &, Coord const &, std::size_t)> check_path);
    CONSTEXPR void move_split_slide(Coord const &s, Coord const &t1, Coord const &t2,
                                    std::function<bool(Board<N, M> const&, Coord const &, Coord const &, std::size_t)> check_path);
    CONSTEXPR void move_merge_slide(Coord const &s1, Coord const &s2, Coord const &t,
                                    std::function<bool(Board<N, M> const &, Coord const &, Coord const &, std::size_t)> check_path);
                                  
    CONSTEXPR void move_pawn_one_step(Coord const &s, Coord const &t);
    CONSTEXPR void move_pawn_two_step(Coord const &s, Coord const &t);
    CONSTEXPR void capture_pawn(Coord const &s, Coord const &t);
    CONSTEXPR void move_enpassant(Coord const &s, Coord const &t, Coord const &ep);

    CONSTEXPR bool mesure_capture_slide(Coord const &s, Coord const &t,
                                        std::function<bool(Board<N, M> const&, Coord const &, Coord const &, std::size_t)> check_path);

    // a basculer en private
    CONSTEXPR void move_classic_jump(Coord const &s, Coord const &t);
    CONSTEXPR bool mesure(Coord const& p);

    template <std::size_t _N, std::size_t _M>
    friend CONSTEXPR bool check_path_straight_1_instance(Board<_N, _M> const& board, Coord const &dpt, Coord const &arv, std::size_t position);
    
    template <std::size_t _N, std::size_t _M>
    friend CONSTEXPR bool check_path_diagonal_1_instance(Board<_N, _M> const& board, Coord const &dpt, Coord const &arv, std::size_t position);

    friend class Piece;

private:
    CONSTEXPR static std::size_t offset(std::size_t ligne, std::size_t colonne) noexcept;
    CONSTEXPR static std::pair<std::array<bool, N * M>, std::array<observer_ptr<Piece const>, N * M>>
    initializer_list_to_2_array(std::initializer_list<std::initializer_list<observer_ptr<Piece const>>> const &board) noexcept;
    CONSTEXPR static void init_mailbox(std::array<int, N * M> &S_mailbox, std::array<int, (N+4) * (M+2)> &L_mailbox) noexcept;
    

    static double get_random_number_0_1();

    template <std::size_t Q>
    CONSTEXPR void modify(std::array<std::pair<std::array<bool, Q>, std::complex<double>>, 2> const &arrayQubit,
                          std::size_t position_board, std::array<std::size_t, Q> const &tab_positions);

    std::vector<std::pair<std::array<bool, N * M>, std::complex<double>>> m_board;
    std::array<observer_ptr<Piece const>, N * M> m_piece_board;

    std::array<int, N * M> m_S_mailbox; // La petite mailbox
    std::array<int, (N+4) * (M+2)> m_L_mailbox; // La grande mailbox

    Color m_couleur;           // Vrai si c'est aux blanc de jouer
    std::array<bool, 2> m_k_castle;  // Vrai si les noirs[0] / blancs[1] peuvent faire le petit roque
    std::array<bool, 2> m_q_castle;  // Vrai si les noirs[0] / blancs[1] peuvent faire le grand roque
    std::optional<Coord> m_ep; // Contient la case sur laquelle il est possible de faire une prise en passant
};

template <std::size_t N>
CONSTEXPR bool operator==(std::array<bool, N> t1, std::array<bool, N> t2) noexcept;

#include "Board.tpp"
#endif