#ifndef SENTFEEDBACK_H
#define SENTFEEDBACK_H

#include <QThread>



class SentFeedback : public QThread
{
    Q_OBJECT
public:
    SentFeedback();
    static SentFeedback *object_sentfeedback;
    ~SentFeedback();
    static void sentFeedback(std::vector<float> vector);
    void emitFeedback(std::vector<float> vector);

signals:
    void uploadFeedback(std::vector<float> vector);
};

#endif