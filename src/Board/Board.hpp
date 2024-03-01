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

class Piece;

template <std::size_t N = 8, std::size_t M = N>
class Board final
{
public:
    // Constructeur
    constexpr Board();
    constexpr Board(std::initializer_list<std::initializer_list<observer_ptr<Piece const>>> const &board);

    // Copie
    constexpr Board(Board const &) = delete;
    constexpr Board &operator=(Board const &) = delete;

    // Mouvement
    constexpr Board(Board &&) = default;
    constexpr Board &operator=(Board &&) = default;

    // Destructeur
    constexpr ~Board() = default;

    constexpr static std::size_t numberLines() noexcept;
    constexpr static std::size_t numberColumns() noexcept;

    constexpr observer_ptr<Piece const> operator()(std::size_t n, std::size_t m) const noexcept;

    std::forward_list<Coord> get_list_move(Coord const& pos) const;

    constexpr double get_proba(Coord const &pos) const noexcept;
    void update_after_merge() noexcept;

    // a basculer en private
    template <std::size_t Q>
    constexpr void move_1_instance(std::array<bool, Q> const &case_modif,
                                   std::size_t position, CMatrix<_2POW(Q)> const &matrix,
                                   std::array<std::size_t, Q> const &tab_positions);

    constexpr void move_split_jump(Coord const &s, Coord const &t1, Coord const &t2);

    constexpr void move_merge_jump(Coord const &s, Coord const &t1, Coord const &t2);
    constexpr void move_classic_slide(Coord const &s, Coord const &t,
                                      std::function<bool(Board<N, M> const&, Coord const &, Coord const &, std::size_t)> check_path);
    constexpr void move_split_slide(Coord const &s, Coord const &t1, Coord const &t2,
                                    std::function<bool(Board<N, M> const&, Coord const &, Coord const &, std::size_t)> check_path);
    constexpr void move_merge_slide(Coord const &s1, Coord const &s2, Coord const &t,
                                    std::function<bool(Board<N, M> const &, Coord const &, Coord const &, std::size_t)> check_path);
                                  
    constexpr void move_pawn_one_step(Coord const &s, Coord const &t);
    constexpr void move_pawn_two_step(Coord const &s, Coord const &t);
    constexpr void capture_pawn(Coord const &s, Coord const &t);
    constexpr void move_enpassant(Coord const &s, Coord const &t, Coord const &ep);

    bool mesure_capture_slide(Coord const &s, Coord const &t,
                              std::function<bool(Board<N, M> const&, Coord const &, Coord const &, std::size_t)> check_path);

    // a basculer en private
    constexpr void move_classic_jump(Coord const &s, Coord const &t);
    constexpr bool mesure(Coord const& p);

    template <std::size_t _N, std::size_t _M>
    friend constexpr bool check_path_straight_1_instance(Board<_N, _M> const& board, Coord const &dpt, Coord const &arv, std::size_t position);
    
    template <std::size_t _N, std::size_t _M>
    friend constexpr bool check_path_diagonal_1_instance(Board<_N, _M> const& board, Coord const &dpt, Coord const &arv, std::size_t position);

    friend class Piece;

private:
    constexpr static std::size_t offset(std::size_t ligne, std::size_t colonne) noexcept;
    constexpr static std::pair<std::array<bool, N * M>, std::array<observer_ptr<Piece const>, N * M>>
    initializer_list_to_2_array(std::initializer_list<std::initializer_list<observer_ptr<Piece const>>> const &board) noexcept;
    constexpr static void init_mailbox(std::array<int, N * M> &S_mailbox, std::array<int, (N+4) * (M+2)> &L_mailbox) noexcept;
    

    static double get_random_number_0_1();

    template <std::size_t Q>
    constexpr void modify(std::array<std::pair<std::array<bool, Q>, std::complex<double>>, 2> const &arrayQubit,
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
bool operator==(std::array<bool, N> t1, std::array<bool, N> t2) noexcept;

#include "Board.tpp"
#endif