
#ifndef MLLM_NETPARAMETER_H
#define MLLM_NETPARAMETER_H

#include "Types.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string.h>
#include <string>
#include <vector>
#include <cassert>
using std::string;
using std::vector;
using std::map;

namespace mllm {

typedef struct TNetTensor NetTensor;
typedef struct TNetParameter NetParameter;

typedef struct TNetOp {
    OpType type;
    vector<NetTensor *> in;
    vector<NetTensor *> out;
    vector<string> in_op; // input ops' names;
    string name;
    OpParam param;
    int out_size = 1; // output tensor size

} NetOp;

typedef struct TNetParameter {
    string weights_path;
    string model_path;
    // string input_name;
    // string output_name;
    vector<NetOp *> net_ops;
    vector<NetTensor *> net_tensors;
    std::set<NetTensor *> net_inputs;
    std::set<NetTensor *> net_outputs;
    void topologySort();

} NetParameter;

// 前置声明
struct Context {
    vector<NetParameter> sub_param_;
    vector<NetOp *> net_ops;
    std::set<NetTensor *> net_tensors;
    int idx = 0;
    int active_sub = 0;
};
inline NetParameter *get_active_subgraph(Context *ctx) {
    if (ctx->active_sub >= ctx->sub_param_.size()) {
        ctx->sub_param_.emplace_back();
    }
    return &ctx->sub_param_[ctx->active_sub];
}

struct intTensor_pair {
    int start_i;
    NetTensor* end_i;
};
struct Tensor_pair {
    NetTensor* end_i;
};
#define ANYDIM -198098
typedef struct TNetTensor {
    string name;
    vector<int> shape_;
    DataType type;
    NetOp *in;
    vector<NetOp *> out;
    NetParameter *subgraph;
    Context *ctx;

    NetTensor *clip(vector<int> b, vector<int> h, vector<int> s, vector<int> d);
    NetTensor *_clip(intTensor_pair b, intTensor_pair h, intTensor_pair s, intTensor_pair d);
    NetTensor *_clip(Tensor_pair b, Tensor_pair h, Tensor_pair s, Tensor_pair d);
    NetTensor *shape(Chl axis);
    NetTensor *view(int b, int h, int s, int d);
    NetTensor *flatten(Chl axis_start, Chl axis_end);
    NetTensor *transpose(Chl axis1, Chl axis2);
    NetTensor *norm(int L_n);

    // Overload the + operator.
    NetTensor *operator+(NetTensor* in_1) ;
} NetTensor;

} // namespace mllm

#endif // MLLM_NETPARAMETER_H