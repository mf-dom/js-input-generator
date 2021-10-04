//
// Created by addisoncrump on 10/4/21.
//

#include "mutation.h"
#include <google/protobuf/util/json_util.h>

using namespace google::protobuf;
using namespace google::protobuf::util;

MutationFactory::MutationFactory(const Message *base,
                                 size_t size_hint) : base(base),
                                                     mutator(),
                                                     size_hint(size_hint * 100) {}

std::string MutationFactory::get_mutation() {
    auto *mutated = this->base->New();
    mutated->CopyFrom(*base);
    this->mutator.Mutate(mutated, this->size_hint);
    std::string res;
    MessageToJsonString(*mutated, &res);
    delete mutated;
    return res;
}
