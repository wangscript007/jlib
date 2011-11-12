/* -*- mode: C++ c-basic-offset: 4 -*-
 *
 * Copyright (c) 1999 Joe Yandle <joey@divisionbyzero.com>
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

#include <iostream>
#include <sstream>
#include <string>

#include <jlib/math/math.hh>
#include <jlib/util/util.hh>

using jlib::math::matrix;
using jlib::math::cuboid;
using jlib::math::vertex;
using jlib::util::tokenize;

int main(int argc, char** argv) {
    std::string ss[] = {
	"2 3\n"
	"4 0", 

	"10 15\n"
	"20 0", 

	"1 2 0\n"
	"5 -1 0",

	"17 1 0\n"
	"4 8 0",

	"1 1 1\n"
	"1 1 3\n"
	"2 5 8",

	"0 0 1\n"
	"1 1 3\n"
	"0 5 8",

	"0 1\n"
	"1 2" };

    uint n = sizeof(ss) / sizeof(std::string);
    std::vector< matrix<int> > m;

    for (uint i = 0; i < n; i++) {
        std::vector<std::string> tok = tokenize(ss[i], "\n", false);

        uint x = tok.size();
        uint y = tokenize(tok[0], " ", false).size();

        std::istringstream is(ss[i]);
        matrix<int> ma(x, y);
        
        is >> ma;
        
        m.push_back(ma);
    }
    
    matrix<int> mi = matrix<int>::identity(2);
    matrix<int> md = matrix<int>::diagonal(2, 5);

    try {
        matrix<int> m4 = (m[0] * m[2]);
        if(m4 != m[3]) {
            std::cerr << m[0] << std::endl << "*" << std::endl
                      << m[2] << std::endl << "=" << std::endl
                      << m4 << std::endl << "rather than" << std::endl
                      << m[3] << std::endl << std::endl;
        }
        
        matrix<int> m5 = (m[0] * mi);
        if(m5 != m[0]) {
            std::cerr << m[0] << std::endl << "*" << std::endl
                      << mi << std::endl << "=" << std::endl
                      << m5 << std::endl << "rather than" << std::endl
                      << m[0] << std::endl << std::endl;
        }
        
        matrix<int> m6 = (m[0] * md);
        if(m6 != m[1]) {
            std::cerr << m[0] << std::endl << "*" << std::endl
                      << md << std::endl << "=" << std::endl
                      << m6 << std::endl << "rather than" << std::endl
                      << m[1] << std::endl << std::endl;
        }
        
        std::map<std::string, matrix<int> > factors = 
            m[5].factor(matrix<int>::LU);
        
        std::cout << factors.find("P")->second << "*" << std::endl 
                  << m[5] << "=" << std::endl 
                  << (m[5] * factors.find("P")->second) << std::endl << std::endl;
        
        std::vector< std::pair<double,double> > clip;
        clip.push_back(std::make_pair(-1.0, 1.0));
        clip.push_back(std::make_pair(-1.0, 1.0));
        clip.push_back(std::make_pair(-1.0, 1.0));
        clip.push_back(std::make_pair(1.0, 4.0));
        matrix<double> tp4 = matrix<double>::project(4, clip);
        std::cout << tp4 << std::endl << std::endl;
        
        cuboid<double> square(2);
        
    } catch (matrix<int>::mismatch) {
        std::cerr << "matrices are mismatched" << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "caught error" << std::endl;
        return 1;
    }
    return 0;
}

