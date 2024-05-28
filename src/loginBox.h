#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class loginBox; }
QT_END_NAMESPACE

enum class loginType
{
    Student,
    Admin
};

class loginBox : public QWidget
{
    Q_OBJECT

    private:
        Ui::loginBox* ui;
        bool m_Logged;

    public:
        explicit loginBox(QWidget* parent = nullptr);
        ~loginBox();
        bool isLogged();

    protected:
        bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        void reset();

    signals:
        void closed();
        void logged(const loginType& type,const QString& id);

    private:
        void initalWidget();
};