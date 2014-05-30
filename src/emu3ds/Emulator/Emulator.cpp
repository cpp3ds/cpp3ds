#include <iostream>
#include <SFML/Graphics.hpp>
#include <cpp3ds/Emulator/Emulator.hpp>

namespace cpp3ds {

	Emulator::Emulator(QWidget *parent): QMainWindow(parent){
		setupUi(this);

//		float ratio = (float)logo->get_width() / logo->get_height();
//		aboutDialog->set_logo(logo->scale_simple(100*ratio,100,Gdk::INTERP_BILINEAR));

		// Create and add a SFML widget
		screen = new QSFMLCanvas(this);
		screen->setMinimumHeight(480);
		screen->setMinimumWidth(400);
		verticalLayout->addWidget(screen, 0, Qt::AlignCenter);
		screen->setFramerateLimit(60);

		// Add 3D slider in toolbar
		slider3D = new QSlider();
		slider3D->setOrientation(Qt::Horizontal);
		slider3D->setEnabled(false);
		slider3D->setMaximum(1000);
		toolBar->insertWidget(actionVolume, slider3D);
		on_toolBar_orientationChanged(Qt::Horizontal);

		// Add spacer between 3D slider and right-side buttons
		spacer = new QWidget();
		spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		toolBar->insertWidget(actionVolume, spacer);

		thread = new sf::Thread(&Emulator::runGame, this);

		pausedFrameTexture.create(800 + EMU_OUTLINE_THICKNESS*2, 480 + EMU_OUTLINE_THICKNESS*2);
	}

	Emulator::~Emulator(){
		stop();
		delete thread;
		delete screen;
		delete spacer;
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
		if (!isThreadRunning && actionStop->isEnabled())
			actionStop->trigger();
	}

	void Emulator::play() {
		if (state == EMU_PLAYING)
			return;
		state = EMU_PLAYING;
		screen->setActive(false);

		// Clear event queues
		sf::Event event;
		while (screen->pollEvent(event)) {}
		while (screen->pollMouseEvent(event)) {}

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
		if (!slider3D->isEnabled())
			return 0;
		return static_cast<float>(slider3D->value()) / 1000;
	}

	void Emulator::saveScreenshot(){
		if (state == EMU_PLAYING)
			actionPlay_Pause->trigger();
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

	void Emulator::on_actionScreenshot_triggered(bool checked) {
//		this->adjustSize();
//		std::cout << slider3D->value() << std::endl;
		drawPausedFrame();
	}

	void Emulator::on_actionToggle_3D_triggered(bool checked) {
		sf::Lock lock(mutex);
		slider3D->setEnabled(checked);
		if (checked) {
			screen->setMinimumWidth(800);
			sf::View view;
			view.reset(sf::FloatRect(0, 0, 800, 480));
			screen->setView(view);
		} else {
			screen->setMinimumWidth(400);
			screen->setView(screen->getDefaultView());
		}
//		screen->set_size_request(active ? 800 + SIM_OUTLINE_THICKNESS*2 : 400, 480 + SIM_OUTLINE_THICKNESS*2);
		// Resize window to smallest size allowed
		this->adjustSize();
	}

	void Emulator::on_actionPlay_Pause_triggered(bool checked) {
		actionStop->setEnabled(true);
		if (checked){
			play();
		} else {
			pause();
		}
	}

	void Emulator::on_actionStop_triggered(bool checked) {
		actionStop->setEnabled(false);
		actionPlay_Pause->setEnabled(false);
		actionPlay_Pause->setChecked(false);
		stop();
		actionPlay_Pause->setEnabled(true);
	}

}
