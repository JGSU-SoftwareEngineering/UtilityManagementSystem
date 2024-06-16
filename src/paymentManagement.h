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
        void refreshRaiseRecord();
        void refreshBillRecord();
        void refreshPaymentRecord();

    signals:

    private:
        void initalWidget();
        void initalTimer();
        void addToRecordTable(QTableWidget* record,const QList<QVariantList>& info);
};

#endif /* REPAIR_MANAGEMENT_H */
