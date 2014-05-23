#include <iostream>
//#include <gtkmm/widget.h>
#include <cpp3ds/Window/TopScreen.hpp>
#include <cpp3ds/Window/BottomScreen.hpp>
#include <cpp3ds/Simulator/SFMLWidget.hpp>


SFMLWidget::SFMLWidget()
{
	topLeftScreen.create(TOP_WIDTH, TOP_HEIGHT, true);
	topRightScreen.create(TOP_WIDTH, TOP_HEIGHT, true);
	bottomScreen.create(BOTTOM_WIDTH, BOTTOM_HEIGHT, true);
}

SFMLWidget::~SFMLWidget(){}

/*
void SFMLWidget::on_size_allocate(Gtk::Allocation& allocation)
{
	//Do something with the space that we have actually been given:
	//(We will not be given heights or widths less than we have requested, though
	//we might get more)

	this->set_allocation(allocation);

	if (m_refGdkWindow) {
		m_refGdkWindow->move_resize(allocation.get_x(),
		                            allocation.get_y(),
		                            allocation.get_width(),
		                            allocation.get_height() );
		renderWindow.setSize(sf::Vector2u(allocation.get_width(), allocation.get_height()));
		m_refGdkWindow->focus(0);
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

		//make the widget receive expose events
		attributes.event_mask = get_events();//Gdk::ALL_EVENTS_MASK;
		attributes.window_type = GDK_WINDOW_CHILD;
		attributes.wclass = GDK_INPUT_OUTPUT;


		m_refGdkWindow = Gdk::Window::create(get_window(), &attributes, GDK_WA_X | GDK_WA_Y);
		set_has_window(true);
		set_window(m_refGdkWindow);

		// transparent background
		unset_background_color();
		set_double_buffered(false);
		set_can_focus(true);

		m_refGdkWindow->set_user_data(gobj());

//		renderWindow.create(GET_WINDOW_HANDLE_FROM_GDK(m_refGdkWindow->gobj()));
		renderWindow.create(sf::VideoMode(800, TOP_HEIGHT*2), "SFML window");
	}
}

void SFMLWidget::on_unrealize(){
	m_refGdkWindow.clear();
	Gtk::Widget::on_unrealize();
}
*/
void SFMLWidget::display(){
//	if(m_refGdkWindow)
		renderWindow.display();
}

void SFMLWidget::invalidate(){
//	if(m_refGdkWindow)
//		m_refGdkWindow->invalidate(true);
}

void SFMLWidget::show3D(){
	//
}

