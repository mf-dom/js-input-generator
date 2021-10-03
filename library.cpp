#include "schema.h"

#ifdef EMSCRIPTEN

#include <emscripten/bind.h>

using namespace emscripten;
using namespace google::protobuf;

EMSCRIPTEN_BINDINGS(inputgen) {
    class_<FieldBuilder>("FieldBuilder")
            .function("withName", &FieldBuilder::with_name, allow_raw_pointers())
            .function("withLabel", &FieldBuilder::with_label, allow_raw_pointers())
            .function("withType", &FieldBuilder::with_type, allow_raw_pointers())
            .function("withNamedType", &FieldBuilder::with_named_type, allow_raw_pointers())
            ;
    class_<SchemaBuilder>("SchemaBuilder")
            .function("addField", &SchemaBuilder::add_field, allow_raw_pointers())
            .function("withName", &SchemaBuilder::with_name, allow_raw_pointers())
            ;
    class_<SchemaFactory>("SchemaFactory")
            .function("addSchema", &SchemaFactory::add_schema, allow_raw_pointers())
            .function("createInstance", &SchemaFactory::create_instance, allow_raw_pointers())
            .function("build", &SchemaFactory::build)
            .function("getErrors", &SchemaFactory::get_errors, allow_raw_pointers())
            ;
    enum_<FieldDescriptorProto_Label>("FieldLabel")
            .value("OPTIONAL", FieldDescriptorProto_Label_LABEL_OPTIONAL)
            .value("REQUIRED", FieldDescriptorProto_Label_LABEL_REQUIRED)
            .value("REPEATED", FieldDescriptorProto_Label_LABEL_REPEATED)
            ;
    enum_<FieldDescriptorProto_Type>("FieldType")
            .value("DOUBLE", FieldDescriptorProto_Type_TYPE_DOUBLE)
            .value("FLOAT", FieldDescriptorProto_Type_TYPE_FLOAT)
            .value("INT64", FieldDescriptorProto_Type_TYPE_INT64)
            .value("UINT64", FieldDescriptorProto_Type_TYPE_UINT64)
            .value("INT32", FieldDescriptorProto_Type_TYPE_INT32)
            .value("BOOL", FieldDescriptorProto_Type_TYPE_BOOL)
            .value("STRING", FieldDescriptorProto_Type_TYPE_STRING)
            .value("GROUP", FieldDescriptorProto_Type_TYPE_GROUP)
            // MESSAGE omitted; use
            .value("BYTES", FieldDescriptorProto_Type_TYPE_BYTES)
            .value("UINT32", FieldDescriptorProto_Type_TYPE_UINT32)
            .value("SFIXED32", FieldDescriptorProto_Type_TYPE_SFIXED32)
            .value("SFIXED64", FieldDescriptorProto_Type_TYPE_SFIXED64)
            .value("SINT32", FieldDescriptorProto_Type_TYPE_SINT32)
            .value("SINT64", FieldDescriptorProto_Type_TYPE_SINT64)
            ;
}

#else
#include <google/protobuf/struct.pb.h>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/empty.pb.h>
#include <google/protobuf/dynamic_message.h>
#include "src/mutator.h"

using namespace protobuf_mutator;
using namespace google::protobuf;
using namespace google::protobuf::util;

int main() {
    SchemaFactory factory;
    auto *sbuilder = factory.add_schema();
    sbuilder->with_name("JSONCustom");
    auto *fbuilder = sbuilder->add_field();
    fbuilder->with_name("hello");
    fbuilder->with_label(FieldDescriptorProto_Label_LABEL_REQUIRED);
    fbuilder->with_type(FieldDescriptorProto_Type_TYPE_STRING);
    if (!factory.build()) {
        for (const auto &error : *factory.get_errors()) {
            std::cout << error << std::endl;
        }
    }

    Message *msg = factory.create_instance("JSONCustom");
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
