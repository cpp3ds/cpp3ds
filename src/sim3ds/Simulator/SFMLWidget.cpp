#include <iostream>
#include <gtkmm/widget.h>
#include <cpp3ds/Window/TopScreen.hpp>
#include <cpp3ds/Window/BottomScreen.hpp>
#include <cpp3ds/Simulator/SFMLWidget.hpp>

// Tested on Linux Mint 12.4 and Windows 7
#if defined(SFML_SYSTEM_WINDOWS)

#include <gdk/gdkwin32.h>
#define GET_WINDOW_HANDLE_FROM_GDK GDK_WINDOW_HANDLE

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD)

#include <gdk/gdkx.h>
#define GET_WINDOW_HANDLE_FROM_GDK GDK_WINDOW_XID

#elif defined(SFML_SYSTEM_MACOS)

#error Note: You have to figure out an analogue way to access the handle of the widget on a Mac-System

#else

// #error Unsupported Operating System

#endif


SFMLWidget::SFMLWidget(sf::VideoMode mode, int size_request)
{
	if(size_request<=0)
		size_request = std::max<int>(1, std::min<int>(mode.width, mode.height) / 2);

	set_size_request(size_request, size_request);

	set_has_window(false); // Makes this behave like an internal object rather then a parent window.

	topLeftScreen.create(TOP_WIDTH, TOP_HEIGHT, true);
	topRightScreen.create(TOP_WIDTH, TOP_HEIGHT, true);
	bottomScreen.create(BOTTOM_WIDTH, BOTTOM_HEIGHT, true);
}

SFMLWidget::~SFMLWidget(){}

void SFMLWidget::on_size_allocate(Gtk::Allocation& allocation)
{
	//Do something with the space that we have actually been given:
	//(We will not be given heights or widths less than we have requested, though
	//we might get more)

	this->set_allocation(allocation);

	if(m_refGdkWindow)
	{
		m_refGdkWindow->move_resize(allocation.get_x(),
		                            allocation.get_y(),
		                            allocation.get_width(),
		                            allocation.get_height() );
		renderWindow.setSize(sf::Vector2u(allocation.get_width(), allocation.get_height()));
	}
}

void SFMLWidget::on_realize(){
	Gtk::Widget::on_realize();

	if(!m_refGdkWindow){
		//Create the GdkWindow:
		GdkWindowAttr attributes;
		memset(&attributes, 0, sizeof(attributes));

		Gtk::Allocation allocation = get_allocation();

		//Set initial position and size of the Gdk::Window:
		attributes.x = allocation.get_x();
		attributes.y = allocation.get_y();
		attributes.width = 800;
		attributes.height = allocation.get_height();

		attributes.event_mask = get_events () | Gdk::EXPOSURE_MASK;
		attributes.window_type = GDK_WINDOW_CHILD;
		attributes.wclass = GDK_INPUT_OUTPUT;


		m_refGdkWindow = Gdk::Window::create(get_window(), &attributes, GDK_WA_X | GDK_WA_Y);
		set_has_window(true);
		set_window(m_refGdkWindow);

		// transparent background
		this->unset_background_color();
		this->set_double_buffered(false);
		this->set_can_focus(true);

		//make the widget receive expose events
		m_refGdkWindow->set_user_data(gobj());

		renderWindow.create(GET_WINDOW_HANDLE_FROM_GDK(m_refGdkWindow->gobj()));
	}
}

void SFMLWidget::on_unrealize(){
	m_refGdkWindow.clear();
	Gtk::Widget::on_unrealize();
}

void SFMLWidget::display(){
	if(m_refGdkWindow)
		renderWindow.display();
}

void SFMLWidget::invalidate(){
	if(m_refGdkWindow)
		m_refGdkWindow->invalidate(true);
}

void SFMLWidget::show3D(){
	//
}
