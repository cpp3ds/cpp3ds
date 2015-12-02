////////////////////////////////////////////////////////////
/// \mainpage
///
/// This documentation is not yet complete. Most of it is generated from the SFML source base. Visit <a href="http://www.sfml-dev.org/">www.sfml-dev.org</a> for more information on that project.
///
/// \section example Hello World Template
/// There is a project template here: https://github.com/cpp3ds/cpp3ds-template
///
/// This is the code:
///
/// \code
///
/// #include <cpp3ds/Graphics.hpp>
/// #include <cpp3ds/System.hpp>
/// #include <cpp3ds/Window.hpp>
///
///
/// // Minimal derived Game class necessary for cpp3ds games
/// class MyGame: public cpp3ds::Game {
/// private:
/// 	cpp3ds::Text text;
/// public:
/// 	MyGame() {
/// 		// Constructor for setting up initial game objects
/// 		text.setColor(cpp3ds::Color::Green);
/// 		text.setCharacterSize(40);
/// 		text.setString(_("Hello World"));
/// 	}
///
/// 	~MyGame() {
/// 		// Destructor called when game exits
/// 	}
///
/// 	void update(float delta) {
/// 		// Update game state based on frame time delta
/// 		text.move(10.f * delta, 0); // Move 10 pixels per second to the right
/// 	}
///
/// 	void processEvent(cpp3ds::Event& event) {
/// 		// Check for input Events
/// 		switch (event.type) {
/// 			case cpp3ds::Event::KeyPressed:
/// 				if (event.key.code == cpp3ds::Keyboard::Select)
/// 					exit(); // Exit game when Select is pressed
/// 				break;
/// 			default:
/// 				break;
/// 		}
/// 	}
///
/// 	void renderTopScreen(cpp3ds::Window& window) {
/// 		window.clear();
/// 		window.draw(text);
/// 	}
///
/// 	void renderBottomScreen(cpp3ds::Window& window) {
/// 		window.clear();
/// 	}
/// };
///
///
/// int main(int argc, char** argv) {
/// 	MyGame game;
/// 	game.console(cpp3ds::BottomScreen); // Console for reading stdout
/// 	game.run();
/// 	return 0;
/// }
/// \endcode
////////////////////////////////////////////////////////////
