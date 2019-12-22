#include "sentFeedback.h"

SentFeedback *SentFeedback::object_sentfeedback;

SentFeedback::SentFeedback(){

    SentFeedback *object_sentfeedback = new SentFeedback();

}

SentFeedback::~SentFeedback(){}

void SentFeedback::sentFeedback(std::vector<float> vector){
    printf("1111\n");
    object_sentfeedback->emitFeedback(vector);
}

void SentFeedback::emitFeedback(std::vector<float> vector){
    printf("2222\n");
    emit this->uploadFeedback(vector);
    
}