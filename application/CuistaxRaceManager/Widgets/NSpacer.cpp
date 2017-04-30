#include "NSpacer.hpp"

NSpacer::NSpacer(int width, int height, QWidget* parent) :
    NSpacer(QSize(width, height), parent)
{
    // Nothing to do here ...
}

NSpacer::NSpacer(QSize size, QWidget* parent) : QWidget(parent)
{
    // GUI Configuration
    this->resize(size);
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
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
