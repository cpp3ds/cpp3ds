#include <iostream>
#include <gtkmm.h>
#include <cpp3ds/Simulator/Simulator.hpp>

// Should never be executed. Only defined so the linker sees the
// symbol in the absence of the --wrap option.
extern "C" int __real_main();

namespace cpp3ds {

	Simulator::Simulator(Glib::RefPtr<Gtk::Application> app, Glib::RefPtr<Gtk::Builder> builder){
		application = app;
		thread = new sf::Thread(&Simulator::runGame, this);
		builder->get_widget("window", window);
		builder->get_widget("boxSFML", boxSFML);

		builder->get_widget("aboutDialog", aboutDialog);
		builder->get_widget("imageLogo", imageLogo);
		Glib::RefPtr<Gdk::Pixbuf> logo = imageLogo->get_pixbuf();
		float ratio = (float)logo->get_width() / logo->get_height();
		aboutDialog->set_logo(logo->scale_simple(100*ratio,100,Gdk::INTERP_BILINEAR));

		
		builder->get_widget("saveDialog", saveDialog);
		saveDialog->add_button("gtk-cancel", Gtk::RESPONSE_CANCEL);
		saveDialog->add_button("gtk-save", Gtk::RESPONSE_OK);

		builder->get_widget("menuAbout", menuAbout);
		builder->get_widget("buttonTest", buttonTest);
		builder->get_widget("buttonScreenshot", buttonScreenshot);
		builder->get_widget("buttonPlayPause", buttonPlayPause);
		builder->get_widget("buttonStop", buttonStop);
		builder->get_widget("buttonToggle3D", buttonToggle3D);
		builder->get_widget("scale3D", scale3D);

		// Create and add a SFMLWidget
		screen = new SFMLWidget(sf::VideoMode(800 + SIM_OUTLINE_THICKNESS*2, 480 + SIM_OUTLINE_THICKNESS*2));
		boxSFML->pack_start(*screen, true, true);
		screen->show();

		// TODO: Make pausedFrame the cpp3ds logo?
		pausedFrameTexture.create(800 + SIM_OUTLINE_THICKNESS*2, 480 + SIM_OUTLINE_THICKNESS*2);
		screen->renderWindow.clear();
		pausedFrameTexture.update(screen->renderWindow);
		pausedFrame.setTexture(pausedFrameTexture);

		screen->renderWindow.setFramerateLimit(60);

		// Connect all GTK signals
		Glib::signal_timeout().connect(sigc::bind_return(sigc::mem_fun(*this,
			&Simulator::checkThreadState ),true),200);

		menuAbout->signal_activate().connect(sigc::mem_fun(*this,
			&Simulator::on_about_clicked ));
		aboutDialog->signal_response().connect(sigc::mem_fun(*this,
			&Simulator::on_about_response ));

		screen->signal_size_allocate().connect(sigc::mem_fun(*this,
			&Simulator::on_sfml_size_allocate ));

		buttonTest->signal_clicked().connect(sigc::mem_fun(*this,
			&Simulator::on_test_clicked ));
		buttonScreenshot->signal_clicked().connect(sigc::mem_fun(*this,
			&Simulator::saveScreenshot ));
		buttonPlayPause->signal_clicked().connect(sigc::mem_fun(*this,
			&Simulator::on_playpause_clicked ));
		buttonStop->signal_clicked().connect(sigc::mem_fun(*this,
			&Simulator::on_stop_clicked ));
		buttonToggle3D->signal_toggled().connect(sigc::mem_fun(*this,
			&Simulator::on_toggle3d_clicked ));
	}

	Simulator::~Simulator(){
		stop();
		delete thread;
		delete screen;
	}

	void Simulator::run(){
		application->run(*window);
	}

	// Meant to be run in another thread or will block the GUI.
	void Simulator::runGame(){
		isThreadRunning = true;
		std::cout << "Simulation starting..." << std::endl;
		int ret = __real_main();
		std::cout << "Simulation ended." << std::endl;
		screen->renderWindow.setActive(false);
		isThreadRunning = false;
	}

