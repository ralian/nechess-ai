#pragma once

#include <array>
#include <tuple>

namespace nai {

	// move to own header once we add movement logic
	namespace piece {
		// For now...
		using underlying_type = uint8_t;

		enum struct owner : underlying_type {
			NONE = 0,
			SELF = 1,
			OTHER = 2,
			ALLY = 3
		};

		enum struct type : underlying_type {
			NONE	= 0,
			BARRIER = 1,
			PAWN	= 2,
			ROOK	= 3,
			KNIGHT	= 4,
			BISHOP	= 5,
			QUEEN	= 6,
			KING	= 7
		};

		[[nodiscard]]
		constexpr underlying_type make(owner o, type t)
		{
			return static_cast<underlying_type>(o) | (static_cast<underlying_type>(t) << 6);
		}

		constexpr underlying_type empty{ make(owner::NONE, type::NONE) };
		constexpr underlying_type barrier{ make(owner::NONE, type::BARRIER) };
	}

// For some gamemodes like poincare, we will eventually need an abstract_board which represents adjacencies with a graph rather than a 2d span

template<unsigned int X, unsigned int Y>
struct local_board {
	piece::underlying_type guts[X][Y]; // TODO: Possibly also make std::array?
	std::array<local_board*, X> connect_t{};
	std::array<local_board*, X> connect_b{};
	std::array<local_board*, Y> connect_l{};
	std::array<local_board*, Y> connect_r{};
};

template<unsigned int X, unsigned int Y>
using board_point = std::tuple<local_board<X, Y>*, unsigned int, unsigned int>;

template<unsigned int X1, unsigned int Y1, unsigned int X2, unsigned int Y2>
[[nodiscard]]
inline bool move(board_point<X1, Y1> from, board_point<X2, Y2> to, piece::underlying_type& taken)
{
	const auto [b1, x1, y1] = from;
	const auto [b2, x2, y2] = to;

	if (b1 == nullptr || x1 >= X1 || y1 >= Y1 || x2 >= X2 || y2 >= Y2)
		return false;

	if (b2 != nullptr) // If the piece isn't just being deleted
	{
		taken = b2->guts[x2][y2];
		b2->guts[x2][y2] = b1->guts[x1][y1];
	}

	b1->guts[x1][y1] = piece::empty_piece;

	return true;
}

};
