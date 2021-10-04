//
// Created by addisoncrump on 10/4/21.
//

#ifndef JS_INPUT_GENERATOR_MUTATION_H
#define JS_INPUT_GENERATOR_MUTATION_H

#include <google/protobuf/arena.h>
#include <src/mutator.h>
#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

class MutationFactory {
private:
    const google::protobuf::Message *base;
    size_t size_hint;
    protobuf_mutator::Mutator mutator;

public:
    MutationFactory(const google::protobuf::Message *base, size_t size_hint);

    std::string get_mutation();
};

#endif //JS_INPUT_GENERATOR_MUTATION_H
