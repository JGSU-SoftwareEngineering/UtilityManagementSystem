#ifndef REPAIR_MANAGEMENT_H
#define REPAIR_MANAGEMENT_H

#include <QWidget>
#include <QTableWidget>

namespace Ui
{
    class paymentManagement;
}

class paymentManagement : public QWidget
{
    Q_OBJECT

    private:
        Ui::paymentManagement* ui;
        QString m_Id;

    public:
        explicit paymentManagement(QWidget* parent = nullptr);
        ~paymentManagement();
        void setCurrentIndex(int i);

    protected:
        bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        void setId(const QString& str);
        void refreshRaiseRecord();
        void refreshBillRecord();
        void refreshPaymentRecord();

    signals:

    private:
        void initalWidget();
        void initalTimer();
};

#endif /* REPAIR_MANAGEMENT_H */
