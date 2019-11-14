#pragma once

#define HAS_NOEXCEPT

#ifndef HAS_NOEXCEPT
#define noexcept throw()
#endif
