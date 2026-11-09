#include "MainWindow.h"
#include <QPlainTextEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFileDialog>
#include <QGraphicsDropShadowEffect>
#include <QStringList>
#include <QColor>
#include <QFont>
#include <vector>
#include <cmath>

namespace {

const char* kStyle = R"(
QWidget#root {
    background: qlineargradient(x1:0, y1:0, x2:0.35, y2:1,
                stop:0 #171650, stop:0.55 #0D0E2C, stop:1 #090A1F);
}
QWidget {
    color: #E8E9FF;
    font-family: 'Segoe UI Variable', 'Segoe UI', 'Helvetica Neue', sans-serif;
    font-size: 13px;
}
QLabel#title {
    font-family: 'Bahnschrift', 'Segoe UI Semibold', 'Segoe UI';
    font-size: 30px;
    font-weight: 600;
    color: #FFFFFF;
}
QLabel#subtitle { color: #9A9DD6; font-size: 13px; }
QLabel#caption  { color: #8A8DC4; font-size: 12px; }
QLabel#section  {
    font-family: 'Bahnschrift', 'Segoe UI Semibold', 'Segoe UI';
    font-size: 16px;
    color: #B9A8FF;
}
QLabel#readoutPc {
    font-family: 'Bahnschrift', 'Segoe UI';
    font-size: 38px;
    color: #FFC857;
}
QLabel#readoutIr {
    font-family: 'Bahnschrift', 'Segoe UI';
    font-size: 38px;
    color: #38E1FF;
}
QLabel#decoded {
    font-family: 'Bahnschrift', 'Segoe UI';
    font-size: 24px;
    color: #FFFFFF;
}
QFrame#rule {
    border: none;
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #7B5CFF, stop:0.5 #38E1FF, stop:1 rgba(56,225,255,0));
}
QFrame#field {
    background: rgba(7, 8, 30, 150);
    border: 1px solid #2B2E78;
    border-radius: 20px;
}
QPlainTextEdit {
    background: rgba(7, 8, 30, 170);
    border: 1px solid #2B2E78;
    border-radius: 10px;
    padding: 10px;
    color: #F1F2FF;
    font-family: 'Cascadia Mono', 'Consolas', monospace;
    font-size: 14px;
    selection-background-color: #5B3FE0;
}
QPlainTextEdit:focus { border: 1px solid #38E1FF; }
QLabel#screen {
    background: rgba(56, 225, 255, 18);
    border: 1px solid #1F5E76;
    border-radius: 10px;
    padding: 10px;
    color: #A9F4FF;
    font-family: 'Cascadia Mono', 'Consolas', monospace;
    font-size: 15px;
}
QLabel#status { color: #9A9DD6; font-size: 12px; }
QTableWidget {
    background: transparent;
    border: none;
    gridline-color: #0B0C2A;
    font-family: 'Cascadia Mono', 'Consolas', monospace;
    font-size: 12px;
}
QHeaderView { background: transparent; }
QHeaderView::section {
    background: transparent;
    color: #6C70B5;
    border: none;
    font-size: 11px;
}
QTableCornerButton::section { background: transparent; border: none; }
QPushButton {
    background: rgba(123, 92, 255, 28);
    border: 1px solid #4A4CA8;
    border-radius: 8px;
    padding: 10px 16px;
    font-weight: 600;
    color: #E8E9FF;
}
QPushButton:hover { background: rgba(123, 92, 255, 80); border-color: #7B5CFF; }
QPushButton:pressed { background: rgba(123, 92, 255, 140); }
QPushButton#btnStep {
    background: rgba(56, 225, 255, 20);
    border-color: #2AA9C4;
    color: #8CEBFF;
}
QPushButton#btnStep:hover { background: rgba(56, 225, 255, 70); border-color: #38E1FF; }
QPushButton#btnRun {
    border: none;
    color: #0A0B22;
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #7B5CFF, stop:1 #38E1FF);
}
QPushButton#btnRun:hover {
    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #957BFF, stop:1 #7DEBFF);
}
QPushButton#btnReset {
    background: transparent;
    border: none;
    color: #FF6FA5;
    padding: 6px;
}
QPushButton#btnReset:hover { color: #FFFFFF; background: rgba(255, 77, 141, 60); }
)";

QString hx(int value, int width)
{
    return QString::number(value, 16).toUpper().rightJustified(width, '0');
}

QString regName(int index)
{
    return "R" + hx(index, 1);
}

