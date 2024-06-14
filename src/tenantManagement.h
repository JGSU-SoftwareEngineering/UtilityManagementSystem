#ifndef TENANT_MANAGEMENT_H
#define TENANT_MANAGEMENT_H

#include <QWidget>

namespace Ui
{
    class tenantManagement;
}

class excelReader;

class tenantManagement : public QWidget
{
    Q_OBJECT

    private:
        Ui::tenantManagement* ui;
        bool isTenant;
        excelReader* reader;

    public:
        explicit tenantManagement(QWidget* parent = nullptr);
        ~tenantManagement();
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

#endif /* TENANT_MANAGEMENT_H */
