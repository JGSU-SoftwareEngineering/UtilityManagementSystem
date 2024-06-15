#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class loginBox; }
QT_END_NAMESPACE

enum class loginType
{
    Tenant,
    Admin
};

class loginBox : public QWidget
{
    Q_OBJECT

    private:
        Ui::loginBox* ui;
        bool m_Logged;
        bool m_Connected;

    public:
        explicit loginBox(QWidget* parent = nullptr);
        ~loginBox();
        bool isLogged();

    protected:
        bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        void reset();
        void updateConnectState();

    signals:
        void closed();
        void logged(const loginType& type,const QString& id);
        void logout();

    private:
        void initalWidget();
        void initalConnectState();
};