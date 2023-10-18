#ifndef MLLM_CPUADD_H
#define MLLM_CPUADD_H

#include "Op.hpp"
#include "CPUBackend.hpp"

namespace mllm {

class CPUAdd final : public Op {
public:
    CPUAdd(Backend *bn, string opName, bool multiThread);
    virtual ~CPUAdd() = default;
    virtual ErrorCode reshape(vector<shared_ptr<Tensor>> inputs, vector<shared_ptr<Tensor>> outputs) override;
    virtual ErrorCode setUp(vector<shared_ptr<Tensor>> inputs, vector<shared_ptr<Tensor>> outputs) override;
    virtual ErrorCode execute(vector<shared_ptr<Tensor>> inputs, vector<shared_ptr<Tensor>> outputs) override;

    virtual ErrorCode load(ParamLoader &loader) override;

private:
    bool support_multi_thread_ = false;
};

class CPUAddCreator : public CPUBackend::Creator {
public:
    virtual Op *create(OpParam op_param, Backend *bn, string name) const {
        return new CPUAdd(bn, name, false);
    }
};

} // namespace mllm

#endif // MLLM_CPUADD_H