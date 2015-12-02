#ifndef CPP3DS_SFMLWIDGET_HPP
#define CPP3DS_SFMLWIDGET_HPP

#include <queue>
#include <QWidget>
#include <QTimer>
#include <QResizeEvent>
#include <QMouseEvent>
#include <SFML/Graphics.hpp>

class QSFMLCanvas: public QWidget, public sf::RenderWindow
{
public :
	QSFMLCanvas(QWidget* Parent);
    virtual ~QSFMLCanvas();

    bool pollEvent(sf::Event& event);

private :
    virtual QPaintEngine* paintEngine() const;

    virtual void showEvent(QShowEvent*);
    virtual void resizeEvent (QResizeEvent* event);
    virtual void paintEvent(QPaintEvent*);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void keyPressEvent(QKeyEvent *);
	virtual void keyReleaseEvent(QKeyEvent *);
    virtual int heightForWidth(int w);

    void pushEvent(const sf::Event& event);

    bool   myInitialized;
    int paintCount;
    std::queue<sf::Event> m_events;
};

#endif

////////////////////////////////////////////////////////////
/// \class QSFMLCanvas
/// \ingroup emulator
////////////////////////////////////////////////////////////
