#ifndef trackingdatalib_football_match_hpp_
#define trackingdatalib_football_match_hpp_

#include <cstdlib>
#include <memory>
#include <fstream>
#include <string>
#include <cmath>

#include <msgpack.hpp>

#include "Ball.hpp"
#include "Team.hpp"
#include "Metadata.hpp"

namespace Football 
{

/// Temporary object to contain the ball and teams for a given instant of the match
struct Frame
{
    std::uint32_t   FRAME_ID;
    Ball            BALL;
    Team            HOMETEAM;
    Team            AWAYTEAM;

    Frame()
    {}

    Frame(std::uint32_t _frame_id, const Ball & _b = Ball(), const Team & _ht = Team(), const Team & _at = Team())
    :   FRAME_ID(_frame_id)
    ,   BALL(_b)
    ,   HOMETEAM(_ht)
    ,   AWAYTEAM(_at)
    {}

    bool    isAlive() const 
    {
        return BALL.is_alive();
    }
};

bool operator!=(const Metadata& lhs, const Metadata& rhs);

/// Match structure. Stores a Ball object and two Team objects for each frame in a std::vector.
/// Unlike most of the objects defined in this library, this is a struct and all members are publicly accessible for shortcuts. Be careful with this.
struct Match 
{

    std::vector<Ball>   BALL_FRAMES;
    std::vector<Team>   HOMETEAM_FRAMES;
    std::vector<Team>   AWAYTEAM_FRAMES;
    std::vector<Team>   OFFICIALS_FRAMES;
    Metadata            METADATA;

    /// Returns the number of frames in the match. Counted from the length of the vectors.
    std::uint32_t number_of_frames() const 
    { 
        // check
        assert(BALL_FRAMES.size() == HOMETEAM_FRAMES.size());
        assert(BALL_FRAMES.size() == AWAYTEAM_FRAMES.size());
        
        return BALL_FRAMES.size(); 
    }

    Match() {}

    Frame get_frame (std::uint32_t idx) const
    {
        if (idx < number_of_frames())
        {
            return Frame (
                BALL_FRAMES[idx].FRAME_ID,
                BALL_FRAMES[idx],
                HOMETEAM_FRAMES[idx],
                AWAYTEAM_FRAMES[idx]
            );
        }
        else
        {
            std::cerr << idx << ">=" << number_of_frames() << std::endl;
            throw std::out_of_range("Index provided to create Frame from vectors is out of range.");
        }
    }

    /// Reduce the match to effective 5fps by removing all frames where the frame ID is not a multiple of 5. Skips if match is already 5fps.
    void reduce_to_5fps()
    {
        if (METADATA.FPS == 5.0) 
        {
            std::cout << "[Note]: Match already 5 FPS" << std::endl;
            return;
        } // don't do if already 5 fps

        std::vector<Ball>   TEMP_BALL;

        for (auto& b : BALL_FRAMES)
        {
            if (b.FRAME_ID % 5 == 0)
            {
                b.FRAME_ID /= 5;
                TEMP_BALL.push_back(b);
            }
        }

        BALL_FRAMES = TEMP_BALL;

        std::vector<Team>   TEMP_HOME;

        for (auto& h : HOMETEAM_FRAMES)
        {
            if (h.FRAME_ID % 5 == 0)
            {
                h.FRAME_ID /= 5;
                TEMP_HOME.push_back(h);
            }
        }

        HOMETEAM_FRAMES = TEMP_HOME;


        std::vector<Team>   TEMP_AWAY;

        for (auto& a : AWAYTEAM_FRAMES)
        {
            if (a.FRAME_ID % 5 == 0)
            {
                a.FRAME_ID /= 5;
                TEMP_AWAY.push_back(a);
            }
        }

        AWAYTEAM_FRAMES = TEMP_AWAY;


        std::vector<Team>   TEMP_OFFI;

        for (auto& o : OFFICIALS_FRAMES)
        {
            if (o.FRAME_ID % 5 == 0)
            {
                o.FRAME_ID /= 5;
                TEMP_OFFI.push_back(o);
            }
        }

        OFFICIALS_FRAMES = TEMP_OFFI;

        METADATA.FPS = 5.0;
        
        for (auto& p: METADATA.PERIODS)
        {
            p.START_FRAME = ceil(((double) p.START_FRAME) / 5);
            p.END_FRAME = floor(((double) p.END_FRAME) / 5);
        }
    }

