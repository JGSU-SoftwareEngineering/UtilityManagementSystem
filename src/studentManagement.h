#ifndef STUDENT_MANAGEMENT_H
#define STUDENT_MANAGEMENT_H

#include <QWidget>

namespace Ui
{
    class studentManagement;
}

class excelReader;

class studentManagement : public QWidget
{
    Q_OBJECT

    private:
        Ui::studentManagement* ui;
        bool isStudent;
        excelReader* reader;

    public:
        explicit studentManagement(QWidget* parent = nullptr);
        ~studentManagement();
        void setCurrentIndex(int i);

    protected:
        bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        void setEdit(bool isEdit);
        void setId(const QString& str);

    signals:

    private:
        void initalWidget();
};

#endif /* STUDENT_MANAGEMENT_H */
