#ifndef ASSISTANTCHAT_H
#define ASSISTANTCHAT_H

#include "core/screen.h"

#include <QJsonArray>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkAccessManager>
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

    QString screenId() const override { return "assistant"; }

private slots:
    void sendMessage();

private:
    struct ChatTurn {
        QString role;
        QString text;
    };

    QWidget    *makeMessageRow(const QString &text, bool fromUser, bool crisis) const;

    void addMessage(const QString &text, bool fromUser, bool crisis = false);
    void handleReply(QNetworkReply *reply);
    void appendConversationTurn(const QString &role, const QString &text);
    QJsonArray recentHistoryPayload() const;
    void scrollToBottom();
    void setBusy(bool busy);
    QString backendUrl() const;

    QNetworkAccessManager *m_network;
    QScrollArea           *m_scroll;
    QWidget               *m_messagesWidget;
    QVBoxLayout           *m_messagesLayout;
    QLineEdit             *m_input;
    QPushButton           *m_sendBtn;
    QLabel                *m_statusLbl;
    QVector<ChatTurn>       m_conversation;
    bool                   m_waitingForReply;
};

#endif // ASSISTANTCHAT_H
