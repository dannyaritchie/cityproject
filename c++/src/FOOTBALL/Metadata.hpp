#ifndef trackingdatalib_football_metadata_hpp
#define trackingdatalib_football_metadata_hpp

#include <cstdlib>
#include <string>
#include <vector>
#include <utility>


namespace Football
{

class Period
{
    friend class Metadata;
    friend class Match;
    friend inline bool operator==(const Period & lhs, const Period & rhs);

    protected:
    std::uint8_t            PERIOD_ID;
    std::uint32_t           START_FRAME;
    std::uint32_t           END_FRAME;

    public:
    Period(){}

    Period(std::uint8_t period_id, std::uint32_t start_frame, std::uint32_t end_frame)
    : PERIOD_ID     (period_id)
    , START_FRAME   (start_frame)
    , END_FRAME     (end_frame)
    {}

    ///     Translate the frameIds by an amount. Used if frameIds are adjusted in the match to ensure that the frameId references in the period objects are correct. E.G if frameIds are translated to make the first frameId = 0.
    ///     All values for start/end frame are subtracted by parameter d_frame.
    ///     @param d_frame - value to subtract from frameIds.
    void adjust_frames (std::uint32_t d_frame)
    {
        // std::cout << START_FRAME << std::endl;
        START_FRAME -= d_frame;
        // std::cout << START_FRAME << std::endl;
        END_FRAME   -= d_frame;
    }

    // getters and setters

    /// Get the ID of this period. KEY: 1 - first half, 2 - second half, 3 - first half of ET, 4 - second half of ET, 5 - penalties.
    /// @returns integer value of this Period's ID.
    std::uint8_t        get_periodId()      const
    {
        return      PERIOD_ID;
    }
    /// Set the ID of this period. KEY: 1 - first half, 2 - second half, 3 - first half of ET, 4 - second half of ET, 5 - penalties.
    /// @param _period_id - integer value of this Period's ID.
    void                set_periodId(const std::uint8_t _period_id)
    {
        PERIOD_ID   = _period_id;
    }

    /// Get the frameId when this period starts
    std::uint32_t       get_startFrame()    const
    {
        return      START_FRAME;
    }
    /// Set the frameId when this period starts
    void                set_startFrame(const std::uint32_t _start_frame)
    {
        START_FRAME = _start_frame;
    }

    /// Get the frameId when this period ends
    std::uint32_t       get_endFrame()      const
    {
        return      END_FRAME;
    }
    /// Set the frameId when this period ends
    void                set_endFrame(const std::uint32_t _end_frame)
    {
        END_FRAME   = _end_frame;
    }

};

class Metadata
{
    friend class Match;
    friend inline bool operator== (const Metadata & lhs, const Metadata & rhs);

    protected:
    std::uint32_t       MATCHID;
    std::string         DATE;
    std::float_t        FPS;
    std::float_t        PITCH_DIMS[2];
    //float               TRACKING_DIMS[2];
    std::vector<Period> PERIODS; 

    // extra tags
    bool                OPTA_F7;
    bool                OPTA_F24;
    std::string         TRACKING_PROVIDER; 

    public:

    Metadata() : OPTA_F7(false) , OPTA_F24(false) {}   

    ///     Translate the frameIds in all period objects by an amount. Used if frameIds are adjusted in the match to ensure that the frameId references in the period objects are correct. E.G if frameIds are translated to make the first frameId = 0.
    ///     All values for start/end frame are subtracted by parameter d_frame.
    ///     @param d_frame - value to subtract from frameIds
    void adjust_frames (std::int32_t d_frame)
    {
        for (auto& p : PERIODS)
        {
            p.adjust_frames(d_frame);
        }
    }

    /// Get optaMatchId
    std::uint32_t       get_matchId ()  const
    {
        return      MATCHID;
    }
    /// set optaMatchId
    void                set_matchId (const std::uint32_t _match_id)
    {
        MATCHID     = _match_id;
    }

    /// get string containing match date
    std::string         get_date ()     const
    {
        return      DATE;
    }
    /// set match date string
    void                set_date (const std::string _date)
    {
        DATE        = _date;
    }

    /// get the value of Frames Per Seconds (FPS) which the data is recorded at
    std::float_t        get_FPS  ()     const
    {
        return      FPS;
    }
    /// set the value of Frames Per Second (FPS) which the data is recorded at
    void                set_FPS (const std::float_t _fps)
    {
        FPS         = _fps;
    }

    /// get the x dimension of the pitch
    std::float_t        get_pitchX ()   const
    {
        return PITCH_DIMS[0];
    }
    /// get the y dimension of the pitch
    std::float_t        get_pitchY ()   const
    {
        return PITCH_DIMS[1];
    }
    // std::pair<std::float_t, std::float_t> get_pitchDims ()  const <- getting rid, no one really likes pairs
    // {
    //     return std::make_pair(PITCH_DIMS[0], PITCH_DIMS[1]);
    // }

    /// get the dimensions of the pitch as a 2D array
    std::array<std::float_t, 2> get_pitchDims() const
    {
        return std::array<std::float_t, 2>({PITCH_DIMS[0], PITCH_DIMS[1]});
    }

