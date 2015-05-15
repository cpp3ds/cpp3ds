#ifndef CPP3DS_EMULATOR_EMU_HPP
#define CPP3DS_EMULATOR_EMU_HPP

#include <QtWidgets>
#include <SFML/Graphics.hpp>
#include <cpp3ds/Emulator/ui_emulator.h>
#include <cpp3ds/Emulator/SFMLWidget.hpp>

#define EMU_OUTLINE_THICKNESS 1

//extern void cpp3ds_main();
extern "C" int __real_main(int argc, char **argv);

namespace cpp3ds {

enum EmulatorState {
	EMU_PLAYING,
	EMU_PAUSED,
	EMU_STOPPED
};

class Emulator : public QMainWindow, private Ui::EmuWindow{
	Q_OBJECT
private:
	QSlider* slider3D;
	QWidget* spacer;

	sf::Thread* thread;
	sf::Mutex mutex;

	sf::Texture pausedFrameTexture;
	sf::Sprite pausedFrame;

	EmulatorState state = EMU_STOPPED;

	bool initialized = false;

	void runGame();
	void checkThreadState();
	void saveScreenshot();

	void drawPausedFrame();

private slots:
	void on_actionScreenshot_triggered(bool checked = false);
	void on_actionToggle_3D_triggered(bool checked = false);
	void on_actionPlay_Pause_triggered(bool checked = false);
	void on_actionStop_triggered(bool checked = false);
	void on_toolBar_orientationChanged(Qt::Orientation orientation);

public:
	QSFMLCanvas *screen;

	bool isThreadRunning = false;

	Emulator(QWidget *parent = 0);
	~Emulator();
	void run();
	void play();
	void pause();
	void stop();
	float get_slider3d();
	void updatePausedFrame();

	EmulatorState getState(){ return state; }
};

// Emulator global to be accessed in-game
extern Emulator* _emulator;

}

#endif
