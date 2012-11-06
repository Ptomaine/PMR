#pragma once

/***************************************************************************
 *   Copyright (C) 2008 by Arlen Albert Keshabyan and Vlad Marahduhdah     *
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

#include <complex>
#include <math.h>

#ifndef PMEP_NS
	#define PMEP_NS planar_movements_event_provider_ns
#endif

namespace planar_movements_event_provider_ns
{

class planar_movements_event_provider
{
public:

	typedef std::complex<double> point_type;


private:

	typedef std::pair<int, double> precision_type;

	bool _is_empty;
	bool _invert_x;
	bool _invert_y;
	point_type _current_point;
	precision_type _precision;
	//static const double _m_pi_4 = .785398163397448309616; //uncomment when c++0x is in effect

	planar_movements_event_provider(const planar_movements_event_provider&);
	planar_movements_event_provider &operator = (const planar_movements_event_provider&);

public:

    enum event
    {
        unknown = 0,
        right = 1,
        down_right = 2,
        down = 3,
        down_left = 4,
        left = 5,
        up_left = 6,
        up = 7,
        up_right = 8
    };

	planar_movements_event_provider(int min_step = 20, double section_delta = 1.) :
		_is_empty(true),
		_invert_x(false),
		_invert_y(false),
		_current_point(0, 0),
		_precision(precision_type((min_step < 0) ? 0 : min_step, (section_delta < 0 || section_delta > 1) ? .5 : section_delta / 2))
	{
	}

	void set_invertion(bool invert_x, bool invert_y)
	{
        _invert_x = invert_x;
        _invert_y = invert_y;
	}

	event input(point_type point)
	{
        if(_invert_y || _invert_x)
			point = point_type((_invert_x) ? -point.real() : point.real(), (_invert_y) ? -point.imag() : point.imag());

        if(_is_empty)
        {
            _current_point = point;
            _is_empty = false;

            return unknown;
        }

		point_type result(point - _current_point);

        if(std::abs(result) < _precision.first)
            return unknown;

        _current_point = point;
        double section(std::arg(result) / double(.785398163397448309616));

        if(section < 0)
            section += 8.;

        int section_index = int(section + .5);

        double difference(0);

        if(section_index == 8)
        {
            section_index = 0;
            difference = 8 - section;
        }
        else
        {
            difference = section - double(section_index);

            if(difference < 0)
                difference = -difference;
        }

        if(difference > 1)
            difference -= int(difference);

        if(difference <= _precision.second)
            return event(section_index + 1);

        return unknown;
	}

	template<class T>
	event input(T x, T y)
	{
		return input(point_type(x, y));
	}

	event operator()(point_type point)
	{
		return input(point);
	}

	template<class T>
	event operator()(T x, T y)
	{
		return input(x, y);
	}

	planar_movements_event_provider& clear()
	{
		_is_empty = true;

		return *this;
	}

    planar_movements_event_provider& operator ~()
    {
    	return clear();
    }
};

}