QColor mix(const QColor& a, const QColor& b, double t)
{
    if (t < 0) t = 0;
    if (t > 1) t = 1;
    return QColor(int(a.red() + (b.red() - a.red()) * t),
                  int(a.green() + (b.green() - a.green()) * t),
                  int(a.blue() + (b.blue() - a.blue()) * t));
}

QColor heat(int value)
{
    const double t = std::pow(value / 255.0, 0.6);
    const QColor low(0x10, 0x12, 0x3A);
    const QColor mid(0x5B, 0x3F, 0xE0);
    const QColor high(0x38, 0xE1, 0xFF);
    if (t < 0.5)
        return mix(low, mid, t * 2.0);
    return mix(mid, high, (t - 0.5) * 2.0);
}

QString describe(uint16_t ir)
{
    const int op = (ir >> 12) & 0xF;
    const int x = (ir >> 8) & 0xF;
    const int y = (ir >> 4) & 0xF;
    const int z = ir & 0xF;
    const int xy = (y << 4) | z;

    switch (op) {
    case 0x1: return QString("Load %1 from memory 0x%2").arg(regName(x), hx(xy, 2));
    case 0x2: return QString("Put 0x%1 into %2").arg(hx(xy, 2), regName(x));
    case 0x3: return QString("Store %1 at memory 0x%2").arg(regName(x), hx(xy, 2));
    case 0x4: return QString("Copy %1 into %2").arg(regName(y), regName(z));
    case 0x5: return QString("Add %1 and %2 into %3").arg(regName(y), regName(z), regName(x));
    case 0x6: return QString("Add %1 and %2 as floating point into %3").arg(regName(y), regName(z), regName(x));
    case 0x7: return QString("OR %1 with %2 into %3").arg(regName(y), regName(z), regName(x));
    case 0x8: return QString("AND %1 with %2 into %3").arg(regName(y), regName(z), regName(x));
    case 0x9: return QString("XOR %1 with %2 into %3").arg(regName(y), regName(z), regName(x));
    case 0xA: return QString("Rotate %1 right by %2").arg(regName(x)).arg(z);
    case 0xB: return QString("Jump to 0x%1 if %2 equals R0").arg(hx(xy, 2), regName(x));
    case 0xC: return QString("Halt");
    default:  return QString("Unknown instruction");
    }
}

void paintNode(QLabel* node, int index, int value, bool changed)
{
    const double t = std::pow(value / 255.0, 0.6);
    const QColor center = mix(QColor(0x1E, 0x20, 0x66), QColor(0x38, 0xE1, 0xFF), t);
    const QColor edge = mix(QColor(0x12, 0x13, 0x3D), QColor(0x7B, 0x5C, 0xFF), t);
    const bool bright = t > 0.55;

    const QString ring = changed ? "#FF4D8D" : (value ? "#7B5CFF" : "#2B2E78");
    const QString fg = bright ? "#0A0B22" : "#E8E9FF";
    const QString sub = bright ? "#2A2F6A" : "#8A8DC4";

    node->setText(QString("<div style='font-size:10px;color:%1;'>%2</div>"
                          "<div style='font-size:19px;font-weight:600;'>%3</div>")
                      .arg(sub, regName(index), hx(value, 2)));

    node->setStyleSheet(QString("QLabel{background:qradialgradient(cx:0.5,cy:0.4,radius:0.75,"
                                "fx:0.5,fy:0.35,stop:0 %1,stop:1 %2);"
                                "border:2px solid %3;border-radius:30px;color:%4;"
                                "font-family:'Bahnschrift','Segoe UI';}")
                            .arg(center.name(), edge.name(), ring, fg));

    if (changed) {
        auto* fx = new QGraphicsDropShadowEffect(node);
        fx->setBlurRadius(28);
        fx->setOffset(0, 0);
        fx->setColor(QColor(0xFF, 0x4D, 0x8D));
        node->setGraphicsEffect(fx);
    } else if (node->graphicsEffect()) {
        node->setGraphicsEffect(nullptr);
    }
}

void paintChip(QLabel* chip, const QString& text, const QString& color)
{
    chip->setText(text);
    chip->setStyleSheet(QString("QLabel{color:%1;border:1px solid %1;border-radius:13px;"
                                "padding:5px 16px;font-weight:600;background:transparent;}")
                            .arg(color));
}

