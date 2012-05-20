#ifndef PARALLAXOIDWALLPAPER
#define PARALLAXOIDWALLPAPER

#include <Plasma/Wallpaper>

class QPropertyAnimation;


class ParallaxoidWallpaper : public Plasma::Wallpaper {
    Q_OBJECT
    Q_PROPERTY(QPointF position READ position() WRITE setPosition())

public:
    ParallaxoidWallpaper(QObject* parent, const QVariantList& args);

    void paint(QPainter* painter, const QRectF& exposedRect);

private:
    void render();
    QSize desktopToCoords(int desktop);
    QPointF position();
    void setPosition(const QPointF& position);

protected:
    virtual void init(const KConfigGroup& config);

private slots:
    void slotMovedToDesktop(int desktop);
    void slotActivityChanged(const QString& activityId);
    void slotTweenFinished();
    void slotTweenUpdated(const QVariant& value);

private:
    QSize m_desktopGrid;
    QSize m_currentCoords;
    QSize m_newCoords;

    int m_desktop;
    QString m_activity;

    bool m_doNothing;
    bool m_doTween;

    QPainter m_painter;
    QImage m_background;
    QPixmap m_buffer;
    QPointF m_position;
    QPoint center;
    QPoint offset;
};

K_EXPORT_PLASMA_WALLPAPER(parallaxoidwallpaper, ParallaxoidWallpaper)

#endif
