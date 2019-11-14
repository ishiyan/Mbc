#pragma once

#include "visibility.h"

#ifdef __cplusplus
extern "C" {
#endif

    //! Clears the Windows-specific CRT debugging flags.
    //! You can call this during the initialization of your unit tests when they are executed in bulk in debug mode.
    DLL_PUBLIC void DLL_CC clearWindowsCrtDebugFlags();

    //! Sets the partial Windows-specific CRT debugging flags.
    //! You can call this during the initialization of your unit tests when they are executed in bulk to track the memory allocation in debug mode.
    //! The checking is relatively fast.
    DLL_PUBLIC void DLL_CC setWindowsCrtDebugFlags();

    //! Sets the full Windows-specific CRT debugging flags.
    //! You can call this to track the memory allocation of in debug mode.
    //! It is useful when you execute only single unit test, but for bulk test execution it slows down performance to almost unacceptable level.
    DLL_PUBLIC void DLL_CC setWindowsCrtDebugFlagsFull();

#ifdef __cplusplus
}
#endif