void addGlow(QLabel* label, const QColor& color)
{
    auto* fx = new QGraphicsDropShadowEffect(label);
    fx->setBlurRadius(26);
    fx->setOffset(0, 0);
    fx->setColor(color);
    label->setGraphicsEffect(fx);
}

} // namespace

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle("Vole machine");
    resize(1340, 800);
    setMinimumSize(1140, 700);

    for (int i = 0; i < 16; i++)
        prevRegs[i] = 0;

    auto* root = new QWidget;
    root->setObjectName("root");
    root->setStyleSheet(kStyle);
    setCentralWidget(root);

    auto* page = new QVBoxLayout(root);
    page->setContentsMargins(28, 22, 28, 16);
    page->setSpacing(16);

    auto sectionTitle = [](const QString& text) {
        auto* label = new QLabel(text);
        label->setObjectName("section");
        return label;
    };

    auto* head = new QHBoxLayout;
    auto* titles = new QVBoxLayout;
    titles->setSpacing(2);
    auto* title = new QLabel("Vole machine");
    title->setObjectName("title");
    auto* subtitle = new QLabel("Watch a tiny 8-bit computer think, one instruction at a time.");
    subtitle->setObjectName("subtitle");
    titles->addWidget(title);
    titles->addWidget(subtitle);
    stateChip = new QLabel;
    head->addLayout(titles, 1);
    head->addWidget(stateChip, 0, Qt::AlignTop | Qt::AlignRight);

    pcValue = new QLabel("00");
    pcValue->setObjectName("readoutPc");
    irValue = new QLabel("0000");
    irValue->setObjectName("readoutIr");
    decoded = new QLabel;
    decoded->setObjectName("decoded");
    decoded->setWordWrap(true);
    addGlow(pcValue, QColor(255, 200, 87, 170));
    addGlow(irValue, QColor(56, 225, 255, 150));

    auto makeReadout = [](const QString& caption, QWidget* value) {
        auto* column = new QVBoxLayout;
        column->setSpacing(0);
        auto* cap = new QLabel(caption);
        cap->setObjectName("caption");
        column->addWidget(cap);
        column->addWidget(value);
        return column;
    };

    auto* ribbon = new QHBoxLayout;
    ribbon->setSpacing(40);
    ribbon->addLayout(makeReadout("Next instruction at", pcValue));
    ribbon->addLayout(makeReadout("Last fetched", irValue));
    ribbon->addLayout(makeReadout("What it does", decoded), 1);

    auto* rule = new QFrame;
    rule->setObjectName("rule");
    rule->setFixedHeight(2);

    auto* left = new QVBoxLayout;
    left->setSpacing(10);
    left->addWidget(sectionTitle("Program"));

    input = new QPlainTextEdit;
    input->setPlaceholderText("2105\n2203\n5312\nC000");
    left->addWidget(input, 3);

    auto* hint = new QLabel("Four hex digits per line. Anything after ; is a comment.");
    hint->setObjectName("caption");
    left->addWidget(hint);

    auto makeButton = [](const QString& text, const QString& name) {
        auto* button = new QPushButton(text);
        button->setObjectName(name);
        button->setCursor(Qt::PointingHandCursor);
        return button;
    };
    auto* loadBtn = makeButton("Load", "btnLoad");
    auto* loadFileBtn = makeButton("Open file...", "btnFile");
    auto* stepBtn = makeButton("Step", "btnStep");
    auto* runBtn = makeButton("Run", "btnRun");
    auto* resetBtn = makeButton("Reset", "btnReset");

    auto* buttons = new QGridLayout;
    buttons->setSpacing(8);
    buttons->addWidget(loadBtn, 0, 0);
    buttons->addWidget(loadFileBtn, 0, 1);
    buttons->addWidget(stepBtn, 1, 0);
    buttons->addWidget(runBtn, 1, 1);
    buttons->addWidget(resetBtn, 2, 0, 1, 2);
    left->addLayout(buttons);

    connect(loadBtn, &QPushButton::clicked, this, &MainWindow::onLoad);
    connect(loadFileBtn, &QPushButton::clicked, this, &MainWindow::onLoadFile);
    connect(stepBtn, &QPushButton::clicked, this, &MainWindow::onStep);
    connect(runBtn, &QPushButton::clicked, this, &MainWindow::onRunAll);
    connect(resetBtn, &QPushButton::clicked, this, &MainWindow::onReset);

    left->addSpacing(6);
    left->addWidget(sectionTitle("Output"));
    screen = new QLabel;
    screen->setObjectName("screen");
    screen->setWordWrap(true);
    screen->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    screen->setMinimumHeight(90);
    left->addWidget(screen, 1);

    auto* fieldFrame = new QFrame;
    fieldFrame->setObjectName("field");
    auto* fieldLayout = new QVBoxLayout(fieldFrame);
    fieldLayout->setContentsMargins(18, 16, 18, 14);
    fieldLayout->setSpacing(8);
    fieldLayout->addWidget(sectionTitle("Memory"));

    memTable = new QTableWidget(16, 16);
    memTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    memTable->setSelectionMode(QAbstractItemView::NoSelection);
    memTable->setFocusPolicy(Qt::NoFocus);
    memTable->setShowGrid(true);
    memTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    memTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QStringList labels;
    for (int i = 0; i < 16; i++)
        labels << QString::number(i, 16).toUpper();
    memTable->setHorizontalHeaderLabels(labels);
    memTable->setVerticalHeaderLabels(labels);
    memTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    memTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    memTable->horizontalHeader()->setHighlightSections(false);
    memTable->verticalHeader()->setHighlightSections(false);
    memTable->setMinimumSize(520, 430);
    fieldLayout->addWidget(memTable, 1);

    auto* legend = new QLabel(
        "<span style='color:#FFC857;'>&#9632;</span> runs next &nbsp;&nbsp; "
        "<span style='color:#FF4D8D;'>&#9632;</span> just ran &nbsp;&nbsp; "
        "<span style='color:#38E1FF;'>&#9632;</span> bigger values glow brighter");
    legend->setObjectName("caption");
    legend->setTextFormat(Qt::RichText);
    fieldLayout->addWidget(legend);

    auto* right = new QVBoxLayout;
    right->setSpacing(10);
    right->addWidget(sectionTitle("Registers"));

    auto* honeycomb = new QGridLayout;
    honeycomb->setHorizontalSpacing(0);
    honeycomb->setVerticalSpacing(6);
    for (int c = 0; c < 9; c++)
        honeycomb->setColumnMinimumWidth(c, 32);
    for (int i = 0; i < 16; i++) {
        auto* node = new QLabel;
        node->setFixedSize(60, 60);
        node->setAlignment(Qt::AlignCenter);
        node->setTextFormat(Qt::RichText);
        regCards[i] = node;
        const int row = i / 4;
        const int col = (i % 4) * 2 + (row % 2);
        honeycomb->addWidget(node, row, col, 1, 2, Qt::AlignCenter);
    }
    right->addLayout(honeycomb);

    auto* nodeHint = new QLabel("A pink ring marks a register that changed in the last action.");
    nodeHint->setObjectName("caption");
    nodeHint->setWordWrap(true);
    right->addWidget(nodeHint);
    right->addStretch(1);

    auto* body = new QHBoxLayout;
    body->setSpacing(26);
    body->addLayout(left, 3);
    body->addWidget(fieldFrame, 6);
    body->addLayout(right, 3);

    status = new QLabel;
    status->setObjectName("status");

    page->addLayout(head);
    page->addLayout(ribbon);
    page->addWidget(rule);
    page->addLayout(body, 1);
    page->addWidget(status);

    refresh();
    showMessage("Ready. Type a program or open a file.");
}

