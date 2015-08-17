#include <QApplication>
#include <cpp3ds/Emulator.hpp>

namespace cpp3ds {
    // Define global pointer to emulator
    Emulator *_emulator;
}

int wrapper_main(int argc, char **argv) {
    QApplication a(argc, argv);

    // Force resource init because it's in cpp3ds archive lib
    Q_INIT_RESOURCE(emulator);

    cpp3ds::_emulator = new cpp3ds::Emulator();
    cpp3ds::_emulator->show();

    int ret = a.exec();

    delete cpp3ds::_emulator;
    return ret;
}

// Use extern "C" to avoid name mangling and call wrapper_main
// to get back into the glorious C++ world.
extern "C" int __wrap_main(int argc, char **argv) {
    return wrapper_main(argc, argv);
}
