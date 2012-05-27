#ifndef PARALLAXOIDWALLPAPER
#define PARALLAXOIDWALLPAPER

#include <Plasma/Wallpaper>

#include "ui_config.h"

class QPropertyAnimation;


class ParallaxoidWallpaper : public Plasma::Wallpaper {
    Q_OBJECT
    Q_PROPERTY(QPointF position READ position() WRITE setPosition())

public:
    ParallaxoidWallpaper(QObject* parent, const QVariantList& args);

    void paint(QPainter* painter, const QRectF& exposedRect);
    virtual void save(KConfigGroup& config);
    virtual QWidget* createConfigurationInterface(QWidget* parent);

private:
    void render();
    QSize desktopToCoords(int desktop);
    QPointF position();
    void setPosition(const QPointF& position);

protected:
    virtual void init(const KConfigGroup& config);

Q_SIGNALS:
    void settingsChanged(bool modified);

private slots:
    void slotChooseBackground(QWidget *);
    void slotMovedToDesktop(int desktop);
    void slotActivityChanged(const QString& activityId);
    void slotTweenFinished();
    void slotTweenUpdated(const QVariant& value);

protected slots:
    void slotSettingsModified();

private:
    Ui::Config configWidget;

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
