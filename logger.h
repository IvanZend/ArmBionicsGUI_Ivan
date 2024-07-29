#ifndef LOGGER_H
#define LOGGER_H

#include <QtWidgets>

struct LogToModelData {
    bool installed;
    QtMessageHandler previous = {};
    QList<QPointer<QObject>> models;
};
Q_GLOBAL_STATIC(LogToModelData, logToModelData)

void logToModelHandler(QtMsgType type, const QMessageLogContext &context,
                       const QString &msg) {
    for (const auto &m : std::as_const(logToModelData->models)) {
        if (auto model = qobject_cast<QAbstractItemModel *>(m)) {
            auto row = model->rowCount();
            model->insertRow(row);
            model->setData(model->index(row, 0), msg);
        } else if (auto doc = qobject_cast<QTextDocument *>(m)) {
            QTextCursor cur(doc);
            cur.movePosition(QTextCursor::End);
            if (cur.position() != 0) cur.insertBlock();
            cur.insertText(msg);
        }
    }
    if (logToModelData->previous) logToModelData->previous(type, context, msg);
}

void logToModel(QObject *model) {
    logToModelData->models.append(QPointer<QObject>(model));
    if (!logToModelData->installed) {
        logToModelData->previous = qInstallMessageHandler(logToModelHandler);
        logToModelData->installed = true;
    }
}

void rescrollToBottom(QAbstractScrollArea *view) {
    static const char kViewAtBottom[] = "viewAtBottom";
    auto *scrollBar = view->verticalScrollBar();
    Q_ASSERT(scrollBar);
    auto rescroller = [scrollBar]() mutable {
        if (scrollBar->property(kViewAtBottom).isNull())
            scrollBar->setProperty(kViewAtBottom, true);
        auto const atBottom = scrollBar->property(kViewAtBottom).toBool();
        if (atBottom) scrollBar->setValue(scrollBar->maximum());
    };
    QObject::connect(scrollBar, &QAbstractSlider::rangeChanged, view, rescroller,
                     Qt::QueuedConnection);
    QObject::connect(scrollBar, &QAbstractSlider::valueChanged, view, [scrollBar] {
        auto const atBottom = scrollBar->value() == scrollBar->maximum();
        scrollBar->setProperty(kViewAtBottom, atBottom);
    });
}

#endif // LOGGER_H
