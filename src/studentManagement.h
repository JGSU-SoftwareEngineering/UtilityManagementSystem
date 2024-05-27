#ifndef STUDENT_MANAGEMENT_H
#define STUDENT_MANAGEMENT_H

#include <QWidget>

namespace Ui
{
    class studentManagement;
}

class studentManagement : public QWidget
{
    Q_OBJECT

    private:
        Ui::studentManagement* ui;

    public:
        explicit studentManagement(QWidget* parent = nullptr);
        ~studentManagement();
        void setCurrentIndex(int i);

    protected:
        bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        ;

    signals:

    private:
        void initalWidget();
        void setEdit(bool isEdit);
};

#endif /* STUDENT_MANAGEMENT_H */
