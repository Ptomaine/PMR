#pragma once

/***************************************************************************
 *   Copyright (C) 2008 by Arlen Keshabyan and Vlad Marahduhdah            *
 *   <arlen.albert@gmail.com>                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <algorithm>
#include <iterator>

#ifndef RNF_NS
	#define RNF_NS remove_noise_filter_ns
#endif

namespace remove_noise_filter_ns
{

class remove_noise_filter
{
public:

	// use move_iterator to sequence in c++x0
	template<typename sequence_type>
	sequence_type filter(sequence_type sequence, int min_length_limit = 2) const
	{
		if(sequence.empty())
			return sequence_type();
			//return std::move(sequence_type());

		sequence_type result_sequence;
		typename sequence_type::value_type current_value(sequence.front());
		//typename sequence_type::value_type current_value(std::move(sequence.front()));
		typename sequence_type::iterator begin(sequence.begin()), current(begin), end(sequence.end());
		int current_sequence_length(1);

		for(++current; current != end; ++current)
		{
			if(*current != current_value)
			{
				if(current_sequence_length >= min_length_limit)
					std::copy(begin, current, std::back_inserter(result_sequence));
					//std::copy(std::make_move_iterator(begin), std::make_move_iterator(current), std::back_inserter(result_sequence));

				begin = current;
				current_value = *current;
				current_sequence_length = 0;
			}

			current_sequence_length += 1;
		}

		if(current_sequence_length >= min_length_limit)
			std::copy(begin, current, std::back_inserter(result_sequence));
			//std::copy(std::make_move_iterator(begin), std::make_move_iterator(current), std::back_inserter(result_sequence));

		return result_sequence;
		//return std::move(result_sequence);
	}

	template<typename event_sequence_type>
	event_sequence_type operator() (event_sequence_type sequence, int min_length_limit = 2) const
	{
		return filter(sequence, min_length_limit);
		//return std::move(filter(std::move(sequence), min_length_limit));
	}
};

}
