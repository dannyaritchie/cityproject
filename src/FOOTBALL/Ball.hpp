#ifndef readtrackingdata_football_ball_hpp_
#define readtrackingdata_football_ball_hpp_

#include <cstdint>

#include <msgpack.hpp>

#include "PitchObject.hpp"

namespace Football 
{

class Ball: public PitchObject 
{
    friend class Match;
    friend inline bool operator==(const Ball& lhs, const Ball& rhs);

    protected:
    // inherited:   int     OBJECT_POS_X
    // inherited:   int     OBJECT_POS_Y
    std::int16_t            OBJECT_POS_Z;
    // double          SPEED;
    bool                    ALIVE;
    char                    OWNING_TEAM;
    std::uint32_t           OWNING_PLAYER_ID;          // PLAYER_ID of player in possession
    std::uint32_t           FRAME_ID;


    public:
    MSGPACK_DEFINE(FRAME_ID, OBJECT_POS_X, OBJECT_POS_Y, OBJECT_POS_Z, ALIVE, OWNING_TEAM, OWNING_PLAYER_ID);

    Ball()
    {}

    Ball(std::uint32_t frame_id)
    {
        FRAME_ID = frame_id;
    }
    Ball(std::int16_t x, std::int16_t y, std::uint32_t frame_id = 0) 
    {
        OBJECT_POS_X = x ; OBJECT_POS_Y = y ; FRAME_ID = frame_id ;
    }
    Ball(std::pair<std::int16_t, std::int16_t> p) 
    {
        OBJECT_POS_X = p.first ; OBJECT_POS_Y = p.second;
    }
    Ball(const Ball& b, std::uint32_t frame_id)
    {
        OBJECT_POS_X = b.OBJECT_POS_X;
        OBJECT_POS_Y = b.OBJECT_POS_Y;
        OBJECT_POS_Z = b.OBJECT_POS_Z;
        // SPEED = b.SPEED;
        ALIVE = b.ALIVE;
        OWNING_TEAM = b.OWNING_TEAM;
        FRAME_ID = frame_id;
    }

    static Ball createRandomBall(std::uint32_t frame_id = 0) 
    {
        double randx = ((double) rand()) / RAND_MAX;
        double randy = ((double) rand()) / RAND_MAX;
        return Ball(randx, randy, frame_id);
    }

    virtual void print(std::ostream os) const 
    {
        os << "Ball: (" << OBJECT_POS_X << "," << OBJECT_POS_Y << ")";
    }

    // getters and setters
    
    std::uint16_t   get_posZ() const
    {
        return OBJECT_POS_Z;
    }
    void            set_posZ(const std::uint16_t _z) 
    {
        OBJECT_POS_Z = _z;
    }

    std::array<std::int16_t, 3> get_pos () const
    {
        return std::array<std::int16_t, 3>({OBJECT_POS_X, OBJECT_POS_Y, OBJECT_POS_Z});
    }
    void            set_pos (const std::array<std::int16_t, 3> & _pos)
    {
        OBJECT_POS_X = _pos[0];
        OBJECT_POS_Y = _pos[1];
        OBJECT_POS_Z = _pos[2];
    }

    bool            is_alive() const
    {
        return ALIVE;
    }
    void            set_alive(const bool  _alive)
    {
        ALIVE = _alive;
    }

    char            get_owningTeam() const
    {
        return OWNING_TEAM;
    }
    void            set_owningTeam(const char _team)
    {
        if (_team == 'H' || _team == 'A' || _team == 'U')
        {
            OWNING_TEAM = _team;
        }
        else
        {
            std::cerr << _team << " is not a value argument for Owning Team." << std::endl;
            throw std::invalid_argument("Owning Team must be assigned 'H', 'A', or 'U'.");
        }
    }
    
    std::uint32_t   get_owningPlayerId()    const
    {
        return OWNING_PLAYER_ID;
    }
    void            set_owningPlayerid(const std::uint32_t _player_id)
    {
        OWNING_PLAYER_ID = _player_id;
    }

    std::uint32_t   get_frameId()           const
    {
        return FRAME_ID;
    }
    void            set_frameId(const std::uint32_t _frame_id)
    {
        FRAME_ID = _frame_id;
    }
};

inline bool operator==(const Ball& lhs, const Ball& rhs)
{
    return (
        lhs.OBJECT_POS_X        ==  rhs.OBJECT_POS_X
        &&
        lhs.OBJECT_POS_Y        ==  rhs.OBJECT_POS_Y
        &&
        lhs.OBJECT_POS_Z        ==  rhs.OBJECT_POS_Z
        &&
        lhs.OWNING_PLAYER_ID    ==  rhs.OWNING_PLAYER_ID
        &&
        lhs.OWNING_TEAM         ==  rhs.OWNING_TEAM
        &&
        lhs.FRAME_ID            ==  rhs.FRAME_ID
        &&
        lhs.ALIVE               ==  rhs.ALIVE
    );
}
inline bool operator!=(const Ball& lhs, const Ball& rhs)
{
    return !(lhs == rhs);
}

} /* namespace Football */

#endif /* trackingdatalib_football_ball_hpp_ */