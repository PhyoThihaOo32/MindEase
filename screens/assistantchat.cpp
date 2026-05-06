#include "screens/assistantchat.h"

#include <QColor>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcessEnvironment>
#include <QTimer>
#include <QUrl>
#include <QtGlobal>

static void applyAssistantGlow(QWidget *widget,
                               int blur = 32,
                               int yOffset = 8,
                               const QColor &color = QColor(126, 170, 124, 28)) {
    auto *shadow = new QGraphicsDropShadowEffect(widget);
    shadow->setBlurRadius(blur);
    shadow->setOffset(0, yOffset);
    shadow->setColor(color);
    widget->setGraphicsEffect(shadow);
}

// ── Stylesheet factory — single source of truth for light vs. dark ────────
namespace {
QString chatStylesheet(bool dark) {
    if (dark) return R"(
        QLabel#assistantIntro {
            color: #7aac6e;
            font-size: 16px; font-weight: 500; line-height: 1.35;
        }
        QFrame#assistantPanel, QFrame#assistantSidePanel {
            border: 1px solid rgba(74,122,67,0.50);
            border-radius: 32px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 rgba(18,36,22,230),
                                        stop:0.52 rgba(22,42,26,220),
                                        stop:1 rgba(16,32,20,210));
        }
        QLabel#assistantPanelTitle, QLabel#assistantSideTitle {
            color: #C8ECC2; font-size: 22px; font-weight: 850;
        }
        QLabel#assistantSmallCopy { color: #5a8a5a; font-size: 11px; line-height: 1.35; }
        QLabel#assistantStatus    { color: #5a7a5a; font-size: 12px; border: none; background: transparent; }
        QFrame#assistantComposer {
            background: rgba(17, 34, 20, 0.98);
            border: none;
        }
        QPushButton#linkPillBtn {
            background: rgba(30,55,30,0.9); color: #C8ECC2;
            border: 1px solid rgba(74,122,67,0.55);
            border-radius: 12px; padding: 7px 10px;
            font-size: 11px; font-weight: 800;
        }
        QPushButton#linkPillBtn:hover {
            background: rgba(40,70,40,0.95); border-color: rgba(100,160,90,0.6); color: #daffd4;
        }
        QLineEdit#assistantInput {
            background: #102516; color: #C8ECC2;
            border: 1px solid rgba(74,122,67,0.55);
            border-radius: 17px; padding: 15px 18px; font-size: 15px;
            selection-background-color: rgba(74,122,67,0.35);
        }
        QLineEdit#assistantInput:focus {
            border: 1px solid rgba(100,160,90,0.7);
            background: rgba(22,45,24,0.94);
        }
    )";
    return R"(
        QLabel#assistantIntro {
            color: #51685b;
            font-size: 16px; font-weight: 500; line-height: 1.35;
        }
        QFrame#assistantPanel, QFrame#assistantSidePanel {
            border: 1px solid rgba(159, 185, 150, 0.64);
            border-radius: 32px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 rgba(249, 244, 232, 216),
                                        stop:0.52 rgba(250, 248, 240, 228),
                                        stop:1 rgba(232, 240, 229, 208));
        }
        QLabel#assistantPanelTitle, QLabel#assistantSideTitle {
            color: #274334; font-size: 22px; font-weight: 850;
        }
        QLabel#assistantSmallCopy { color: #6c8170; font-size: 11px; line-height: 1.35; }
        QLabel#assistantStatus    { color: #738573; font-size: 12px; border: none; background: transparent; }
        QFrame#assistantComposer {
            background: rgba(249, 247, 239, 0.98);
            border: none;
        }
        QPushButton#linkPillBtn {
            background: #f2f7ee; color: #315143;
            border: 1px solid #c7d8c1;
            border-radius: 12px; padding: 7px 10px;
            font-size: 11px; font-weight: 800;
        }
        QPushButton#linkPillBtn:hover {
            background: #e8f1e4; border-color: #a8c09f; color: #274334;
        }
        QLineEdit#assistantInput {
            background: #fffef9; color: #264033;
            border: 1px solid rgba(159, 185, 150, 0.72);
            border-radius: 17px; padding: 15px 18px; font-size: 15px;
            selection-background-color: #cfe3c8;
        }
        QLineEdit#assistantInput:focus {
            border: 1px solid #9fbb94;
            background: rgba(255, 253, 247, 0.94);
        }
    )";
}
} // namespace

