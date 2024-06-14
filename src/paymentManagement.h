#ifndef REPAIR_MANAGEMENT_H
#define REPAIR_MANAGEMENT_H

#include <QWidget>

namespace Ui
{
    class paymentManagement;
}

class paymentManagement : public QWidget
{
    Q_OBJECT

    private:
        Ui::paymentManagement* ui;
        bool isStudent;
        QString m_Id;

    public:
        explicit paymentManagement(QWidget* parent = nullptr);
        ~paymentManagement();
        void setCurrentIndex(int i);

    protected:
        bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        void setId(const QString& str);
        void refreshHandleRecord();
        void refreshRepairRecord();

    signals:

    private:
        void initalWidget();
};

#endif /* REPAIR_MANAGEMENT_H */
