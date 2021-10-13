#ifndef JS_INPUT_GENERATOR_MUTATION_H
#define JS_INPUT_GENERATOR_MUTATION_H

#include <google/protobuf/message.h>
#include <src/mutator.h>

class MutationFactory {
private:
    const google::protobuf::Message *base;
    size_t size_hint;
    protobuf_mutator::Mutator mutator;

public:
    explicit MutationFactory(const google::protobuf::Message *base);

    std::string get_mutation();
};

#endif //JS_INPUT_GENERATOR_MUTATION_H
