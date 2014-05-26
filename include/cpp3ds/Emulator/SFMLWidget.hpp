#ifndef CPP3DS_SFMLWIDGET_HPP
#define CPP3DS_SFMLWIDGET_HPP

#include <queue>
#include <SFML/Graphics.hpp>
#include <QWidget>
#include <QTimer>

class QSFMLCanvas: public QWidget, public sf::RenderWindow
{
public :

	sf::RenderTexture topLeftScreen, topRightScreen, bottomScreen;

	QSFMLCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime = 0);
    virtual ~QSFMLCanvas();

    bool pollMouseEvent(sf::Event& event);

private :

//    virtual void OnInit();
//
//    virtual void OnUpdate();

    virtual QPaintEngine* paintEngine() const;

    virtual void showEvent(QShowEvent*);
    virtual void paintEvent(QPaintEvent*);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);

    void pushMouseEvent(const sf::Event& event);

    QTimer myTimer;
    bool   myInitialized;
    std::queue<sf::Event> m_events;
};

#endif
