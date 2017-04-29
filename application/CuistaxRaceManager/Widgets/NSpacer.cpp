#include "NSpacer.hpp"

NSpacer::NSpacer(QWidget* parent) : QWidget(parent)
{
    // GUI Configuration
    this->resize(QSize(0, 0));
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

NSpacer::~NSpacer(void)
{
    // Nothing to do here ...
}

void NSpacer::setHorizontalSpacing(int width)
{
    QSize size = this->size();
    size.setWidth(width);
    this->resize(size);
}

void NSpacer::setVerticalSpacing(int height)
{
    QSize size = this->size();
    size.setHeight(height);
    this->resize(size);
}
