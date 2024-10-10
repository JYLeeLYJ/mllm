#include "backends/xnnpack/Ops/XpBinary.hpp"
#include "Types.hpp"
#include "backends/xnnpack/XpInterface.hpp"

namespace mllm::xnnpack {

ErrorCode XpAdd::setUp(vector<shared_ptr<Tensor>> inputs, vector<shared_ptr<Tensor>> outputs) {
    auto xnnbk = (XnnpackBackend *)(this->backend());

    // define output tensor
    // defineXpTensor(xnnbk, outputs[0], XpTensorType::Normal);

    // the inputs should already has uuid.

    // create binary op: add
    auto status = xnn_define_binary(
        xnnbk->getXnnSubgraph(),
        xnn_binary_add,
        nullptr,
        inputs[0]->uuid(),
        inputs[1]->uuid(),
        outputs[0]->uuid(),
        0);

    if (status != xnn_status_success) {
        Log::error("XpAdd::setUp Error");
        exit(-1);
    }
    return MLLM_NO_ERROR;
}

ErrorCode XpAdd::reshape(vector<shared_ptr<Tensor>> inputs, vector<shared_ptr<Tensor>> outputs) {
    assert(inputs.size() == 2);
    assert(outputs.size() == 1);
    if (inputs[0]->batch() == 1 || inputs[1]->batch() == 1) {
    } else {
        assert(inputs[0]->batch() == inputs[1]->batch());
    }
    assert(inputs[0]->head() == inputs[1]->head());
    assert(inputs[0]->sequence() == inputs[1]->sequence());
    assert(inputs[0]->dimension() == inputs[1]->dimension());
    outputs[0]->reshape(std::max(inputs[0]->batch(), inputs[1]->batch()), inputs[0]->head(), inputs[0]->sequence(), inputs[0]->dimension());
    // outputs[0]->setDtype(activationDtype());
    return Op::reshape(inputs, outputs);
}

ErrorCode XpAdd::execute(vector<shared_ptr<Tensor>> inputs, vector<shared_ptr<Tensor>> outputs) {
    // do not execute this op
    return MLLM_NO_ERROR;
}

Op *XpAddCreator::create(OpParam op_param, Backend *bk, const string &name, int thread_count) const {
    return new XpAdd(bk, name, thread_count);
}
} // namespace mllm::xnnpack