	void Simulator::checkThreadState(){
		// Check to see if thread ended but GUI hasn't updated
		// (Can't easily update GUI inside thread)
		if (!isThreadRunning && buttonStop->get_sensitive())
			on_stop_clicked();
	}

	void Simulator::play() {
		if (state == SIM_PLAYING) return;
		state = SIM_PLAYING;
		screen->renderWindow.setActive(false);
		if (!isThreadRunning)
			thread->launch();
	}

	void Simulator::pause() {
		state = SIM_PAUSED;
	}

	void Simulator::stop() {
		if (state == SIM_STOPPED) return;
		// Set state first to trigger thread stop
		state = SIM_STOPPED;
		thread->wait();
		screen->renderWindow.setActive(true);
		pausedFrameTexture.update(screen->renderWindow);
		pausedFrame.setTexture(pausedFrameTexture, true);
	}

	float Simulator::get_slider3d(){
		sf::Lock lock(mutex);
		if (!buttonToggle3D->get_active())
			return 0;
		return scale3D->get_value();
	}

	void Simulator::saveScreenshot(){
		if (state == SIM_PLAYING)
			on_playpause_clicked();
		saveDialog->set_current_name("screenshot.png");
		if (saveDialog->run() == Gtk::RESPONSE_OK){
			sf::Image screenie = screen->renderWindow.capture();
			screen->renderWindow.setActive(false);
			std::cout << "Saving screenshot to '"
				<< saveDialog->get_filename() << "'... " << std::endl;
			screenie.saveToFile(saveDialog->get_filename());
		}
		saveDialog->close();
	}

	void Simulator::drawPausedFrame(){
		if (state != SIM_PLAYING){
			screen->renderWindow.clear();
			screen->renderWindow.draw(pausedFrame);
			// If paused, redraw paused frame
			screen->display();
		}
	}

	/***********************
	  UI Events
	 ***********************/

	void Simulator::on_sfml_size_allocate(Gtk::Allocation& allocation){
		// Dirty hack to trigger event AFTER resize
		Glib::signal_timeout().connect_once(sigc::mem_fun(*this,
			&Simulator::drawPausedFrame ),20);
	}

	bool Simulator::on_my_delete_event(GdkEventAny* event){
		if (state != SIM_STOPPED)
			on_stop_clicked();
		return false;
	}

	void Simulator::on_about_response(int response_id){
		// There's only a close button, so what else is there to do?
		aboutDialog->hide();
	}

	void Simulator::on_playpause_clicked(){
		buttonStop->set_sensitive(true);
		if (state != SIM_PLAYING){
			buttonPlayPause->set_icon_name("media-playback-pause");
			play();
		} else {
			buttonPlayPause->set_icon_name("media-playback-start");
			pause();
		}
	}

	void Simulator::on_stop_clicked(){
		buttonStop->set_sensitive(false);
		buttonPlayPause->set_sensitive(false);
		buttonPlayPause->set_icon_name("media-playback-start");
		stop();
		buttonPlayPause->set_sensitive(true);
	}

	void Simulator::on_toggle3d_clicked(){
		sf::Lock lock(mutex);
		bool active = buttonToggle3D->get_active();
		scale3D->set_sensitive(active);
		scale3D->set_opacity(active ? 1.0 : 0.8);
		screen->show3D();
		screen->set_size_request(active ? 800 + SIM_OUTLINE_THICKNESS*2 : 400, 480 + SIM_OUTLINE_THICKNESS*2);
		// Resize window to smallest size allowed
		window->resize(1,1);
	}

	void Simulator::on_about_clicked(){
		aboutDialog->run();
	}

	void Simulator::on_test_clicked(){
		// drawPausedFrame();
		std::cout << scale3D->get_value() << std::endl;
	}

}