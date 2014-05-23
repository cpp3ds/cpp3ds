#include <QApplication>
#include <cpp3ds/Simulator.hpp>

namespace cpp3ds {
	Simulator* _simulator;
}

int main(int argc, char **argv){
	QApplication a(argc, argv);

	// Force resource init because it's in archive lib
	Q_INIT_RESOURCE(emulator);

	// Define global pointer to simulator
	cpp3ds::_simulator = new cpp3ds::Simulator();
	cpp3ds::_simulator->show();

	return a.exec();
}
