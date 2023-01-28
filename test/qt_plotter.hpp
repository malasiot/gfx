#pragma once
#include <memory>
#include <thread>

#include <QMainWindow>
#include <QPushButton>

class QPlotsControl : public QMainWindow
{
        Q_OBJECT

public:
        QPlotsControl(QWidget *parent = nullptr) : QMainWindow(parent)
        {m_Btn = new QPushButton(this);}
private:
        QPushButton* m_Btn;
};

class QtPlotter
{
public:
        QtPlotter();
        ~QtPlotter();
        void Close();
private:
        void ApplicationLoop();
        std::unique_ptr<std::thread> m_guiThread;
};


void Start();
void Stop();
