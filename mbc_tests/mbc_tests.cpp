#include "mbc/testharness_shell.h"

//! Clears the Windows-specific CRT debugging flags.
//! You can call this during the initialization of your unit tests when they are executed in bulk in debug mode.
void clearWindowsCrtDebugFlags();

//! Sets the partial Windows-specific CRT debugging flags.
//! You can call this during the initialization of your unit tests when they are executed in bulk to track the memory allocation in debug mode.
//! The checking is relatively fast.
void setWindowsCrtDebugFlags();

//! Sets the full Windows-specific CRT debugging flags.
//! You can call this to track the memory allocation of in debug mode.
//! It is useful when you execute only single unit test, but for bulk test execution it slows down performance to almost unacceptable level.
void setWindowsCrtDebugFlagsFull();

int main(const int argc, char* argv[]) // NOLINT(bugprone-exception-escape)
{
#if defined(_DEBUG)
    setWindowsCrtDebugFlags();
#endif

    return  th::shell::run_tests(argc, argv);
}
