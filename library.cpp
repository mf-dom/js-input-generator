#include <google/protobuf/struct.pb.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/empty.pb.h>
#include <google/protobuf/dynamic_message.h>
#include "library.h"
#include "src/mutator.h"
#include "DescriptorGenerator.h"

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
using namespace protobuf_mutator;
using namespace google::protobuf;
using namespace google::protobuf::util;

int main() {
    DescriptorPool pool;
    FileDescriptorProto file;
    file.set_name("JSONCustom.proto");
    DescriptorProto *desc = file.add_message_type();
    desc->set_name("JSONCustom");
    FieldDescriptorProto *hello = desc->add_field();
    hello->set_name("hello");
    hello->set_label(FieldDescriptorProto_Label_LABEL_REQUIRED);
    hello->set_type(FieldDescriptorProto_Type_TYPE_STRING);
    hello->set_number(1);
    pool.BuildFile(file);

    DynamicMessageFactory factory(&pool);
    Message *msg = factory.GetPrototype(pool.FindMessageTypeByName("JSONCustom"))->New();
    std::string stuff = R"({"hello":"world"})";

    JsonStringToMessage(stuff, msg);
    Mutator mutator;

    for (int i = 0; i < 100; i++) {
        stuff.clear();
        auto copy = msg->New();
        copy->CopyFrom(*msg);
        mutator.Mutate(copy, 1000);
        google::protobuf::util::MessageToJsonString(*copy, &stuff);
        std::cout << stuff << std::endl;
    }
}
#endif