    /// Removes frames where the ball is dead.
    /// @param verbose  -   print before and after statistics to std::cout
    void remove_dead_frames(bool verbose = false)
    {
        std::uint32_t initial_count = number_of_frames();

        std::vector<Ball>   TEMP_BALL;
        std::vector<Team>   TEMP_HOME;
        std::vector<Team>   TEMP_AWAY;
        std::vector<Team>   TEMP_OFFI;

        for (std::uint32_t i = 0; i < BALL_FRAMES.size() ; i++)
        {
            if (BALL_FRAMES[i].ALIVE)
            {
                TEMP_BALL.push_back(BALL_FRAMES[i]);
                TEMP_HOME.push_back(HOMETEAM_FRAMES[i]);
                TEMP_AWAY.push_back(AWAYTEAM_FRAMES[i]);
                TEMP_OFFI.push_back(OFFICIALS_FRAMES[i]);
            }
        }

        BALL_FRAMES         = TEMP_BALL;
        HOMETEAM_FRAMES     = TEMP_HOME;
        AWAYTEAM_FRAMES     = TEMP_AWAY;
        OFFICIALS_FRAMES    = TEMP_OFFI;

        std::uint32_t final_count = number_of_frames();

        if (verbose)
            std::cout << "Before: " << initial_count << " | After: " << final_count << " | Removed: " << (initial_count - final_count) << std::endl;
    }

    /// Rotate the pitch coordinates for periods 2,4 to stop teams swapping halves after each period.
    void mirror_alternate_periods ()
    {
        // iterate through periods
        for (auto& p : METADATA.PERIODS)
        {
            // check if period id is even
            if (p.PERIOD_ID % 2 == 0)
            {
                // values used for while loop
                bool finished;
                std::uint32_t i;
                std::uint32_t fin = BALL_FRAMES.size();

                // loop through frames
                for (i = 0, finished = false ; (!finished && i < fin) ; i++)
                {
                    // check if current frame is in relevant period
                    if (BALL_FRAMES[i].FRAME_ID >= p.START_FRAME)
                    {
                        if (BALL_FRAMES[i].FRAME_ID > p.END_FRAME)
                        {
                            // outside of relevant period, declare finished
                            finished = true;
                        }
                        else
                        {
                            // reverse all coordinates (tedious)
                            BALL_FRAMES[i].OBJECT_POS_X *= -1;
                            BALL_FRAMES[i].OBJECT_POS_Y *= -1;

                            // Home
                            for (auto& p : HOMETEAM_FRAMES[i].PLAYERS_IN_TEAM)
                            {
                                p.OBJECT_POS_X *= -1;
                                p.OBJECT_POS_Y *= -1;
                            }

                            // Away
                            for (auto& p : AWAYTEAM_FRAMES[i].PLAYERS_IN_TEAM)
                            {
                                p.OBJECT_POS_X *= -1;
                                p.OBJECT_POS_Y *= -1;
                            }

                            // Officials, warning, may not be present
                            if (OFFICIALS_FRAMES.size() > 0)
                            {
                                // official frames are present
                                for (auto& p : OFFICIALS_FRAMES[i].PLAYERS_IN_TEAM)
                                {
                                    p.OBJECT_POS_X *= -1;
                                    p.OBJECT_POS_Y *= -1;
                                }
                            }
                            
                        }
                    }
                }
            }
        }
    }

    /// Translate all frameIDs such that the first frame in the dataset has frameID = 0. This will automatically adjust the start/end values in METADATA.PERIODS
    void resetFrameIDs()
    {
        if (number_of_frames() >= 1)
        {
            std::uint32_t first_frame_value = BALL_FRAMES[0].FRAME_ID;

            for (std::uint32_t i = 0 ; i < number_of_frames() ; i++)
            {
                BALL_FRAMES[i].FRAME_ID         -= first_frame_value;
                HOMETEAM_FRAMES[i].FRAME_ID     -= first_frame_value;
                AWAYTEAM_FRAMES[i].FRAME_ID     -= first_frame_value;
                if (OFFICIALS_FRAMES.size() > 0)
                    OFFICIALS_FRAMES[i].FRAME_ID    -= first_frame_value;
            }

            for (auto& p : METADATA.PERIODS)
            {   
                // adjust period start and end frame tags
                p.adjust_frames(first_frame_value);
            }
        }
    }

