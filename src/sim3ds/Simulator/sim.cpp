#include <iostream>
#include <gtkmm.h>
#include <cpp3ds/Simulator/Simulator.hpp>
#include <cpp3ds/Simulator/simulator-res.h>

namespace cpp3ds {
	Simulator* _simulator;
}

extern "C" int __wrap_main(int argc, char **argv){
	// Loads embedded resources (see: /res/sim/ directory)
	g_resources_register(simulator_get_resource());
	
	Gtk::Main kit(argc, argv);
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "cpp3ds.simulator");
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create();
	try {
		builder->add_from_resource("/org/cpp3ds/ui/simulator.glade");
	}catch(const Gio::ResourceError& ex){
		std::cerr << "ResourceError: " << ex.what() << std::endl;
		return 1;
	}catch(const Glib::MarkupError& ex){
		std::cerr << "MarkupError: " << ex.what() << std::endl;
		return 1;
	}catch(const Gtk::BuilderError& ex){
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		return 1;
	}

	// Define global pointer to simulator
	cpp3ds::_simulator = new cpp3ds::Simulator(app, builder);
	cpp3ds::_simulator->run();

	delete cpp3ds::_simulator;
	
	return 0;
}