    /// Set the x dimension of the pitch
    void                set_pitchX (const std::float_t _x)
    {
        PITCH_DIMS[0] = _x;
    }
    /// Set the y dimension of the pitch
    void                set_pitchY (const std::float_t _y)
    {
        PITCH_DIMS[1] = _y;
    }
    /// Set the pitch dimensions
    void                set_pitchDims (const std::float_t _x, const std::float_t _y)
    {
        set_pitchX(_x);
        set_pitchY(_y);
    }
    /// Set the pitch dimensions
    void                set_pitchDims (const std::pair<std::float_t, std::float_t> & _dims)
    {
        set_pitchX(_dims.first);
        set_pitchY(_dims.second);
    }
    /// Set the pitch dimensions
    void                set_pitchDims (const std::array<std::float_t, 2> & _dims)
    {
        set_pitchX(_dims[0]);
        set_pitchY(_dims[1]);
    }

    ///     Get the number of periods stored in this metadata file. Counted using std::vector<>.size() with the PERIODS vector.
    ///     @returns    integer counting the number of periods in the PERIODS vector
    std::uint16_t       get_numberOfPeriods () const
    {
        return PERIODS.size();
    }

    ///     Get period object (by reference) from PERIODS vector.
    ///     @param _period_idx - the index selecting which period in the vector.
    ///     @returns reference to Period object located at _period_idx in PERIODS
    ///     @throws std::out_of_range() if _period_idx >= get_numberOfPeriods()  
    Period&             get_period          (const std::uint16_t _period_idx)
    {
        if (_period_idx < get_numberOfPeriods())
        {
            return PERIODS[_period_idx];
        }
        else
        {
            std::cerr << _period_idx << ">=" << get_numberOfPeriods() << " when requesting period" << std::endl;
            throw std::out_of_range("Index out of range while requesting period.");
        }
    }

    ///     Get period object (by reference) from PERIODS vector.
    ///     @param _period_idx - the index indicating location in the vector.
    ///     @param _period  - the period object to store.
    ///     @throws std::out_of_range() if _period_idx >= get_numberOfPeriods().
    void                set_period          (const std::uint16_t _period_idx, const Period& _period)
    {
        if (_period_idx < get_numberOfPeriods())
        {
            PERIODS[_period_idx] = _period;
        }
        else
        {
            std::cerr << _period_idx << ">=" << get_numberOfPeriods() << " when requesting period" << std::endl;
            throw std::out_of_range("Index out of range while requesting period.");
        }
    }

    ///     Get the full vector (by reference) which all period objects are stored in.
    ///     @returns std::vector<Football::Period>&.
    std::vector<Period>&    get_periodsVector() 
    {
        return PERIODS;
    }
    ///     Set the full vector which all period objects are stored in.
    ///     @param  _periods - vector of periods to store
    void                    set_periodsVector(const std::vector<Period>& _periods)
    {
        PERIODS = _periods;
    }

    /// Check whether an accompanying Opta F7 feed is available.
    bool                get_optaF7 () const
    {
        return OPTA_F7;
    }
    /// Set whether an accompanying Opta F7 feed is available.
    void                set_optaF7 (const bool _f7)
    {
        OPTA_F7 = _f7;
    }

    /// Check whether an accompanying Opta F24 feed is available.
    bool                get_optaF24 () const
    {
        return OPTA_F24;
    }
    /// Set whether an accompanying Opta F24 feed is available.
    void                set_optaF24 (const bool _f24) 
    {
        OPTA_F24    = _f24;
    }

    
};

inline bool operator==(const Period & lhs, const Period & rhs)
{
    return (
        lhs.PERIOD_ID   ==  rhs.PERIOD_ID
        &&
        lhs.START_FRAME ==  rhs.START_FRAME
        &&
        lhs.END_FRAME   ==  rhs.END_FRAME
    );
}

inline bool operator!=(const Period & lhs, const Period & rhs)
{
    return !(lhs == rhs);
}

inline bool operator== (const Metadata & lhs, const Metadata & rhs)
{
    // philosophy: assume true unless proven otherwise


    if (lhs.get_numberOfPeriods() == rhs.get_numberOfPeriods())
    {
        // this is good, compare each period
        for (uint i = 0 ; i < lhs.get_numberOfPeriods() ; i++)
        {
            if (lhs.PERIODS[i] != rhs.PERIODS[i])
            {
                return false;
            }
        }
    }

    // periods are correct, check the other member variables

    return (
        lhs.DATE        ==  rhs.DATE
        &&
        lhs.FPS         ==  rhs.FPS
        &&
        lhs.MATCHID     ==  rhs.MATCHID
        &&
        lhs.OPTA_F24    ==  rhs.OPTA_F24
        &&
        lhs.OPTA_F7     ==  rhs.OPTA_F7
        &&
        lhs.TRACKING_PROVIDER == rhs.TRACKING_PROVIDER
        &&
        lhs.PITCH_DIMS[0]==  rhs.PITCH_DIMS[0]
        &&
        lhs.PITCH_DIMS[1]==  rhs.PITCH_DIMS[1]
    );
}

inline bool operator!= (const Metadata & lhs, const Metadata & rhs)
{
    return !(lhs == rhs);
}


} /* namespace Football */



#endif /* trackingdatalib_football_metadata_hpp */