    /// Loads a full match from a given path into this match object. If fps5 option is true then the '5fps/' subdir is used to load data.
    /// @param  _data_dir   - the path to the directory where data folders are stored.
    /// @param  _match_id   - the optaId of the desired match (used to locate the gamePack folder)
    /// @param  fps5 [true] - use the 5fps version of the match
    void loadFromFile(std::string _data_dir, std::uint32_t _match_id, bool fps5 = true)
    {
        getMatchFromFile(*this, _data_dir, _match_id, fps5);
    }

    /// I'm leaving this exposed rather than having it as a protected member. It loads the msgpk file from @param path and stores the data in @param T& store. While T is a template it will only work with structures that have proper MsgPack definitions. 
    /// If you wish to use this function to load a subfile, create an empty std::vector<Football::Team> (or std::vector<Football::Ball> for Ball subfile) and use that vector as the store for this method.
    /// @param path - path to subfile
    /// @param T& store - object to store data in
    /// @returns bool success - whether the file was loaded or not
    template<typename T>
    static bool load_subfile(std::string path, T& store, bool required = true)
    {
        std::cout << path << std::endl;
        // check if exists
        // if (boost::filesystem::exists(path))
        try // work around for the moment
        {
            std::ifstream ifs (path);
            // read file into buffer
            std::stringstream buffer;
            buffer << ifs.rdbuf();
            // unpack buffer into msgpack object
            msgpack::object_handle oh = msgpack::unpack(buffer.str().data(), buffer.str().size());
            msgpack::object obj = oh.get();
            obj.convert(store);

            // success
            return true;
        }
        catch (std::exception& e)
        {
            // file not found essentially, if file is required inform the user
            if (required)
            {    
                std::cerr << "Error loading subfile: " <<  e.what() << std::endl;
                throw std::exception();
            }
            return false;
        }
        
    }

    /// Loads a full match from a given path. If fps5 option is true then the '5fps/' subdir is used to load data. Match is stored in a provided container.
    /// @param  storage_match - Match object which is wiped then used to store loaded data
    /// @param  _data_dir   - the path to the directory where data folders are stored.
    /// @param  _match_id   - the optaId of the desired match (used to locate the gamePack folder)
    /// @param  fps5 [true] - use the 5fps version of the match
    /// @returns bool success - whether the file was loaded or not
    static bool getMatchFromFile(Match& storage_match, std::string _data_dir, std::uint32_t _match_id, bool fps5 = true)
    /*
        Loads match from base_file_path and stores in match container provided.

        Returns false if there was a problem loading the match
    */
    {
        // check data_dir
        if (_data_dir.back() != '/')
        {
            // add the slash for proper file path construction
            _data_dir += '/';
        }

        // sub_dir used to locate 5fps files if needed
        std::string sub_dir = "";
        if (fps5)
        {
            sub_dir = "5fps/";
        }

        std::string base_file_path = _data_dir + std::to_string(_match_id) + "/" + sub_dir + std::to_string(_match_id);
        // return bool, whether loading went okay or not
        bool load_ok = true;
        // wipe the match container provided
        storage_match = Match();

        // ball
        load_ok = load_ok && load_subfile(base_file_path + ".BALL.msgpack", storage_match.BALL_FRAMES);
        // home
        load_ok = load_ok && load_subfile(base_file_path + ".HOME.msgpack", storage_match.HOMETEAM_FRAMES);
        // away
        load_ok = load_ok && load_subfile(base_file_path + ".AWAY.msgpack", storage_match.AWAYTEAM_FRAMES);
        // officials - not necessarily present and not required 
        auto official_file = load_subfile(base_file_path + ".OFFICIALS.msgpack", storage_match.OFFICIALS_FRAMES, false);
        
        if (!official_file)
            std::cout << "[INFO] No officials file" << std::endl;
        

        // if any of these failed an return false, load_ok will now read false

        return load_ok;
    }

};

} /* namespace Football */

#endif /* trackingdatalib_football_match_hpp_ */