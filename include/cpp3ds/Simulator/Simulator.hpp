#ifndef CPP3DS_SIMULATOR_HPP
#define CPP3DS_SIMULATOR_HPP

#include <gtkmm.h>
#include <cpp3ds/Simulator/SFMLWidget.hpp>

#define SIM_OUTLINE_THICKNESS 1

namespace cpp3ds {

	enum SimulatorState {
		SIM_PLAYING,
		SIM_PAUSED,
		SIM_STOPPED
	};

	class Simulator{

	private:
		Glib::RefPtr<Gtk::Application> application;
		Gtk::Window *window;
		Gtk::Image *imageLogo;
		Gtk::MenuItem *menuAbout;
		Gtk::AboutDialog *aboutDialog;
		Gtk::FileChooserDialog *saveDialog;
		Gtk::Box *boxSFML;
		Gtk::ToolButton *buttonScreenshot, *buttonPlayPause, *buttonStop, *buttonTest;
		Gtk::ToggleToolButton *buttonToggle3D;
		Gtk::Scale *scale3D;

		sf::Thread* thread;
		sf::Mutex mutex;

		sf::Texture pausedFrameTexture;
		sf::Sprite pausedFrame;

		SimulatorState state = SIM_STOPPED;

		void runGame();
		void checkThreadState();
		void saveScreenshot();

		void drawPausedFrame();

		void on_sfml_size_allocate(Gtk::Allocation& allocation);
		bool on_my_delete_event(GdkEventAny* event);
		void on_about_response(int response_id);
		void on_playpause_clicked();
		void on_stop_clicked();
		void on_toggle3d_clicked();
		void on_about_clicked();
		void on_test_clicked();

	public:
		SFMLWidget *screen;
		
		bool isThreadRunning = false;

		Simulator(Glib::RefPtr<Gtk::Application> app, Glib::RefPtr<Gtk::Builder> builder);
		~Simulator();
		void run();
		void play();
		void pause();
		void stop();
		float get_slider3d();
		void updatePausedFrame();

		SimulatorState getState(){ return state; }
	};

	// Simulator global to be accessed in-game
	extern Simulator* _simulator;

}

#endif
