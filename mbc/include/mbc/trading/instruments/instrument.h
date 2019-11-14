#pragma once

namespace mbc::trading
{

    //! Method by which short positions are assigned to an exercise notice during exercise and assignment processing.
    //! Modelled on FIX field: tag=1049, fieldName=InstrmtAssignmentMethod, xmlName=@AsgnMeth, dataType=Char.
    enum class InstrumentAssignmentMethod
    {
        //! Method is undefined.
        Undefined,
        //! Random assignment method (FIX field value 'R').
        Random,
        //! ProRata assignment method (FIX field value 'P').
        ProRata
    };

    //! Denotes the current state of the instrument. Used for derivatives.
    enum class InstrumentSecurityStatus
    {
        //! Status is undefined.
        Undefined = 0,
        //! Status is active.
        Active = '1',
        //! Status is inactive.
        Inactive = '2'
    };
}
