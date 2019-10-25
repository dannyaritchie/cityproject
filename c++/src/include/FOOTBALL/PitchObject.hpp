/*!
 *  \file PitchObject.hpp
 *
 *  \author Lewis Higgins
 *  \date October 2019
 *
 *  \brief Defines the Base Class used for representing anything with a position on the pitch.
 */

#ifndef readtrackingdata_football_pitchobject_hpp_
#define readtrackingdata_football_pitchobject_hpp_

#include <cstdlib>
#include <utility>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>

namespace Football 
{

class PitchObject 
{
    
    friend std::ostream& operator<<(std::ostream&, const PitchObject&); // declare free function as friend (function defined below)
    
    protected:
    std::int16_t    OBJECT_POS_X;
    std::int16_t    OBJECT_POS_Y;

    public:
    PitchObject(std::int16_t x = 0, std::int16_t y = 0) : OBJECT_POS_X(x), OBJECT_POS_Y(y) {}
    PitchObject(std::pair<std::int16_t, std::int16_t> p ) : OBJECT_POS_X(p.first), OBJECT_POS_Y(p.second) {}

    /// get the x position 
    std::int16_t get_posX() const 
    {
        return OBJECT_POS_X;
    }
    /// get the y position
    std::int16_t get_posY() const 
    {
        return OBJECT_POS_Y;
    }
    /// set the x position
    void set_posX(const std::int16_t x) 
    {
        OBJECT_POS_X = x;
    }
    /// set the y position
    void set_posY(const std::int16_t y) 
    {
        OBJECT_POS_Y = y;
    }

    void printPosition() const 
    {
        std::cout << "(" << OBJECT_POS_X << "," << OBJECT_POS_Y << ")" << std::flush;
    }

    // to make printing nice

    virtual void print(std::ostream& where) const 
    {
        where << "(" << OBJECT_POS_X << "," << OBJECT_POS_Y << ")"; // virtual method so we can alter print behaviour on derived classes
    }
};

inline std::ostream& operator<<(std::ostream& os, const PitchObject& c)
{
    c.print(os);
}

} /* namespace Football */

#endif /* readtrackingdata_football_pitchobject_hpp_ */