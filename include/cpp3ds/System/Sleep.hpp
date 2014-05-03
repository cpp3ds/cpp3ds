#ifndef CPP3DS_SLEEP_HPP
#define CPP3DS_SLEEP_HPP

#include <cpp3ds/System/Time.hpp>


namespace cpp3ds {
////////////////////////////////////////////////////////////
/// \ingroup system
/// \brief Make the current thread sleep for a given duration
///
/// cpp3ds::sleep is the best way to block a program or one of its
/// threads, as it doesn't consume any CPU power.
///
/// \param duration Time to sleep
///
////////////////////////////////////////////////////////////
void sleep(Time duration);

}


#endif
