#ifndef STATE_MANAGEMENT_H
#define STATE_MANAGEMENT_H

#include <QWidget>

namespace Ui
{
    class stateManagement;
}

class stateManagement : public QWidget
{
    Q_OBJECT

    private:
        Ui::stateManagement* ui;
        QString m_Id;

    public:
        explicit stateManagement(QWidget* parent = nullptr);
        ~stateManagement();
        void setCurrentIndex(int i);

    protected:
        bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        void setId(const QString& str);
        void updateCombox();
        void updateRecord();
        void updateChart();

    signals:

    private:
        void initalWidget();
        
};

#endif /* STATE_MANAGEMENT_H */