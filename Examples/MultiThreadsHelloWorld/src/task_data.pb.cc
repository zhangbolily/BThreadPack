// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: task_data.proto

#include "task_data.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)

class TaskDataDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<TaskData>
      _instance;
} _TaskData_default_instance_;
namespace protobuf_task_5fdata_2eproto {
static void InitDefaultsTaskData() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::_TaskData_default_instance_;
    new (ptr) ::TaskData();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::TaskData::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<0> scc_info_TaskData =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsTaskData}, {}};

void InitDefaults() {
  ::google::protobuf::internal::InitSCC(&scc_info_TaskData.base);
}

::google::protobuf::Metadata file_level_metadata[1];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::TaskData, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::TaskData, message_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::TaskData, taskid_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::TaskData)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::_TaskData_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  AssignDescriptors(
      "task_data.proto", schemas, file_default_instances, TableStruct::offsets,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\017task_data.proto\"+\n\010TaskData\022\017\n\007Message"
      "\030\001 \001(\t\022\016\n\006TaskID\030\002 \001(\005b\006proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 70);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "task_data.proto", &protobuf_RegisterTypes);
}

void AddDescriptors() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_task_5fdata_2eproto

// ===================================================================

void TaskData::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int TaskData::kMessageFieldNumber;
const int TaskData::kTaskIDFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

TaskData::TaskData()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_task_5fdata_2eproto::scc_info_TaskData.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:TaskData)
}
TaskData::TaskData(const TaskData& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  message_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.message().size() > 0) {
    message_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.message_);
  }
  taskid_ = from.taskid_;
  // @@protoc_insertion_point(copy_constructor:TaskData)
}

void TaskData::SharedCtor() {
  message_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  taskid_ = 0;
}

TaskData::~TaskData() {
  // @@protoc_insertion_point(destructor:TaskData)
  SharedDtor();
}

void TaskData::SharedDtor() {
  message_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void TaskData::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ::google::protobuf::Descriptor* TaskData::descriptor() {
  ::protobuf_task_5fdata_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_task_5fdata_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const TaskData& TaskData::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_task_5fdata_2eproto::scc_info_TaskData.base);
  return *internal_default_instance();
}


void TaskData::Clear() {
// @@protoc_insertion_point(message_clear_start:TaskData)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  message_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  taskid_ = 0;
  _internal_metadata_.Clear();
}

bool TaskData::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:TaskData)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // string Message = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(10u /* 10 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_message()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->message().data(), static_cast<int>(this->message().length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "TaskData.Message"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // int32 TaskID = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(16u /* 16 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &taskid_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:TaskData)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:TaskData)
  return false;
#undef DO_
}

void TaskData::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:TaskData)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string Message = 1;
  if (this->message().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->message().data(), static_cast<int>(this->message().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "TaskData.Message");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      1, this->message(), output);
  }

  // int32 TaskID = 2;
  if (this->taskid() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->taskid(), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:TaskData)
}

::google::protobuf::uint8* TaskData::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:TaskData)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // string Message = 1;
  if (this->message().size() > 0) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->message().data(), static_cast<int>(this->message().length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "TaskData.Message");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->message(), target);
  }

  // int32 TaskID = 2;
  if (this->taskid() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->taskid(), target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:TaskData)
  return target;
}

size_t TaskData::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:TaskData)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // string Message = 1;
  if (this->message().size() > 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::StringSize(
        this->message());
  }

  // int32 TaskID = 2;
  if (this->taskid() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->taskid());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void TaskData::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:TaskData)
  GOOGLE_DCHECK_NE(&from, this);
  const TaskData* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const TaskData>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:TaskData)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:TaskData)
    MergeFrom(*source);
  }
}

void TaskData::MergeFrom(const TaskData& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:TaskData)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.message().size() > 0) {

    message_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.message_);
  }
  if (from.taskid() != 0) {
    set_taskid(from.taskid());
  }
}

void TaskData::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:TaskData)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void TaskData::CopyFrom(const TaskData& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:TaskData)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool TaskData::IsInitialized() const {
  return true;
}

void TaskData::Swap(TaskData* other) {
  if (other == this) return;
  InternalSwap(other);
}
void TaskData::InternalSwap(TaskData* other) {
  using std::swap;
  message_.Swap(&other->message_, &::google::protobuf::internal::GetEmptyStringAlreadyInited(),
    GetArenaNoVirtual());
  swap(taskid_, other->taskid_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::google::protobuf::Metadata TaskData::GetMetadata() const {
  protobuf_task_5fdata_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_task_5fdata_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
namespace google {
namespace protobuf {
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::TaskData* Arena::CreateMaybeMessage< ::TaskData >(Arena* arena) {
  return Arena::CreateInternal< ::TaskData >(arena);
}
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)
