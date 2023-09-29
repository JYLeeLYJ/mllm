#include <cmath>
#include "CPURMSNorm.hpp"
#include "Tensor.hpp"

namespace mllm {

// template class CPURMSNorm;
// template class CPURMSNorm;

CPURMSNorm::CPURMSNorm(Backend *bn, bool multiThread, float epsilon) :
    Op(bn), epsilon_(epsilon), support_multi_thread_(multiThread) {
    weight_.setBackend(bn);
}

ErrorCode CPURMSNorm::reshape(vector<shared_ptr<Tensor>> &inputs, vector<shared_ptr<Tensor>> &outputs) {
    // RMSNorm 类似于LayerNorm作用于channel维度
    weight_.reshape(1, inputs[0]->channels(), 1, 1); // (C, 1, 1, 1)
    weight_.setName(name() + ".weight");
    outputs[0]->reshape(inputs[0]->num(), inputs[0]->channels(), inputs[0]->height(), inputs[0]->width());
    std::cout << "CPURMSNorm  reshape" << std::endl;
    return NO_ERROR;
}

ErrorCode CPURMSNorm::setUp(vector<shared_ptr<Tensor>> &inputs, vector<shared_ptr<Tensor>> &outputs) {
    if (!inputs[0]->allocted()) {
        inputs[0]->alloc(); //TODO remove
    }
    outputs[0]->alloc();
    weight_.alloc();

    //TEST
//    weight_.fullData(1.0);
//    inputs[0]->fullDataTest();

    std::cout << "CPURMSNorm  setUp" << std::endl;
    return NO_ERROR;
}

ErrorCode CPURMSNorm::execute(vector<shared_ptr<Tensor>> &inputs, vector<shared_ptr<Tensor>> &outputs) {
    auto input = inputs[0];
    int batch = input->num();
    int ch = input->channels();
    int n2 = input->height();
    int n3 = input->width();
    for (int w = 0; w < n3; w++) {
        for (int h = 0; h < n2; h++) {
            for (int n = 0; n < batch; n++) {
                float sum_squares = 0.0F;
                // sum
                for (int c = 0; c < ch; c++) {
                    float value = input->dataAt<float>(n, c, h, w);
                    sum_squares += value * value;
                }
                float rms = std::sqrt(sum_squares / ch );//+ epsilon_);
                // use memset to set the value of the memory block
                for (int c = 0; c < ch; c++) {
                    float value = input->dataAt<float>(n, c, h, w);
                    outputs[0]->setDataAt<float>(n, c, h, w, weight_.dataAt<float>(0, c, 0, 0) * value / rms);
                }
            }
        }
    }
//    input->printData<float>();
//    weight_.printData<float>();
//    outputs[0]->printData<float>();

    std::cout << "CPURMSNorm()" << std::endl;
    return NO_ERROR;
}
ErrorCode CPURMSNorm::load(ParamLoader &loader) {
    return Op::load(loader);
}
} // namespace mllm