AssistantChat::AssistantChat(QWidget *parent)
    : Screen("MindEase Assistant", parent),
      m_network(new QNetworkAccessManager(this)),
      m_backendProcess(new QProcess(this)),
      m_scroll(nullptr),
      m_messagesWidget(nullptr),
      m_messagesLayout(nullptr),
      m_input(nullptr),
      m_sendBtn(nullptr),
      m_statusLbl(nullptr),
      m_waitingForReply(false),
      m_backendStartAttempted(false) {

    setStyleSheet(chatStylesheet(false));

    m_backendProcess->setProcessChannelMode(QProcess::MergedChannels);
    connect(m_backendProcess, &QProcess::readyReadStandardOutput, this, [this]() {
        m_backendLastOutput += QString::fromUtf8(m_backendProcess->readAllStandardOutput());
        constexpr qsizetype maxOutputChars = 2400;
        if (m_backendLastOutput.size() > maxOutputChars) {
            m_backendLastOutput = m_backendLastOutput.right(maxOutputChars);
        }
    });
    connect(m_backendProcess,
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this,
            [this](int, QProcess::ExitStatus) {
                if (!m_waitingForReply) {
                    m_backendStartAttempted = false;
                }
            });

    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(58, 44, 58, 44);
    root->setSpacing(24);

    buildHeader(root);

    QLabel *intro = new QLabel(
        "A calm student-support chat for stress, overwhelm, homesickness, motivation, and help-seeking.");
    intro->setObjectName("assistantIntro");
    intro->setWordWrap(true);
    root->addWidget(intro);

    QFrame *chatPanel = new QFrame();
    chatPanel->setObjectName("assistantPanel");
    applyAssistantGlow(chatPanel, 40, 8, QColor(126, 170, 124, 34));
    QVBoxLayout *chatLay = new QVBoxLayout(chatPanel);
    chatLay->setContentsMargins(36, 32, 36, 30);
    chatLay->setSpacing(18);

    QLabel *panelTitle = new QLabel("Talk through one next step");
    panelTitle->setObjectName("assistantPanelTitle");
    chatLay->addWidget(panelTitle);

    m_statusLbl = new QLabel("");
    m_statusLbl->setObjectName("assistantStatus");
    m_statusLbl->setWordWrap(true);
    m_statusLbl->setVisible(false);
    chatLay->addWidget(m_statusLbl);

    m_scroll = new QScrollArea();
    m_scroll->setWidgetResizable(true);
    m_scroll->setFrameShape(QFrame::NoFrame);
    m_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scroll->setMinimumHeight(360);
    m_scroll->setStyleSheet(
        "QScrollArea { background: transparent; border: none; }"
        "QScrollArea > QWidget > QWidget { background: transparent; border: none; }"
        "QScrollBar:vertical { width: 8px; background: transparent; margin: 2px 0; }"
        "QScrollBar::handle:vertical { background: rgba(122,172,110,0.45); border-radius: 4px; min-height: 42px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: transparent; }");

    m_messagesWidget = new QWidget();
    m_messagesWidget->setStyleSheet("background: transparent; border: none;");
    m_messagesLayout = new QVBoxLayout(m_messagesWidget);
    m_messagesLayout->setContentsMargins(2, 2, 12, 112);
    m_messagesLayout->setSpacing(16);
    m_messagesLayout->addStretch(1);
    m_scroll->setWidget(m_messagesWidget);
    chatLay->addWidget(m_scroll, 1);

    QFrame *composer = new QFrame();
    composer->setObjectName("assistantComposer");
    composer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout *inputLay = new QHBoxLayout(composer);
    inputLay->setContentsMargins(0, 12, 0, 0);
    inputLay->setSpacing(12);

    m_input = new QLineEdit();
    m_input->setObjectName("assistantInput");
    m_input->setPlaceholderText("Say Something..");
    m_input->setMinimumHeight(56);
    inputLay->addWidget(m_input, 1);

    m_sendBtn = new QPushButton("Send");
    m_sendBtn->setObjectName("primaryBtn");
    m_sendBtn->setCursor(Qt::PointingHandCursor);
    m_sendBtn->setMinimumSize(96, 56);
    inputLay->addWidget(m_sendBtn);
    chatLay->addWidget(composer);

    connect(m_sendBtn, &QPushButton::clicked, this, &AssistantChat::sendMessage);
    connect(m_input, &QLineEdit::returnPressed, this, &AssistantChat::sendMessage);

    root->addWidget(chatPanel, 1);

    const QString openingMessage =
        "Hi, I am MindEase. Tell me what feels heavy right now, and I will help you choose one small, realistic next step.";
    addMessage(openingMessage, false);
    appendConversationTurn("assistant", openingMessage);
}

