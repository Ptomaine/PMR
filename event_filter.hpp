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

#include <map>

#ifndef EF_NS
	#define EF_NS event_filter_ns
#endif

namespace event_filter_ns
{

template<typename event_type>
class event_filter
{
private:
	std::map<event_type, event_type> _event_map;

public:
	void clear()
	{
		_event_map.clear();
	}

	event_filter& set(event_type source_event, event_type dest_event)
	{
		_event_map[source_event] = dest_event;

		return *this;
	}

	event_filter& set(event_type event)
	{
		return set(event, event);
	}

	event_type& operator[](event_type source_event)
	{
		return _event_map[source_event];
	}

	int size() const
	{
		return _event_map.size();
	}

	event_type filter(event_type source_event, event_type default_event = event_type(0), bool transparent = false)
	{
		if(_event_map.empty())
			return source_event;

		typename std::map<event_type, event_type>::const_iterator it(_event_map.find(source_event));

		if(it != _event_map.end())
			return it->second;
		else
			if(transparent)
				return source_event;

		return default_event;
	}

	event_type operator()(event_type source_event, event_type default_event = event_type(0), bool transparent = false)
	{
		return filter(source_event, default_event, transparent);
	}

	void remove(event_type source_event)
	{
		_event_map.remove(source_event);
	}
};

}
