#include <google/protobuf/struct.pb.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/empty.pb.h>
#include "library.h"
#include "src/mutator.h"
#include "src/text_format.h"

#ifdef EMSCRIPTEN

#include <iostream>
#include <emscripten.h>

#else
#define EMSCRIPTEN_KEEPALIVE
#endif

extern "C" {
EMSCRIPTEN_KEEPALIVE
void hello() {
}
}

#ifndef EMSCRIPTEN
int main() {
    protobuf_mutator::Mutator mutator;
    google::protobuf::Struct msg;
    std::string stuff = R"(
{
  "hello": "world"
}
)";

    google::protobuf::util::JsonStringToMessage(stuff, &msg);

    for (int i = 0; i < 100; i++) {
        stuff.clear();
        google::protobuf::util::MessageToJsonString(msg, &stuff);
        std::cout << stuff << std::endl;
        mutator.Mutate(&msg, 1000);
    }
}
#endif