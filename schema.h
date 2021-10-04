#ifndef JS_INPUT_GENERATOR_SCHEMA_H
#define JS_INPUT_GENERATOR_SCHEMA_H

#include <google/protobuf/stubs/macros.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>

#include "mutation.h"

class FieldBuilder {
private:
    google::protobuf::FieldDescriptorProto *proto;

public:
    explicit FieldBuilder(google::protobuf::FieldDescriptorProto *proto);

    FieldBuilder *with_name(std::string name);

    FieldBuilder *with_number(google::protobuf::int32 number);

    FieldBuilder *with_label(google::protobuf::FieldDescriptorProto_Label label);

    FieldBuilder *with_type(google::protobuf::FieldDescriptorProto_Type type);

    FieldBuilder *with_named_type(std::string struct_name);

    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(FieldBuilder);
};

class SchemaBuilder {
private:
    google::protobuf::int32 field_counter;
    google::protobuf::int32 oneof_counter;

    google::protobuf::DescriptorProto *proto;

public:
    explicit SchemaBuilder(google::protobuf::DescriptorProto *proto);

    SchemaBuilder *with_name(std::string name);

    FieldBuilder *add_field();

    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(SchemaBuilder);
};

class SchemaFactory : google::protobuf::DescriptorPool::ErrorCollector {
private:
    google::protobuf::DynamicMessageFactory factory;
    google::protobuf::DescriptorPool pool;
    google::protobuf::FileDescriptorProto *proto;
    google::protobuf::Arena arena;
    std::vector<std::string> errors;

    void AddError(const std::string &filename, const std::string &element_name, const google::protobuf::Message *descriptor,
                  ErrorLocation location, const std::string &message) override;

public:
    SchemaFactory();

    SchemaBuilder *add_schema();

    bool build();

    MutationFactory *create_mutator(const std::string &type, const std::string &json);

    std::vector<std::string> *get_errors();

    GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(SchemaFactory);
};

#endif //JS_INPUT_GENERATOR_SCHEMA_H
