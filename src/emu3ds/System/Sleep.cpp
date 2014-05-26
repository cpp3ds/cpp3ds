#include <cpp3ds/System/Sleep.hpp>
#include <cpp3ds/System/Time.hpp>
#include <SFML/System/Sleep.hpp>

namespace cpp3ds {
////////////////////////////////////////////////////////////
void sleep(Time duration) {
	sf::Time time = sf::milliseconds(duration.asMilliseconds());
	sf::sleep(time);
}

}
