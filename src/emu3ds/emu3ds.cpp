#include <QApplication>
#include <cpp3ds/Emulator.hpp>

namespace cpp3ds {
	// Define global pointer to emulator
	Emulator* _emulator;
}

int main(int argc, char **argv){
	QApplication a(argc, argv);

	// Force resource init because it's in archive lib
	Q_INIT_RESOURCE(emulator);

	cpp3ds::_emulator = new cpp3ds::Emulator();
	cpp3ds::_emulator->show();

	int ret = a.exec();

	delete cpp3ds::_emulator;
	return ret;
}
