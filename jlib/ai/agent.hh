/* -*- mode: C++ c-basic-offset: 4 -*-
 * 
 * Copyright (c) 2010 Joe Yandle <joey@divisionbyzero.com>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */

#ifndef JLIB_AI_AGENT_HH
#define JLIB_AI_AGENT_HH


#include <jlib/ai/action.hh>
#include <jlib/ai/percept.hh>

#include <jlib/sys/object.hh>


namespace jlib {
namespace ai {

class Agent : public sys::Object {
public:
    typedef Glib::RefPtr<Agent> ptr;
    typedef std::list<ptr> list;

    Agent();
    virtual ~Agent();

    virtual Action::ptr agent(const Percept::map percepts) = 0;

private:
};


}
}

#endif //JLIB_AI_AGENT_HH