void MainWindow::showMessage(const QString& text, bool isError)
{
    status->setText(text);
    status->setStyleSheet(isError ? "QLabel{color:#FF6FA5;}" : "QLabel{color:#9A9DD6;}");
}

void MainWindow::onLoad()
{
    std::vector<uint16_t> instructions;
    const QStringList lines = input->toPlainText().split('\n');

    for (QString line : lines) {
        int sc = line.indexOf(';');
        if (sc != -1)
            line = line.left(sc);
        line = line.trimmed();
        if (line.isEmpty())
            continue;

        bool ok = false;
        uint value = line.toUInt(&ok, 16);
        if (line.length() != 4 || !ok) {
            showMessage("\"" + line + "\" is not a valid instruction. Use exactly 4 hex digits.", true);
            return;
        }
        instructions.push_back(static_cast<uint16_t>(value));
    }

    if (instructions.empty() || instructions.back() != 0xC000)
        instructions.push_back(0xC000);

    sim.reset();
    sim.loadprogram(instructions);
    lastExec = -1;
    refresh();
    showMessage("Loaded " + QString::number(instructions.size()) + " instructions.");
}

void MainWindow::onLoadFile()
{
    QString path = QFileDialog::getOpenFileName(
        this, "Open program file", QString(), "Text files (*.txt);;All files (*)");
    if (path.isEmpty())
        return;

    prog.clear();
    if (!prog.loadFromFile(path.toLocal8Bit().toStdString())) {
        prog.clear();
        showMessage("Could not read \"" + path + "\". Each line must be exactly 4 hex digits.", true);
        return;
    }

    QStringList lines;
    for (size_t i = 0; i < prog.getSize(); i++)
        lines << QString("%1").arg(prog.getInstruction(i), 4, 16, QChar('0')).toUpper();

    input->setPlainText(lines.join('\n'));
    onLoad();
}

