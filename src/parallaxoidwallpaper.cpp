#include <QPainter>
#include <QPropertyAnimation>

#include <QX11Info>
#include <NETRootInfo>
#include <KWindowSystem>
#include <KActivities/Consumer>

#include "parallaxoidwallpaper.h"

ParallaxoidWallpaper::ParallaxoidWallpaper(QObject* parent, const QVariantList& args)
    : Plasma::Wallpaper(parent, args),
      m_doTween(true),
      m_doNothing(false)
{

}

void ParallaxoidWallpaper::paint(QPainter* painter, const QRectF& exposedRect)
{
    if (m_buffer.size() != boundingRect().size()) {
        render();
    }

    painter->drawPixmap(exposedRect, m_buffer, exposedRect);
}

QPointF ParallaxoidWallpaper::position()
{
    return m_position;
}

void ParallaxoidWallpaper::setPosition(const QPointF& position)
{
    m_position = position;
}

void ParallaxoidWallpaper::init(const KConfigGroup& config)
{
    Q_UNUSED(config);

    m_background.load("/home/makism/Development/plasma/wallpaper.jpg");

    m_activity = KActivities::Consumer::currentActivity();
    m_desktop = KWindowSystem::currentDesktop();

    const unsigned long properties[2] = { 0, NET::WM2DesktopLayout };
    NETRootInfo info(QX11Info::display(), properties, 2);
    m_desktopGrid = info.desktopLayoutColumnsRows();

    QRect res = KWindowSystem::workArea(0);

    center = QPoint(m_background.width(), m_background.height());
    center.rx() -= res.width();
    center.ry() -= res.height();
    center.rx() /= 2.0;
    center.ry() /= 2.0;
    center *= -1.0;

    offset = center;
    offset /= 3;

    m_position = center;
    m_position.setX(offset.x() * -(m_desktop - 1));

    connect(KWindowSystem::self(), SIGNAL(currentDesktopChanged(int)),
            this, SLOT(slotMovedToDesktop(int)));

    render();
    emit update(boundingRect());
}

void ParallaxoidWallpaper::render()
{
    if (m_buffer.size() != boundingRect().size()) {
        m_buffer = QPixmap(boundingRect().size().toSize());
    }

    m_painter.begin(&m_buffer);
    m_painter.drawImage(m_position, m_background);
    m_painter.end();
}

QSize ParallaxoidWallpaper::desktopToCoords(int desktop)
{
    int offset = 0;
    QSize targetGrid(0, 0);
    bool hasLocated = false;

    for (int h=0; h<m_desktopGrid.height(); ++h) {
        for (int w=0; w<m_desktopGrid.width(); ++w) {
            ++offset;

            if (offset == desktop) {
                hasLocated = true;
                targetGrid.setWidth(w);
                targetGrid.setHeight(h);

                break;
            }
        }

        if (hasLocated)
            break;
    }

    return targetGrid;
}

void ParallaxoidWallpaper::slotMovedToDesktop(int newDesktop)
{
    if (m_doNothing)
        return;

    QSize currentGrid = desktopToCoords(m_desktop);

    m_desktop = newDesktop;
    QSize targetGrid = desktopToCoords(newDesktop);
    QSize diffGrid = currentGrid - targetGrid;

    if (!m_doTween)
        return;

    m_doTween = false;
    QPointF newPosition(m_position.x() + -diffGrid.width() * offset.x() * 3.0f,
                        m_position.y() + -diffGrid.height() * offset.y() * 3.0f);

    QPropertyAnimation *tween = new QPropertyAnimation(this, "position");
    connect(tween, SIGNAL(finished()),
            this, SLOT(slotTweenFinished()));
    connect(tween, SIGNAL(valueChanged(QVariant)),
            this, SLOT(slotTweenUpdated(QVariant)));

    tween->setDuration(1000);
    tween->setStartValue(m_position);
    tween->setEndValue(newPosition);
    tween->setEasingCurve(QEasingCurve::OutCubic);
    tween->start();
}

void ParallaxoidWallpaper::slotActivityChanged(const QString& activityId)
{
    if (m_activity == activityId)
        m_doNothing = false;
    else
        m_doNothing = true;
}

void ParallaxoidWallpaper::slotTweenFinished()
{
    m_doTween = true;
}

void ParallaxoidWallpaper::slotTweenUpdated(const QVariant& value)
{
    Q_UNUSED(value);

    render();
    emit update(boundingRect());
}

#include "parallaxoidwallpaper.moc"
