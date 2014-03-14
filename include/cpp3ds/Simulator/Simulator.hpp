#ifndef CPP3DS_SIMULATOR_HPP
#define CPP3DS_SIMULATOR_HPP

#include <gtkmm.h>
#include <cpp3ds/Simulator/SFMLWidget.hpp>

#define SIM_OUTLINE_THICKNESS 1

namespace cpp3ds {

	class Simulator{

	private:
		Glib::RefPtr<Gtk::Application> application;
		Gtk::Window *window;
		Gtk::MenuItem *menuAbout;
		Gtk::AboutDialog *aboutDialog;
		Gtk::FileChooserDialog *saveDialog;
		Gtk::Box *boxSFML;
		Gtk::ToolButton *buttonScreenshot, *buttonPlayPause, *buttonStop;
		Gtk::ToggleToolButton *buttonToggle3D;
		Gtk::Scale *scale3D;

		sf::Thread* thread;

		sf::Texture pausedFrameTexture;
		sf::Sprite pausedFrame;

		void runGame();
		void checkThreadState();
		void saveScreenshot();

		void drawPausedFrame();

		void on_sfml_size_allocate(Gtk::Allocation& allocation);
		void on_playpause_clicked();
		void on_stop_clicked();
		void on_toggle3d_clicked();
		void on_about_clicked();

	public:
		SFMLWidget *screen;
		sf::Mutex mutex;

		bool triggerStop = false;
		bool isPaused = true;
		bool isThreadRunning = false;

		Simulator(Glib::RefPtr<Gtk::Application> app, Glib::RefPtr<Gtk::Builder> builder);
		~Simulator();
		void run();
		void play();
		void pause();
		void stop();
		float get_slider3d();

	};

	// Simulator global to be accessed in-game
	extern Simulator* _simulator;

}

#endif
