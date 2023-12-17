#pragma once

#include <array>
#include <charconv>
#include <tuple>

namespace nai {

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
			KING	= 7,
			ANY		= 63
		};

		const std::array<const char, 8> char_representations{ ' ', 'X', 'P', 'R', 'N', 'B', 'Q', 'K' };

		constexpr type char_to_type(char chrrep)
		{
			if (chrrep == '!')
				return type::ANY;

			for (int i = 0; i < char_representations.size(); i++)
				if (char_representations[i] == chrrep)
					return static_cast<type>(i);

			return type::NONE;
		}

		constexpr char type_to_char(type typerep)
		{
			size_t idx = static_cast<size_t>(typerep);
			if (idx < char_representations.size())
				return char_representations[idx];

			return '!'; // This should never happen, we may want to throw an exception here
		}

		constexpr bool is_self_or_ally(underlying_type piece)
		{
			return ((piece >> 6) & static_cast<underlying_type>(owner::SELF)) == static_cast<underlying_type>(owner::SELF);
		}

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
	std::array<std::array<piece::underlying_type, X>, Y> guts;
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

// The format here is <coords>:<side>:<piece_str> where <coords> = i,j,k...
template<int depth = 2>
inline bool from_string(const char* str_in, std::array<int, depth>& coords, piece::underlying_type& what)
{
	std::array<const char*, depth+1> commas;
	std::array<const char*, 2> colons;

	int comma_idx = 1, colon_idx = 0;
	auto it = str_in;
	while (*it++ != '\0')
	{
		if (*it == ',') {
			if (comma_idx >= depth) return false; // more than expected
			commas[comma_idx++] = it;
		} else if (*it == ':') {
			if (colon_idx >= 2) return false; // more than expected
			colons[colon_idx++] = it;
		}
	}

	// less coords or separators than expected
	if (comma_idx < depth || colon_idx < 2) return false;

	commas[0] = str_in-1;
	commas[depth] = colons[0]; // First separator is honorary comma

	// todo: error handle from_chars
	for (int i = 0; i < depth; i++)
		std::from_chars(commas[i]+1, commas[i+1], coords[i]);

	int side = 0;
	std::from_chars(colons[0]+1, colons[1], side);

	const char* piece_type = colons[1] + 1;
	//piece::underlying_type pt = piece::type::from_string(piece_type);

	what = piece::make(static_cast<piece::owner>(side), piece::char_to_type(*piece_type));
	return true;
}

};
