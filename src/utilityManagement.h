#ifndef UTIlIY_MANAGEMENT_H
#define UTIlIY_MANAGEMENT_H

#include <QWidget>

namespace Ui
{
    class utilityManagement;
}

class utilityManagement : public QWidget
{
    Q_OBJECT

    private:
        Ui::utilityManagement* ui;
        bool isStudent;

    public:
        explicit utilityManagement(QWidget* parent = nullptr);
        ~utilityManagement();
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

#endif /* UTIlIY_MANAGEMENT_H */