AssistantChat::~AssistantChat() {
    if (m_backendProcess && m_backendProcess->state() != QProcess::NotRunning) {
        m_backendProcess->terminate();
        if (!m_backendProcess->waitForFinished(1200)) {
            m_backendProcess->kill();
            m_backendProcess->waitForFinished(800);
        }
    }
}

void AssistantChat::onActivated() {
    if (m_waitingForReply) return;

    QNetworkRequest request((QUrl(backendHealthUrl())));
    request.setTransferTimeout(1600);

    QNetworkReply *reply = m_network->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        const bool ready =
            reply->error() == QNetworkReply::NoError &&
            statusCode >= 200 &&
            statusCode < 300;
        reply->deleteLater();

        if (ready && m_statusLbl && m_statusLbl->text().contains("backend", Qt::CaseInsensitive)) {
            m_statusLbl->clear();
            m_statusLbl->setVisible(false);
        }
    });
}

QWidget *AssistantChat::makeMessageRow(const QString &text, bool fromUser, bool crisis) const {
    QFrame *row = new QFrame();
    row->setStyleSheet("background: transparent; border: none;");
    row->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    QHBoxLayout *rowLay = new QHBoxLayout(row);
    rowLay->setContentsMargins(0, 0, 0, 0);
    rowLay->setSpacing(8);

    QFrame *bubble = new QFrame();
    bubble->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    bubble->setMaximumWidth(fromUser ? 860 : 980);
    bubble->setMinimumWidth(fromUser ? 300 : 420);
    bubble->setStyleSheet(crisis
        ? "QFrame { background:#fff8f3; border:1px solid #f5b8a8; border-radius:20px; }"
        : fromUser
            ? "QFrame { background:#dff6ff; border:1px solid #8bdff2; border-radius:20px; }"
            : "QFrame { background:qlineargradient(x1:0,y1:0,x2:1,y2:1, stop:0 #f9f5e8, stop:1 #edf4e8); border:1px solid #c7d8c1; border-radius:20px; }");
    if (fromUser) {
        applyAssistantGlow(bubble, 18, 3, QColor(139, 223, 242, 30));
    }

    QVBoxLayout *bubbleLay = new QVBoxLayout(bubble);
    bubbleLay->setContentsMargins(18, 14, 18, 14);

    QLabel *msg = new QLabel(text);
    msg->setWordWrap(true);
    msg->setTextFormat(Qt::PlainText);
    msg->setTextInteractionFlags(Qt::TextSelectableByMouse);
    msg->setMinimumWidth(fromUser ? 240 : 360);
    msg->setStyleSheet(crisis
        ? "color:#7c2d12; font-size:14px; line-height:1.35; border:none; background:transparent;"
        : fromUser
            ? "color:#082f49; font-size:14px; line-height:1.35; border:none; background:transparent;"
            : "color:#274334; font-size:14px; line-height:1.35; border:none; background:transparent;");
    bubbleLay->addWidget(msg);

    if (fromUser) {
        rowLay->addStretch();
        rowLay->addWidget(bubble);
    } else {
        rowLay->addWidget(bubble);
        rowLay->addStretch();
    }

    return row;
}

void AssistantChat::addMessage(const QString &text, bool fromUser, bool crisis) {
    QWidget *row = makeMessageRow(text, fromUser, crisis);
    const int insertIndex = qMax(0, m_messagesLayout->count() - 1);
    m_messagesLayout->insertWidget(insertIndex, row);

    m_messagesWidget->adjustSize();
    if (m_scroll) {
        m_scroll->ensureWidgetVisible(row, 0, 130);
    }
    scrollToBottom();
}

void AssistantChat::sendMessage() {
    if (m_waitingForReply) return;

    const QString message = m_input->text().trimmed();
    if (message.isEmpty()) return;

    const QJsonArray history = recentHistoryPayload();

    addMessage(message, true);
    appendConversationTurn("user", message);
    m_input->clear();
    setBusy(true);
    scrollToBottom();

    ensureBackendReady(message, history);
}

