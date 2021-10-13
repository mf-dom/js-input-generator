#include "mutation.h"
#include <google/protobuf/message.h>
#include <google/protobuf/reflection.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/util/internal/json_objectwriter.h>

using namespace google::protobuf;
using namespace google::protobuf::util;
using namespace google::protobuf::util::converter;
using namespace google::protobuf::io;

MutationFactory::MutationFactory(const Message *base) : base(base),
                                                        mutator(),
                                                        size_hint(base->ByteSizeLong() * 10) {
}

inline const std::string &preferred_name(const FieldDescriptor &field) {
    if (field.has_json_name()) {
        return field.json_name();
    }
    return field.name();
}

void write_to_json(const Message &msg, JsonObjectWriter &writer, const std::string &name = "") {
    const auto *descriptor = msg.GetDescriptor();
    const auto *reflection = msg.GetReflection();

    writer.StartObject(name);
    for (int i = 0; i < descriptor->field_count(); i++) {
        auto *field = descriptor->field(i);
        if (field->is_repeated()) {
            writer.StartList(preferred_name(*field));
            switch (field->type()) {
                case FieldDescriptor::TYPE_DOUBLE:
                    for (const auto val: reflection->GetRepeatedFieldRef<double>(msg, field)) {
                        writer.RenderDouble("", val);
                    }
                    break;
                case FieldDescriptor::TYPE_FLOAT:
                    for (const auto val: reflection->GetRepeatedFieldRef<float>(msg, field)) {
                        writer.RenderFloat("", val);
                    }
                    break;
                case FieldDescriptor::TYPE_BOOL:
                    for (const auto val: reflection->GetRepeatedFieldRef<bool>(msg, field)) {
                        writer.RenderBool("", val);
                    }
                    break;
                case FieldDescriptor::TYPE_STRING:
                    for (const auto &val: reflection->GetRepeatedFieldRef<std::string>(msg, field)) {
                        writer.RenderString("", val);
                    }
                    break;
                case FieldDescriptor::TYPE_MESSAGE:
                    for (const auto &val: reflection->GetRepeatedFieldRef<Message>(msg, field)) {
                        write_to_json(val, writer, "");
                    }
                    break;
                case FieldDescriptor::TYPE_BYTES:
                    for (const auto &val: reflection->GetRepeatedFieldRef<std::string>(msg, field)) {
                        writer.RenderBytes("", val);
                    }
                    break;
                case FieldDescriptor::TYPE_ENUM:
                    // TODO does this work?
                    for (const auto &val: reflection->GetRepeatedFieldRef<int>(msg, field)) {
                        writer.RenderString("", field->enum_type()->value(val)->name());
                    }
                    break;
                case FieldDescriptor::TYPE_FIXED32:
                case FieldDescriptor::TYPE_UINT32:
                    for (const auto &val: reflection->GetRepeatedFieldRef<uint32>(msg, field)) {
                        writer.RenderUint32("", val);
                    }
                    break;
                case FieldDescriptor::TYPE_INT32:
                case FieldDescriptor::TYPE_SFIXED32:
                case FieldDescriptor::TYPE_SINT32:
                    for (const auto &val: reflection->GetRepeatedFieldRef<int32>(msg, field)) {
                        writer.RenderInt32("", val);
                    }
                    break;
                case FieldDescriptor::TYPE_INT64:
                case FieldDescriptor::TYPE_UINT64:
                case FieldDescriptor::TYPE_SINT64:
                case FieldDescriptor::TYPE_FIXED64:
                case FieldDescriptor::TYPE_SFIXED64:
                case FieldDescriptor::TYPE_GROUP:
                    abort();
            }
            writer.EndList();
        } else if (reflection->HasField(msg, field)) {
            switch (field->type()) {
                case FieldDescriptor::TYPE_DOUBLE:
                    writer.RenderDouble(preferred_name(*field), reflection->GetDouble(msg, field));
                    break;
                case FieldDescriptor::TYPE_FLOAT:
                    writer.RenderFloat(preferred_name(*field), reflection->GetFloat(msg, field));
                    break;
                case FieldDescriptor::TYPE_BOOL:
                    writer.RenderBool(preferred_name(*field), reflection->GetBool(msg, field));
                    break;
                case FieldDescriptor::TYPE_STRING:
                    writer.RenderString(preferred_name(*field), reflection->GetString(msg, field));
                    break;
                case FieldDescriptor::TYPE_MESSAGE:
                    write_to_json(reflection->GetMessage(msg, field), writer, preferred_name(*field));
                    break;
                case FieldDescriptor::TYPE_BYTES:
                    writer.RenderBytes(preferred_name(*field), reflection->GetString(msg, field));
                    break;
                case FieldDescriptor::TYPE_ENUM:
                    writer.RenderString(preferred_name(*field),
                                        field->enum_type()->value(reflection->GetEnumValue(msg, field))->name());
                    break;
                case FieldDescriptor::TYPE_FIXED32:
                case FieldDescriptor::TYPE_UINT32:
                    writer.RenderUint32(preferred_name(*field), reflection->GetUInt32(msg, field));
                    break;
                case FieldDescriptor::TYPE_INT32:
                case FieldDescriptor::TYPE_SFIXED32:
                case FieldDescriptor::TYPE_SINT32:
                    writer.RenderInt32(preferred_name(*field), reflection->GetInt32(msg, field));
                    break;
                case FieldDescriptor::TYPE_INT64:
                case FieldDescriptor::TYPE_UINT64:
                case FieldDescriptor::TYPE_SINT64:
                case FieldDescriptor::TYPE_FIXED64:
                case FieldDescriptor::TYPE_SFIXED64:
                case FieldDescriptor::TYPE_GROUP:
                    abort();
            }
        }
    }
    writer.EndObject();
}

std::string MutationFactory::get_mutation() {
    auto *mutated = this->base->New();
    mutated->CopyFrom(*base);
    std::string output;

    this->mutator.Mutate(mutated, this->size_hint);

    StringOutputStream sos(&output);
    CodedOutputStream os(&sos);
    JsonObjectWriter writer("", &os);
    write_to_json(*mutated, writer);
    delete mutated;
    return output;
}
