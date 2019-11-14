#if defined(_WIN32)
#include <crtdbg.h>
#endif
#include "../include/crtDebug.h"

#ifdef __cplusplus
extern "C" {
#endif

    // ----------------------------------------------------------------------
    // Msvc-specific crt debug utilities
    // ----------------------------------------------------------------------

    DLL_PUBLIC void DLL_CC clearWindowsCrtDebugFlags()
    {
#if defined(_WIN32)
        _CrtSetDbgFlag(0);
        _CrtSetReportMode(_CRT_ASSERT, 0);
        _CrtSetReportMode(_CRT_WARN, 0);
        _CrtSetReportMode(_CRT_ERROR, 0);
#endif
    }
    
    DLL_PUBLIC void DLL_CC setWindowsCrtDebugFlags()
    {
#if defined(_WIN32)
        _CrtSetDbgFlag(
            // Enable debug heap allocations and use memory block type identifiers.
            _CRTDBG_ALLOC_MEM_DF |

            // Call _CrtCheckMemory at every allocation and deallocation request.
            // _CRTDBG_CHECK_ALWAYS_DF | // slows down execution a lot use '--timeout 4000000' argument if you enable it

            // Perform automatic leak checking at program exit through a call to _CrtDumpMemoryLeaks and generate an
            // error report if the application failed to free all the memory it allocated.
            _CRTDBG_LEAK_CHECK_DF |

            // Include _CRT_BLOCK types in leak detection and memory state difference operations.
            // _CRTDBG_CHECK_CRT_DF | // this detects a lot of leakage in CRT code

            // Keep freed memory blocks in the heap's linked list, assign them the
            // _FREE_BLOCK type, and fill them with the byte value 0xDD.
            _CRTDBG_DELAY_FREE_MEM_DF
        );

        // Redirect the output to both the debugger and the message box
        // ReSharper disable CommentTypo
        _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG /*| _CRTDBG_MODE_WNDW*/);
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG /*| _CRTDBG_MODE_WNDW*/);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG /*| _CRTDBG_MODE_WNDW*/);

#endif
    }

    DLL_PUBLIC void DLL_CC setWindowsCrtDebugFlagsFull()
    {
#if defined(_WIN32)
        _CrtSetDbgFlag(
            // Enable debug heap allocations and use memory block type identifiers.
            _CRTDBG_ALLOC_MEM_DF |

            // Call _CrtCheckMemory at every allocation and deallocation request.
            _CRTDBG_CHECK_ALWAYS_DF | // slows down execution a lot use '--timeout 4000000' argument if you enable it

            // Perform automatic leak checking at program exit through a call to _CrtDumpMemoryLeaks and generate an
            // error report if the application failed to free all the memory it allocated.
            _CRTDBG_LEAK_CHECK_DF |

            // Include _CRT_BLOCK types in leak detection and memory state difference operations.
            _CRTDBG_CHECK_CRT_DF | // this detects a lot of leakage in CRT code

            // Keep freed memory blocks in the heap's linked list, assign them the
            // _FREE_BLOCK type, and fill them with the byte value 0xDD.
            _CRTDBG_DELAY_FREE_MEM_DF
        );

        // Redirect the output to both the debugger and the message box
        // ReSharper disable CommentTypo
        _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG /*| _CRTDBG_MODE_WNDW*/);
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG /*| _CRTDBG_MODE_WNDW*/);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG /*| _CRTDBG_MODE_WNDW*/);

#endif
    }

#ifdef __cplusplus
}
#endif