void MainWindow::onStep()
{
    if (!sim.is_programmed()) {
        showMessage("Load a program first.", true);
        return;
    }
    if (sim.isHalted() || sim.hasError()) {
        showMessage("The program has stopped. Reset or load it again to start over.", true);
        return;
    }
    lastExec = sim.getPC();
    sim.runstep();
    refresh();
}

void MainWindow::onRunAll()
{
    if (!sim.is_programmed()) {
        showMessage("Load a program first.", true);
        return;
    }
    lastExec = -1;
    bool finished = sim.runAll();
    refresh();
    if (!finished)
        showMessage("Stopped after 100,000 steps. The program may be stuck in a loop.", true);
}

void MainWindow::onReset()
{
    sim.reset();
    prog.clear();
    lastExec = -1;
    refresh();
    showMessage("Simulator reset.");
}

void MainWindow::refresh()
{
    const bool loaded = sim.is_programmed();
    const int pc = sim.getPC();
    const int ir = sim.getIR();

    for (int i = 0; i < 16; i++) {
        const int value = sim.getRegister(i);
        paintNode(regCards[i], i, value, value != prevRegs[i]);
        prevRegs[i] = value;
    }

    for (int r = 0; r < 16; r++) {
        for (int c = 0; c < 16; c++) {
            const int addr = r * 16 + c;
            const int value = sim.readMemory(addr);

            QColor bg = heat(value);
            QColor fg = value == 0 ? QColor(0x3B, 0x3E, 0x8A)
                                   : (value > 150 ? QColor(0x0A, 0x0B, 0x22) : QColor(0xF1, 0xF2, 0xFF));
            bool bold = false;

            const bool isNext = loaded && (addr == pc || addr == pc + 1);
            const bool isLast = loaded && lastExec >= 0 && (addr == lastExec || addr == lastExec + 1);
            if (isNext) {
                bg = QColor(0xFF, 0xC8, 0x57);
                fg = QColor(0x0A, 0x0B, 0x22);
                bold = true;
            } else if (isLast) {
                bg = QColor(0xFF, 0x4D, 0x8D);
                fg = QColor(0xFF, 0xFF, 0xFF);
                bold = true;
            }

            auto* item = new QTableWidgetItem(hx(value, 2));
            item->setTextAlignment(Qt::AlignCenter);
            item->setBackground(bg);
            item->setForeground(fg);
            item->setToolTip("0x" + hx(addr, 2) + " = " + QString::number(value));
            if (bold) {
                QFont f = item->font();
                f.setBold(true);
                item->setFont(f);
            }
            memTable->setItem(r, c, item);
        }
    }

    pcValue->setText(hx(pc, 2));
    irValue->setText(hx(ir, 4));

    if (!loaded)
        decoded->setText("No program loaded");
    else if (ir == 0 && pc == 0)
        decoded->setText("Press Step or Run to begin");
    else
        decoded->setText(describe(static_cast<uint16_t>(ir)));

    if (!loaded)
        paintChip(stateChip, "Idle", "#8A8DC4");
    else if (sim.hasError())
        paintChip(stateChip, "Error", "#FF4D8D");
    else if (sim.isHalted())
        paintChip(stateChip, "Halted", "#6BFFB8");
    else if (ir == 0 && pc == 0)
        paintChip(stateChip, "Ready", "#38E1FF");
    else
        paintChip(stateChip, "Running", "#B9A8FF");

    const std::string out = sim.getScreen();
    screen->setText(out.empty() ? QString("Nothing written to memory 0xFF yet.")
                                : QString::fromStdString(out));

    if (sim.hasError())
        showMessage(QString::fromStdString(sim.getError()), true);
}