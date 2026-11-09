#pragma once
#include <QMainWindow>
#include <QString>
#include "Simulator.h"
#include "program.h"

class QPlainTextEdit;
class QTableWidget;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void onLoad();
    void onLoadFile();
    void onStep();
    void onRunAll();
    void onReset();

private:
    void refresh();
    void showMessage(const QString& text, bool isError = false);

    Simulator sim;
    program prog;

    QPlainTextEdit* input;
    QTableWidget* memTable;
    QLabel* regCards[16];
    int prevRegs[16];
    int lastExec = -1;

    QLabel* pcValue;
    QLabel* irValue;
    QLabel* decoded;
    QLabel* stateChip;
    QLabel* status;
    QLabel* screen;
};