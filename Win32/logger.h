#ifndef LOGGER_H
#define LOGGER_H

#include <QtWidgets>

/**
 * @brief Structure to hold logging data and state.
 *
 * This structure stores information about the installed log handler,
 * the previous log handler, and a list of models to which log messages
 * should be appended.
 */
struct LogToModelData {
    bool installed; ///< Flag indicating if the custom log handler is installed.
    QtMessageHandler previous = {}; ///< Previous Qt message handler before installing the custom one.
    QList<QPointer<QObject>> models; ///< List of models or documents where log messages will be sent.
};
Q_GLOBAL_STATIC(LogToModelData, logToModelData)

/**
 * @brief Custom message handler for logging.
 *
 * This function is used to handle log messages and append them to
 * specified models or documents. It is called by Qt's logging system
 * whenever a log message is generated.
 *
 * @param type The type of the message (e.g., QtDebugMsg, QtWarningMsg, etc.).
 * @param context The context in which the message was generated.
 * @param msg The log message text.
 */
void logToModelHandler(QtMsgType type, const QMessageLogContext &context,
                       const QString &msg) {
    // Iterate through the list of models and append the log message
    for (const auto &m : std::as_const(logToModelData->models)) {
        if (auto model = qobject_cast<QAbstractItemModel *>(m)) {
            // Handle models derived from QAbstractItemModel
            auto row = model->rowCount();
            model->insertRow(row);
            model->setData(model->index(row, 0), msg);
        } else if (auto doc = qobject_cast<QTextDocument *>(m)) {
            // Handle QTextDocument models
            QTextCursor cur(doc);
            cur.movePosition(QTextCursor::End);
            if (cur.position() != 0) cur.insertBlock();
            cur.insertText(msg);
        }
    }
    // Call the previous message handler if it exists
    if (logToModelData->previous) logToModelData->previous(type, context, msg);
}

/**
 * @brief Registers a model or document to receive log messages.
 *
 * This function adds the provided model or document to the list of
 * targets for log messages. If the custom log handler is not yet installed,
 * it installs it.
 *
 * @param model Pointer to the QObject-based model or QTextDocument to receive log messages.
 */
void logToModel(QObject *model) {
    logToModelData->models.append(QPointer<QObject>(model));
    if (!logToModelData->installed) {
        logToModelData->previous = qInstallMessageHandler(logToModelHandler);
        logToModelData->installed = true;
    }
}

/**
 * @brief Ensures the view scrolls to the bottom when new content is added.
 *
 * This function sets up connections to automatically scroll a QAbstractScrollArea
 * to the bottom whenever the scroll area’s content changes. This is useful for
 * log views or text areas where you want to keep the most recent content visible.
 *
 * @param view Pointer to the QAbstractScrollArea (e.g., QTextEdit) to be auto-scrolled.
 */
void rescrollToBottom(QAbstractScrollArea *view) {
    static const char kViewAtBottom[] = "viewAtBottom";
    auto *scrollBar = view->verticalScrollBar();
    Q_ASSERT(scrollBar);

    // Lambda function to scroll to the bottom if needed
    auto rescroller = [scrollBar]() mutable {
        if (scrollBar->property(kViewAtBottom).isNull())
            scrollBar->setProperty(kViewAtBottom, true);
        auto const atBottom = scrollBar->property(kViewAtBottom).toBool();
        if (atBottom) scrollBar->setValue(scrollBar->maximum());
    };

    // Connect scroll bar signals to the rescroller lambda
    QObject::connect(scrollBar, &QAbstractSlider::rangeChanged, view, rescroller,
                     Qt::QueuedConnection);
    QObject::connect(scrollBar, &QAbstractSlider::valueChanged, view, [scrollBar] {
        auto const atBottom = scrollBar->value() == scrollBar->maximum();
        scrollBar->setProperty(kViewAtBottom, atBottom);
    });
}

#endif // LOGGER_H
