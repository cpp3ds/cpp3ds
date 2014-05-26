#include <iostream>
#include <SFML/Graphics.hpp>
#include <cpp3ds/Emulator/Emulator.hpp>

namespace cpp3ds {

	Emulator::Emulator(QWidget *parent): QMainWindow(parent){
		setupUi(this);

		thread = new sf::Thread(&Emulator::runGame, this);

//		float ratio = (float)logo->get_width() / logo->get_height();
//		aboutDialog->set_logo(logo->scale_simple(100*ratio,100,Gdk::INTERP_BILINEAR));

		// Create and add a SFML widget
		screen = new QSFMLCanvas(nullptr, QPoint(0, 0), QSize(400, 480));
//		boxSFML->pack_start(*screen, true, true);
		verticalLayout->addWidget(screen);
//		screen->show();
		screen->setFramerateLimit(60);

		slider3D = new QSlider();
		slider3D->setOrientation(Qt::Horizontal);
		toolBar->insertWidget(actionVolume, slider3D);

		spacer = new QWidget();
		spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		toolBar->insertWidget(actionVolume, spacer);

		pausedFrameTexture.create(800 + EMU_OUTLINE_THICKNESS*2, 480 + EMU_OUTLINE_THICKNESS*2);

	}

	Emulator::~Emulator(){
		stop();
		delete thread;
		delete screen;
		delete slider3D;
	}

	// Meant to be run in another thread or will block the GUI.
	void Emulator::runGame(){
		isThreadRunning = true;
		std::cout << "Emulation starting..." << std::endl;
		cpp3ds_main();
		std::cout << "Emulation ended. " << std::endl;
		screen->setActive(false);
		isThreadRunning = false;
	}

	void Emulator::checkThreadState(){
		// Check to see if thread ended but GUI hasn't updated
		// (Can't easily update GUI inside thread)
//		if (!isThreadRunning && buttonStop->get_sensitive())
//			on_stop_clicked();
	}

	void Emulator::play() {
		if (state == EMU_PLAYING)
			return;
		state = EMU_PLAYING;
		screen->setActive(false);

		// Clear event queue
		sf::Event event;
		while (screen->pollEvent(event)) {}

		if (!isThreadRunning)
			thread->launch();
	}

	void Emulator::pause() {
		state = EMU_PAUSED;
	}

	void Emulator::updatePausedFrame(){
		pausedFrameTexture.update(*screen);
		pausedFrame.setTexture(pausedFrameTexture, true);
		screen->setActive(false);
	}

	void Emulator::stop() {
		if (state == EMU_STOPPED)
			return;
		// Set state first to trigger thread stop
		state = EMU_STOPPED;
		thread->wait();
	}

	float Emulator::get_slider3d(){
		sf::Lock lock(mutex);
//		if (!buttonToggle3D->get_active())
			return 0;
//		return scale3D->get_value();
	}

	void Emulator::saveScreenshot(){
//		if (state == SIM_PLAYING)
//			on_playpause_clicked();
//		saveDialog->set_current_name("screenshot.png");
//		if (saveDialog->run() == Gtk::RESPONSE_OK){
//			sf::Image screenie = screen->renderWindow.capture();
//			screen->renderWindow.setActive(false);
//			std::cout << "Saving screenshot to "
//				<< saveDialog->get_filename() << std::endl;
//			screenie.saveToFile(saveDialog->get_filename());
//		}
//		saveDialog->close();
	}

	void Emulator::drawPausedFrame(){
		if (!initialized){
			initialized = true;
			screen->clear();
			// TODO: Make pausedFrame the cpp3ds logo?
			screen->display();
//			pausedFrameTexture.update(screen->renderWindow);
			updatePausedFrame();
		}

		// If paused/stopped, redraw paused frame
		if (state != EMU_PLAYING){
			screen->clear();
			screen->draw(pausedFrame);
			screen->display();
		}
	}

	/***********************
	  UI Events
	 ***********************/

	void Emulator::on_pushButton_clicked() {
//		buttonStop->set_sensitive(true);
		if (state != EMU_PLAYING){
//			buttonPlayPause->set_icon_name("gtk-media-pause");
			play();
		} else {
//			buttonPlayPause->set_icon_name("gtk-media-play-ltr");
			pause();
		}
	}

	void Emulator::on_toolBar_orientationChanged(Qt::Orientation orientation){
		slider3D->setOrientation(orientation);
		if (orientation == Qt::Horizontal){
			slider3D->setMinimumWidth(50);
			slider3D->setMaximumWidth(150);
			slider3D->setMinimumHeight(0);
			slider3D->setMaximumHeight(200);
		} else {
			slider3D->setMinimumWidth(0);
			slider3D->setMaximumWidth(200);
			slider3D->setMinimumHeight(50);
			slider3D->setMaximumHeight(150);
		}
	}
/*
	void Simulator::on_sfml_size_allocate(Gtk::Allocation& allocation){
		bool dualscreen = (get_slider3d() != 0);
		if (!dualscreen){
			sf::View view;
			view.reset(sf::FloatRect(0, 0, 400, 480));
			screen->renderWindow.setView(view);
		} else {
			screen->renderWindow.setView(screen->renderWindow.getDefaultView());
		}
		// Dirty hack to trigger event AFTER resize
		Glib::signal_timeout().connect_once(sigc::mem_fun(*this,
			&Simulator::drawPausedFrame ), 20);

		// Won't give proper focus without this for some reason
		// (for key_press events)
		screen->grab_focus();
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

	void Simulator::on_stop_clicked(){
		buttonStop->set_sensitive(false);
		buttonPlayPause->set_sensitive(false);
		buttonPlayPause->set_icon_name("gtk-media-play-ltr");
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
		screen->renderWindow.clear();
		screen->renderWindow.display();
//		drawPausedFrame();
		std::cout << scale3D->get_value() << std::endl;
	}
*/
}
