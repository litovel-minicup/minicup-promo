
#include "trianglecontainer.h"

TrianglesContainer::TrianglesContainer(QQuickItem* parent): QQuickItem(parent)
{
    m_interpolationAnimation = new QVariantAnimation { this };
    m_interpolationAnimation->setDuration(800);
    m_interpolationAnimation->setEasingCurve(QEasingCurve::OutCubic);

    m_pieceManager = new PiecesManager(":/low-poly.svg", ":/low-poly-disasembled.svg");
    this->initTriangles(m_pieceManager->disassembledTriangles(),
                        m_pieceManager->assembledTriangles());

    connect(m_interpolationAnimation, &QVariantAnimation::valueChanged, [this](const QVariant& v) {
        emit this->interpolationRequest(v.toReal());
    });
}

qreal TrianglesContainer::piecesRotation() const
{
    return m_piecesRotation;
}

void TrianglesContainer::interpolate(bool inverted)
{
    if(inverted) {
        m_interpolationAnimation->setStartValue(1.);
        m_interpolationAnimation->setEndValue(0.);
    }

    else {
        m_interpolationAnimation->setStartValue(0.);
        m_interpolationAnimation->setEndValue(1.);
    }
    m_interpolationAnimation->start();
}

void TrianglesContainer::initTriangles(const QVector<TrianglePrimitive>& srcPolygons, const QVector<TrianglePrimitive>& destPolygons)
{
    m_srcPolygons = srcPolygons;
    m_destPolygons = destPolygons;

    for(int i = 0; i < m_srcPolygons.length(); i++) {
        const TrianglePrimitive triangleData = m_srcPolygons.at(i);
        const TrianglePrimitive destTriangleData = m_destPolygons.at(i);

        TriangleItem* item = new TriangleItem { this };
        item->setColor(triangleData.color());
        item->setSrcPoints(triangleData.points());
        item->setDestPoints(destTriangleData.points());
        m_triangles.append(item);

        connect(this, &TrianglesContainer::interpolationRequest,
                item, &TriangleItem::setInterpolation);
    }
}

void TrianglesContainer::setPiecesRotation(qreal piecesRotation)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_piecesRotation, piecesRotation))
        return;

    m_piecesRotation = piecesRotation;
    emit piecesRotationChanged(m_piecesRotation);
}