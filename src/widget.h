#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

   // void on_pushButton_5_clicked();

    void on_draw_poly_check_clicked();

    void on_load_button_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
