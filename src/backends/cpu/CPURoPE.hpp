#ifndef MLLM_CPUROPE_H
#define MLLM_CPUROPE_H

#include "Op.hpp"
#include "CPUBackend.hpp"

namespace mllm {

class CPURoPE final : public Op {
public:
    CPURoPE(Backend *bn, string opName, bool hf, bool multiThread);
    virtual ~CPURoPE() = default;
    virtual ErrorCode reshape(vector<shared_ptr<Tensor>> inputs, vector<shared_ptr<Tensor>> outputs) override;
    virtual ErrorCode setUp(vector<shared_ptr<Tensor>> inputs, vector<shared_ptr<Tensor>> outputs) override;
    virtual ErrorCode execute(vector<shared_ptr<Tensor>> inputs, vector<shared_ptr<Tensor>> outputs) override;

    virtual ErrorCode load(ParamLoader &loader) override;

private:
    Tensor sin_;
    Tensor cos_;
    bool hf_;
    bool support_multi_thread_ = false;
};

class CPURoPECreator : public CPUBackend::Creator {
public:
    virtual Op *create(OpParam op_param, Backend *bn, string name) const {
        return new CPURoPE(bn, name,true, false);
    }
};
} // namespace mllm

#endif // MLLM_CPUROPE_H