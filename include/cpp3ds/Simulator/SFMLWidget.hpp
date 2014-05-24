#ifndef CPP3DS_SFMLWIDGET_HPP
#define CPP3DS_SFMLWIDGET_HPP

#include <SFML/Graphics.hpp>
#include <QWidget>
#include <QTimer>

class QSFMLCanvas: public QWidget, public sf::RenderWindow
{
public :

	sf::RenderTexture topLeftScreen, topRightScreen, bottomScreen;

	QSFMLCanvas(QWidget* Parent, const QPoint& Position, const QSize& Size, unsigned int FrameTime = 0);

    virtual ~QSFMLCanvas();

private :

//    virtual void OnInit();
//
//    virtual void OnUpdate();

    virtual QPaintEngine* paintEngine() const;

    virtual void showEvent(QShowEvent*);

    virtual void paintEvent(QPaintEvent*);

    QTimer myTimer;
    bool   myInitialized;
};

#endif