void AssistantChat::ensureBackendReady(const QString &message,
                                       const QJsonArray &history,
                                       int attempt) {
    QNetworkRequest request((QUrl(backendHealthUrl())));
    request.setTransferTimeout(2200);

    QNetworkReply *reply = m_network->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply, message, history, attempt]() {
        const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        const bool backendReady =
            reply->error() == QNetworkReply::NoError &&
            statusCode >= 200 &&
            statusCode < 300;
        const QString errorText = reply->errorString();
        reply->deleteLater();

        if (backendReady) {
            sendChatRequest(message, history);
            return;
        }

        if (!canAutoStartBackend()) {
            failBackendStartup(
                "Automatic startup only works with the local backend at 127.0.0.1 or localhost.\n"
                "Current backend: " + backendBaseUrl() + "\n"
                "Details: " + errorText);
            return;
        }

        if (!m_backendStartAttempted) {
            if (!startBackendProcess()) {
                return;
            }
        }

        if (m_backendStartAttempted &&
            m_backendProcess->state() == QProcess::NotRunning &&
            attempt > 2) {
            QString detail = "The backend process stopped before it became ready.";
            if (!m_backendLastOutput.trimmed().isEmpty()) {
                detail += "\n\nBackend output:\n" + m_backendLastOutput.trimmed();
            }
            failBackendStartup(detail);
            return;
        }

        constexpr int maxAttempts = 180; // About 90 seconds, enough for a first local npm install.
        if (attempt >= maxAttempts) {
            QString detail = "The backend did not become ready in time.";
            if (!m_backendLastOutput.trimmed().isEmpty()) {
                detail += "\n\nBackend output:\n" + m_backendLastOutput.trimmed();
            }
            failBackendStartup(detail);
            return;
        }

        m_statusLbl->setText("Starting local assistant...");
        m_statusLbl->setVisible(true);
        QTimer::singleShot(500, this, [this, message, history, attempt]() {
            ensureBackendReady(message, history, attempt + 1);
        });
    });
}

bool AssistantChat::startBackendProcess() {
    if (m_backendProcess->state() != QProcess::NotRunning) {
        return true;
    }

    const QString backendDir = backendDirectory();
    if (backendDir.isEmpty()) {
        failBackendStartup(
            "MindEase could not find the backend folder from the current app location.");
        return false;
    }

    m_backendStartAttempted = true;
    m_backendLastOutput.clear();
    m_backendProcess->setWorkingDirectory(backendDir);

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    const QString safePath =
        "/opt/homebrew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:" +
        env.value("PATH");
    env.insert("PATH", safePath);
    m_backendProcess->setProcessEnvironment(env);

    const QString command =
        "export PATH=/opt/homebrew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:$PATH"
        " && cd " + shellQuote(backendDir) +
        " && if [ ! -d node_modules ]; then npm install --no-audit --no-fund; fi"
        " && exec node server.js";

    m_statusLbl->setText("Starting local assistant...");
    m_statusLbl->setVisible(true);
    m_backendProcess->start("/bin/zsh", { "-lc", command });

    if (!m_backendProcess->waitForStarted(1500)) {
        failBackendStartup(
            "MindEase could not launch Node.js automatically.\n"
            "Check that Node.js and npm are installed.\n\n"
            "Details: " + m_backendProcess->errorString());
        m_backendStartAttempted = false;
        return false;
    }

    return true;
}

void AssistantChat::sendChatRequest(const QString &message, const QJsonArray &history) {
    m_statusLbl->clear();
    m_statusLbl->setVisible(false);

    QJsonObject payload;
    payload["message"] = message;
    payload["history"] = history;

    QNetworkRequest request((QUrl(backendUrl())));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = m_network->post(
        request,
        QJsonDocument(payload).toJson(QJsonDocument::Compact));

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleReply(reply);
    });
}

void AssistantChat::handleReply(QNetworkReply *reply) {
    const QByteArray body = reply->readAll();
    const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const bool networkError = reply->error() != QNetworkReply::NoError;
    const QString errorText = reply->errorString();
    reply->deleteLater();

    setBusy(false);

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(body, &parseError);
    const QJsonObject obj = doc.isObject() ? doc.object() : QJsonObject{};

    if (networkError) {
        const QString backendHelp =
            "I could not reach the local MindEase assistant backend.\n\n"
            "MindEase tried to start it automatically. Check that Node.js is installed and your OPENAI_API_KEY is saved in backend/.env.\n\n"
            "Details: " + errorText;
        m_statusLbl->setText("Backend unavailable.");
        m_statusLbl->setVisible(true);
        addMessage(backendHelp, false);
        return;
    }

    if (statusCode >= 400) {
        const QString serverError = obj.value("error").toString("The backend returned an error.");
        m_statusLbl->setText("MindEase could not finish that reply.");
        m_statusLbl->setVisible(true);
        addMessage(serverError, false);
        return;
    }

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        m_statusLbl->setText("MindEase returned an unreadable reply.");
        m_statusLbl->setVisible(true);
        addMessage("The backend returned an unreadable response. Please try again.", false);
        return;
    }

    const QString botReply = obj.value("reply").toString().trimmed();
    const bool crisis = obj.value("crisis").toBool(false);
    const QString replyText = botReply.isEmpty()
        ? "I am here with you. Try naming the main stressor, then choose one small next step."
        : botReply;

    addMessage(replyText, false, crisis);
    appendConversationTurn("assistant", replyText);
    m_statusLbl->setText(crisis ? "MindEase switched to crisis support guidance." : "MindEase is ready.");
    m_statusLbl->setVisible(true);
    scrollToBottom();
}

