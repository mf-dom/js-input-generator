#include "schema.h"
#include <google/protobuf/struct.pb.h>
#include <google/protobuf/util/json_util.h>

using namespace google::protobuf;
using namespace google::protobuf::util;

FieldBuilder::FieldBuilder(FieldDescriptorProto *proto) : proto(proto) {
}

FieldBuilder *FieldBuilder::with_name(std::string name) {
    this->proto->set_name(name);
    return this;
}

FieldBuilder *FieldBuilder::with_number(int32 number) {
    this->proto->set_number(number);
    return this;
}

FieldBuilder *FieldBuilder::with_label(FieldDescriptorProto_Label label) {
    this->proto->set_label(label);
    return this;
}

FieldBuilder *FieldBuilder::with_type(FieldDescriptorProto_Type type) {
    this->proto->set_type(type);
    return this;
}

FieldBuilder *FieldBuilder::with_named_type(std::string struct_name) {
    this->proto->set_type(FieldDescriptorProto_Type_TYPE_MESSAGE);
    this->proto->set_type_name(struct_name);
    return this;
}

SchemaBuilder::SchemaBuilder(DescriptorProto *proto) : proto(proto), field_counter(0), oneof_counter(0) {
}

SchemaBuilder *SchemaBuilder::with_name(std::string name) {
    this->proto->set_name(name);
    return this;
}

FieldBuilder *SchemaBuilder::add_field() {
    auto *raw = this->proto->add_field();
    auto *field = new FieldBuilder(raw);
    this->proto->GetArena()->Own(field);
    this->field_counter++;
    field->with_number(this->field_counter);
    return field;
}

SchemaFactory::SchemaFactory() : arena(), pool(), factory(&pool) {
    FileDescriptorProto temp;
    proto = temp.New(&this->arena);
    proto->set_name("DynamicMessage.proto");
}

void SchemaFactory::AddError(const string &filename, const string &element_name, const Message *descriptor,
                             DescriptorPool::ErrorCollector::ErrorLocation location, const string &message) {
    std::string error = element_name + ": " + message;
    this->errors.push_back(error);
}

SchemaBuilder *SchemaFactory::add_schema() {
    auto *raw = proto->add_message_type();
    auto *schema = new SchemaBuilder(raw);
    this->arena.Own(schema);
    return schema;
}

MutationFactory *SchemaFactory::create_mutator(const string &type, const string &json) {
    auto *base = factory.GetPrototype(this->pool.FindMessageTypeByName(type))->New(&this->arena);
    JsonStringToMessage(json, base);
    auto *mfactory = new MutationFactory(base);
    this->arena.Own(mfactory);
    return mfactory;
}

bool SchemaFactory::build() {
    return this->pool.BuildFileCollectingErrors(*this->proto, this) != nullptr;
}

std::vector<std::string> *SchemaFactory::get_errors() {
    return &this->errors;
}
