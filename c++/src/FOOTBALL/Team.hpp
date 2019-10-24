#ifndef readtrackingdata_football_team_hpp_
#define readtrackingdata_football_team_hpp_

#include <cstdlib>
#include <memory>
#include <vector>

#include <msgpack.hpp>

#include "Player.hpp"

namespace Football 
{

class Team 
{
    friend class Match;

    protected:
    std::uint32_t           FRAME_ID;
    bool                    BALL_OWNED      =   false;
    std::vector<Player>     PLAYERS_IN_TEAM;

    public:

    /// Returns the number of players on the team determined by the size of the PLAYERS_IN_TEAM vector.
    std::uint16_t number_of_players () const 
    {
        return PLAYERS_IN_TEAM.size();
    }

    MSGPACK_DEFINE(FRAME_ID, BALL_OWNED, PLAYERS_IN_TEAM);

    Team() { }

    Team(std::uint32_t frame_id)
    {
        FRAME_ID = frame_id;
    }

    Team(std::vector<Player> plyrs, std::uint32_t frame_id, bool ball_owned = false) 
    : PLAYERS_IN_TEAM(plyrs) 
    , FRAME_ID(frame_id)
    , BALL_OWNED(ball_owned)
    { }

    // setters and getters

    /// Change the value of TEAM for all players in the team. 
    /// @param team_char - single character representing the players' team. Valid values are 'H' - Home, 'A' - Away, 'O' - Officials, 'U' - Undefined (in case of error)
    /// @throws std::invalid_argument if team_char not in "HAOU"
    void setPlayerTeamChar(char team_char)
    {
        std::string allowed_characters = "HAOU";

        // check if provided argument meets requirements
        if (allowed_characters.find(team_char) == std::string::npos)
        {
            // character provided is not in the allowed characters string.
            std::cerr << team_char << " passed as an argument to setPlayerTeamChar. Valid options are: " << allowed_characters << std::endl;
            throw std::invalid_argument("Attempting to assign invalid team character to players.");
        }

        for (auto& p : PLAYERS_IN_TEAM)
        {
            p.TEAM = team_char;
        }
    }

    /// get current frameId
    std::uint32_t   get_frameId() const
    {
        return FRAME_ID;
    }
    /// set current frameId
    void            set_frameId(const std::uint32_t _frame_id)
    {
        FRAME_ID = _frame_id;
    }

    /// Check if team is in possession of the ball
    bool            get_ballOwned() const
    {
        return BALL_OWNED;
    }
    /// Check if team is in possession of the ball
    bool            ownsBalled()    const
    {
        return get_ballOwned();
    }
    /// Set if team is in possession of the ball
    void            set_ballOwned(bool _ball_owned)
    {
        BALL_OWNED = _ball_owned;
    }

    /// Returns (by reference) a std::vector containing all the players in the team. 
    std::vector<Player>&    get_playersInTeam()
    {
        return  PLAYERS_IN_TEAM;
    }
    /// Set the vector of players for this team.
    void                    set_playersInTeam(const std::vector<Player>& _players_in_team)
    {
        PLAYERS_IN_TEAM = _players_in_team;
    }

    /// Returns (by reference) a specific Football::Player located by index.
    /// @param _player_array_index - location of the player in the vector.
    /// @returns Football::Player located at position _player_array_index in PLAYERS_IN_TEAM vector.
    /// @throws std::out_of_range if _player_array_index >= number_of_players()
    Player&                 get_player(const std::uint16_t _player_array_index)
    {
        if (_player_array_index < number_of_players())
        {
            return PLAYERS_IN_TEAM[_player_array_index];
        }
        else
        {
            std::cerr << _player_array_index << ">=" << number_of_players() << std::endl;
            throw std::out_of_range("Player index given exceeds Player array.");
        }
    }
    /// Assign the Football::Player provided to the PLAYERS_IN_TEAM vector at the given position.
    /// @param _player_array_index - location in the std::vector to store the player.
    /// @param _player      - the player to store
    /// @throws std::out_of_range if _player_array_index >= number_of_players()
    void                    set_player(const std::uint16_t _player_array_index, const Player& _player)
    {
        if (_player_array_index < number_of_players())
        {
            PLAYERS_IN_TEAM[_player_array_index] = _player;
        }
        else
        {
            std::cerr << _player_array_index << ">=" << number_of_players() << std::endl;
            throw std::out_of_range("Player index given exceeds Player array.");
        }
    }

    /// Adds a player to the PLAYERS_IN_TEAM vector via the push_back() method.
    /// @param _player - the player to add.
    void                    add_player(const Player& _player)
    {
        PLAYERS_IN_TEAM.push_back(_player);
    }

};

std::ostream& operator<<(std::ostream& os, const Team& c);
inline bool operator==(const Team& lhs, const Team& rhs);
inline bool operator!=(const Team& lhs, const Team& rhs);

} /* namespace Football */

#endif /* trackingdatalib_football_team_hpp_ */