void AssistantChat::failBackendStartup(const QString &detail) {
    m_backendStartAttempted = false;
    setBusy(false);
    m_statusLbl->setText("Assistant is offline. Press Send to try again.");
    m_statusLbl->setVisible(true);

    QString conciseDetail = detail.section('\n', 0, 0).trimmed();
    if (conciseDetail.isEmpty()) {
        conciseDetail = "The local backend did not become ready.";
    }

    addMessage(
        "I could not connect to the local MindEase assistant yet.\n\n"
        "MindEase will try again when you press Send. Check that Node.js/npm are installed and OPENAI_API_KEY is saved in backend/.env.\n\n"
        + conciseDetail,
        false);
}

void AssistantChat::appendConversationTurn(const QString &role, const QString &text) {
    const QString cleanRole = role.trimmed().toLower();
    const QString cleanText = text.trimmed();

    if ((cleanRole != "user" && cleanRole != "assistant") || cleanText.isEmpty()) {
        return;
    }

    m_conversation.push_back({ cleanRole, cleanText });

    constexpr int maxStoredTurns = 20;
    while (m_conversation.size() > maxStoredTurns) {
        m_conversation.removeFirst();
    }
}

QJsonArray AssistantChat::recentHistoryPayload() const {
    constexpr int maxHistoryTurns = 12;
    QJsonArray history;
    const int startIndex = qMax(0, m_conversation.size() - maxHistoryTurns);

    for (int i = startIndex; i < m_conversation.size(); ++i) {
        const ChatTurn &turn = m_conversation.at(i);
        if (turn.text.trimmed().isEmpty()) {
            continue;
        }

        QJsonObject item;
        item["role"] = turn.role;
        item["text"] = turn.text;
        history.append(item);
    }

    return history;
}

void AssistantChat::scrollToBottom() {
    if (!m_scroll || !m_scroll->verticalScrollBar()) return;

    auto scroll = [this]() {
        if (m_scroll && m_scroll->verticalScrollBar()) {
            QScrollBar *bar = m_scroll->verticalScrollBar();
            bar->setValue(bar->maximum());
        }
    };

    QTimer::singleShot(0, this, scroll);
    QTimer::singleShot(60, this, scroll);
    QTimer::singleShot(140, this, scroll);
    QTimer::singleShot(320, this, scroll);
}

void AssistantChat::setBusy(bool busy) {
    m_waitingForReply = busy;
    m_sendBtn->setEnabled(!busy);
    m_input->setEnabled(!busy);
    m_statusLbl->clear();
    m_statusLbl->setVisible(false);

    if (!busy) {
        m_input->setFocus();
    }
}

QString AssistantChat::backendUrl() const {
    return backendBaseUrl() + "/api/chat";
}

QString AssistantChat::backendHealthUrl() const {
    return backendBaseUrl() + "/health";
}

QString AssistantChat::backendBaseUrl() const {
    QString baseUrl = qEnvironmentVariable("MINDEASE_API_URL", "http://127.0.0.1:8788");
    while (baseUrl.endsWith('/')) {
        baseUrl.chop(1);
    }
    return baseUrl;
}

QString AssistantChat::backendDirectory() const {
    const QStringList starts = {
        QDir::currentPath(),
        QCoreApplication::applicationDirPath()
    };

    for (const QString &start : starts) {
        QDir dir(start);
        for (int depth = 0; depth < 12; ++depth) {
            const QString candidate = dir.filePath("backend");
            if (QFileInfo::exists(QDir(candidate).filePath("package.json"))) {
                return QDir(candidate).absolutePath();
            }
            if (!dir.cdUp()) {
                break;
            }
        }
    }

    return QString();
}

QString AssistantChat::shellQuote(const QString &value) const {
    QString escaped = value;
    escaped.replace("'", "'\"'\"'");
    return "'" + escaped + "'";
}

bool AssistantChat::canAutoStartBackend() const {
    const QUrl url(backendBaseUrl());
    const QString host = url.host().toLower();
    return host == "127.0.0.1" || host == "localhost" || host == "::1";
}

void AssistantChat::onThemeChanged(bool dark) {
    setStyleSheet(chatStylesheet(dark));
}
