#ifndef TENANT_MANAGEMENT_H
#define TENANT_MANAGEMENT_H

#include <QWidget>

#include "global.h"

namespace Ui
{
    class tenantManagement;
}

class tenantManagement : public QWidget
{
    Q_OBJECT

    private:
        Ui::tenantManagement* ui;

    public:
        explicit tenantManagement(QWidget* parent = nullptr);
        ~tenantManagement();
        void setCurrentIndex(int i);

    protected:
        bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        void setEdit(bool isEdit);

    signals:

    private:
        void initalWidget();
};

#endif /* TENANT_MANAGEMENT_H */
