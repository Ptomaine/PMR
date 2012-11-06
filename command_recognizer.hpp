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

#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <functional>

#ifndef CR_NS
	#define CR_NS command_recognizer_ns
#endif

namespace command_recognizer_ns
{

/**
*
*
*
*
*
*
*
*
*
*
*/

template<typename event_type, typename command_type, typename event_sequence_type = std::list<event_type> >
class command_recognizer
{
private:
    std::map<event_sequence_type, command_type> _map_to_command;
    bool _remove_repetitions;

	inline bool discover(event_sequence_type full_sequence, event_sequence_type current_event_sequence, bool strict, bool from_end) const
	{
		typename event_sequence_type::iterator it;

		if(from_end)
		{
			reverse(current_event_sequence.begin(), current_event_sequence.end());
			reverse(full_sequence.begin(), full_sequence.end());
		}

		it = search(full_sequence.begin(), full_sequence.end(), current_event_sequence.begin(), current_event_sequence.end());

		if(it == full_sequence.end())
			return false;

		if(strict && it != full_sequence.begin())
			return false;

		return true;
	}

public:
    command_recognizer(bool remove_repetitions = true) :
		_remove_repetitions(remove_repetitions)
    {
    }

    command_recognizer& set_remove_repetitions(bool remove_repetitions = true)
    {
    	_remove_repetitions = remove_repetitions;

	return *this;
    }

    command_recognizer& add_command(command_type command, event_sequence_type sequence)
    {
    	if(!sequence.empty())
    	{
			sequence.erase(std::remove_if(sequence.begin(), sequence.end(), std::logical_not<event_type>()), sequence.end());

			if(_remove_repetitions)
				sequence.erase(std::unique(sequence.begin(), sequence.end()), sequence.end());

			if(!sequence.empty())
				_map_to_command[sequence] = command;
    	}

		return *this;
    }

    command_recognizer& remove_sequence(event_sequence_type sequence)
    {
    	if(!sequence.empty())
    	{
			sequence.erase(std::remove_if(sequence.begin(), sequence.end(), std::logical_not<event_type>()), sequence.end());

			if(_remove_repetitions)
				sequence.erase(std::unique(sequence.begin(), sequence.end()), sequence.end());

			_map_to_command.remove(sequence);
    	}

    	return *this;
    }

    command_recognizer& remove_command(command_type command)
    {
		for(typename std::map<event_sequence_type, command_type>::iterator it(_map_to_command.begin()), end(_map_to_command.end()); it != end; ++it)
			if(it->second == command)
				_map_to_command.remove(it->first);

    	return *this;
    }

    command_type get_command(event_sequence_type current_event_sequence, bool recover = false, bool is_strict = true, bool from_end = false) const
    {
        if(current_event_sequence.empty())
			return command_type(0);

		if(_remove_repetitions)
			current_event_sequence.erase(std::unique(current_event_sequence.begin(), current_event_sequence.end()), current_event_sequence.end());

		typename std::map<event_sequence_type, command_type>::const_iterator it, end;

		if(recover)
		{
			current_event_sequence.erase(std::remove_if(current_event_sequence.begin(), current_event_sequence.end(), std::logical_not<event_type>()), current_event_sequence.end());

			for(it = _map_to_command.begin(), end = _map_to_command.end(); it != end; ++it)
				if(discover(it->first, current_event_sequence, is_strict, from_end))
					return it->second;
		}
		else
		{
			it = _map_to_command.find(current_event_sequence);

			if(it != _map_to_command.end())
				return it->second;
			else
				return command_type(0);
		}

        return command_type(0);
    }

    command_type operator()(event_sequence_type current_event_sequence, bool recover = false, bool is_strict = true, bool from_end = false) const
    {
    	return get_command(current_event_sequence, recover, is_strict, from_end);
    }

    bool is_command_here(command_type command) const
    {
		for(typename std::map<event_sequence_type, command_type>::const_iterator it(_map_to_command.begin()), end(_map_to_command.end()); it != end; ++it)
			if(it->second == command)
				return true;

    	return false;
    }

    bool is_sequence_here(event_sequence_type sequence) const
    {
    	if(!sequence.empty())
    	{
			sequence.erase(std::remove_if(sequence.begin(), sequence.end(), std::logical_not<event_type>()), sequence.end());

			if(_remove_repetitions)
				sequence.erase(std::unique(sequence.begin(), sequence.end()), sequence.end());
    	}

		return (_map_to_command.find(sequence) != _map_to_command.end());
    }

    std::map<event_sequence_type, command_type> &get_data()
    {
    	return _map_to_command;
    }
};

}

template<typename container_type, typename item_type>
inline container_type& operator << (container_type& container, item_type item)
{
    container.push_back(item);

    return container;
}

template<typename container_type, const char*>
inline container_type& operator << (container_type& container, const char *item)
{
    container.push_back(std::string(item));

    return container;
}

inline std::string& operator << (std::string& container, const char item)
{
    container.append(1, item);

    return container;
}

inline std::string& operator << (std::string& container, const char *item)
{
    container.append(item);

    return container;
}

template<typename container_type, const wchar_t*>
inline container_type& operator << (container_type& container, const wchar_t *item)
{
    container.push_back(std::wstring(item));

    return container;
}

inline std::wstring& operator << (std::wstring& container, const wchar_t item)
{
    container.append(1, item);

    return container;
}

inline std::wstring& operator << (std::wstring& container, const wchar_t* item)
{
    container.append(item);

    return container;
}

template<typename container_type, typename event_type>
inline container_type& operator <<= (container_type& container, event_type event)
{
    if(!!event)
		container << event;

    return container;
}

template<typename container_type>
inline container_type& operator ~ (container_type& container)
{
	container.clear();

    return container;
}
