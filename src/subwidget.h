#pragma once

#include <QWidget>
#include <QPixmap>
#include <QScrollArea>
#include <QPushButton>

#include "clicklabel.h"

namespace Ui
{
    class subWidget;
}

class subWidget : public QWidget
{
    Q_OBJECT

    private:
        Ui::subWidget* ui;

        QPixmap* m_Background;
        QPixmap* m_Pic;
        QLabel* m_Icon;
        QLabel* m_Title;
        QScrollArea* m_ContentArea;
        QLabel* m_Content;

        QPushButton* m_LeftBtn;
        QPushButton* m_MidBtn;
        QPushButton* m_RightBtn;

    public:
        explicit subWidget(QWidget* parent = nullptr);
        ~subWidget();

        void setIcon(const QString& path);
        void setBackground(const QString& path);
        void setTitle(const QString& title);
        void setContent(const QString& content);
        void setTextOfBottom(const QString& left,const QString& mid,const QString& right);
        void setFuncOfBtn(std::function<void(void)> left,std::function<void(void)> right);
        void setFuncOfBtn(std::function<void(void)> left,std::function<void(void)> mid,std::function<void(void)> right);

        void showContent();
        void showInput();
        QString getInput();

    protected:
        bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        ;

    signals:
        void closed();

    private:
        void initalWidget();
};