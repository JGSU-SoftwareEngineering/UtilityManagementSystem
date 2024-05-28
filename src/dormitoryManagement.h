#ifndef DORMITORY_MANAGEMENT_H
#define DORMITORY_MANAGEMENT_H

#include <QWidget>

namespace Ui
{
    class dormitoryManagement;
}

class dormitoryManagement : public QWidget
{
    Q_OBJECT

    private:
        Ui::dormitoryManagement* ui;
        bool isStudent;

    public:
        explicit dormitoryManagement(QWidget* parent = nullptr);
        ~dormitoryManagement();
        void setCurrentIndex(int i);

    protected:
        bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        void setId(const QString& str);
        void updateAllocation();

    signals:

    private:
        void initalWidget();
};

#endif /* DORMITORY_MANAGEMENT_H */
