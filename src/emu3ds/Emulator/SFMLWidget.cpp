#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif
#include <iostream>
#include <cpp3ds/Window/TopScreen.hpp>
#include <cpp3ds/Window/BottomScreen.hpp>
#include <cpp3ds/Emulator/SFMLWidget.hpp>

QSFMLCanvas::QSFMLCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime) :
QWidget       (Parent),
myInitialized (false)
{
    // Setup some states to allow direct rendering into the widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_PaintUnclipped);

    topLeftScreen.create(TOP_WIDTH, TOP_HEIGHT, true);
	topRightScreen.create(TOP_WIDTH, TOP_HEIGHT, true);
	bottomScreen.create(BOTTOM_WIDTH, BOTTOM_HEIGHT, true);

    // Set strong focus to enable keyboard events to be received
    setFocusPolicy(Qt::WheelFocus);

    this->setMinimumHeight(480);
    this->setMinimumWidth(400);

    // Setup the widget geometry
//    move(Position);
    resize(Size);

    // Setup the timer
    myTimer.setInterval(FrameTime);
}

QSFMLCanvas::~QSFMLCanvas(){}

void QSFMLCanvas::showEvent(QShowEvent*)
{
    if (!myInitialized) {
        // Under X11, we need to flush the commands sent to the server to ensure that
        // SFML will get an updated view of the windows
        #ifdef Q_WS_X11
            XFlush(QX11Info::display());
        #endif

        // Create the SFML window with the widget handle
		RenderWindow::create(winId());

        myInitialized = true;
    }
}

QPaintEngine* QSFMLCanvas::paintEngine() const {
//	We make the paintEvent function return a null paint engine. This functions works together with
//	the WA_PaintOnScreen flag to tell Qt that we're not using any of its built-in paint engines.
	return nullptr;
}

void QSFMLCanvas::paintEvent(QPaintEvent*) {
	RenderWindow::display();
}

void QSFMLCanvas::mousePressEvent(QMouseEvent* event) {
    sf::Event e;
    e.type = sf::Event::MouseButtonPressed;
    e.mouseButton.x = event->x();
    e.mouseButton.y = event->y();
    pushMouseEvent(e);
}
void QSFMLCanvas::mouseMoveEvent(QMouseEvent* event) {
	sf::Event e;
	e.type = sf::Event::MouseMoved;
	e.mouseButton.x = event->x();
	e.mouseButton.y = event->y();
	pushMouseEvent(e);
}
void QSFMLCanvas::mouseReleaseEvent(QMouseEvent* event) {
	sf::Event e;
	e.type = sf::Event::MouseButtonReleased;
	e.mouseButton.x = event->x();
	e.mouseButton.y = event->y();
	pushMouseEvent(e);
}

void QSFMLCanvas::pushMouseEvent(const sf::Event& event) {
	m_events.push(event);
}

bool QSFMLCanvas::pollMouseEvent(sf::Event& event) {
	// Pop the first event of the queue, if it is not empty
	if (!m_events.empty()) {
		event = m_events.front();
		m_events.pop();
		return true;
	}
	return false;
}

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
