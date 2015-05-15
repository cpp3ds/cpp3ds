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

	sf::RenderTexture topLeftScreen, topRightScreen, bottomScreen;

	QSFMLCanvas(QWidget* Parent);
    virtual ~QSFMLCanvas();

    bool pollMouseEvent(sf::Event& event);

private :

//    virtual void OnInit();
//
//    virtual void OnUpdate();

    virtual QPaintEngine* paintEngine() const;

    virtual void showEvent(QShowEvent*);
    virtual void resizeEvent (QResizeEvent* event);
    virtual void paintEvent(QPaintEvent*);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual int heightForWidth(int w);

    void pushMouseEvent(const sf::Event& event);

    bool   myInitialized;
    int paintCount;
    std::queue<sf::Event> m_events;
};

#endif
