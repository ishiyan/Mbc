#include "mbc/testharness_shell.h"
#include "crtDebug.h"

int main(const int argc, char* argv[]) // NOLINT(bugprone-exception-escape)
{
    #if defined(_DEBUG)
	setWindowsCrtDebugFlags();
    #endif

    return  th::shell::run_tests(argc, argv);
}
