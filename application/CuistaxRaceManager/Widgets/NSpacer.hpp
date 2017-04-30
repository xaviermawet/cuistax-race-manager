#ifndef __NSPACER_HPP__
#define __NSPACER_HPP__

#include <QWidget>

class NSpacer : public QWidget
{
    Q_OBJECT
    public:

        explicit NSpacer(int width, int height, QWidget* parent = NULL);
        explicit NSpacer(QSize size, QWidget* parent = NULL);
        virtual ~NSpacer(void);

    signals:

    public slots:

        void setHorizontalSpacing(int width);
        void setVerticalSpacing(int height);
};

#endif /* __NSPACER_HPP__ */
