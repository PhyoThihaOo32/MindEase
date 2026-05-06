#ifndef ASSISTANTCHAT_H
#define ASSISTANTCHAT_H

#include "core/screen.h"

#include <QJsonArray>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QProcess>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QVector>

class QNetworkReply;

class AssistantChat : public Screen {
    Q_OBJECT

public:
    explicit AssistantChat(QWidget *parent = nullptr);
    ~AssistantChat() override;

    QString screenId() const override { return "assistant"; }
    void onActivated() override;
    void onThemeChanged(bool dark) override;   // Feature 5: Zen Night readability

private slots:
    void sendMessage();

private:
    struct ChatTurn {
        QString role;
        QString text;
    };

    QWidget    *makeMessageRow(const QString &text, bool fromUser, bool crisis) const;

    void addMessage(const QString &text, bool fromUser, bool crisis = false);
    void ensureBackendReady(const QString &message, const QJsonArray &history, int attempt = 0);
    bool startBackendProcess();
    void sendChatRequest(const QString &message, const QJsonArray &history);
    void handleReply(QNetworkReply *reply);
    void failBackendStartup(const QString &detail);
    void appendConversationTurn(const QString &role, const QString &text);
    QJsonArray recentHistoryPayload() const;
    void scrollToBottom();
    void setBusy(bool busy);
    QString backendBaseUrl() const;
    QString backendUrl() const;
    QString backendHealthUrl() const;
    QString backendDirectory() const;
    QString shellQuote(const QString &value) const;
    bool canAutoStartBackend() const;

    QNetworkAccessManager *m_network;
    QProcess              *m_backendProcess;
    QScrollArea           *m_scroll;
    QWidget               *m_messagesWidget;
    QVBoxLayout           *m_messagesLayout;
    QLineEdit             *m_input;
    QPushButton           *m_sendBtn;
    QLabel                *m_statusLbl;
    QVector<ChatTurn>       m_conversation;
    bool                   m_waitingForReply;
    bool                   m_backendStartAttempted;
    QString                m_backendLastOutput;
};

#endif // ASSISTANTCHAT_H
