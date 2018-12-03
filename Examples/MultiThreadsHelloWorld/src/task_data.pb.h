// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: task_data.proto

#ifndef PROTOBUF_INCLUDED_task_5fdata_2eproto
#define PROTOBUF_INCLUDED_task_5fdata_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_task_5fdata_2eproto 

namespace protobuf_task_5fdata_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_task_5fdata_2eproto
class TaskData;
class TaskDataDefaultTypeInternal;
extern TaskDataDefaultTypeInternal _TaskData_default_instance_;
namespace google {
namespace protobuf {
template<> ::TaskData* Arena::CreateMaybeMessage<::TaskData>(Arena*);
}  // namespace protobuf
}  // namespace google

// ===================================================================

class TaskData : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:TaskData) */ {
 public:
  TaskData();
  virtual ~TaskData();

  TaskData(const TaskData& from);

  inline TaskData& operator=(const TaskData& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  TaskData(TaskData&& from) noexcept
    : TaskData() {
    *this = ::std::move(from);
  }

  inline TaskData& operator=(TaskData&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const TaskData& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const TaskData* internal_default_instance() {
    return reinterpret_cast<const TaskData*>(
               &_TaskData_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(TaskData* other);
  friend void swap(TaskData& a, TaskData& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline TaskData* New() const final {
    return CreateMaybeMessage<TaskData>(NULL);
  }

  TaskData* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<TaskData>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const TaskData& from);
  void MergeFrom(const TaskData& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(TaskData* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string Message = 1;
  void clear_message();
  static const int kMessageFieldNumber = 1;
  const ::std::string& message() const;
  void set_message(const ::std::string& value);
  #if LANG_CXX11
  void set_message(::std::string&& value);
  #endif
  void set_message(const char* value);
  void set_message(const char* value, size_t size);
  ::std::string* mutable_message();
  ::std::string* release_message();
  void set_allocated_message(::std::string* message);

  // int32 TaskID = 2;
  void clear_taskid();
  static const int kTaskIDFieldNumber = 2;
  ::google::protobuf::int32 taskid() const;
  void set_taskid(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:TaskData)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr message_;
  ::google::protobuf::int32 taskid_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_task_5fdata_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// TaskData

// string Message = 1;
inline void TaskData::clear_message() {
  message_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& TaskData::message() const {
  // @@protoc_insertion_point(field_get:TaskData.Message)
  return message_.GetNoArena();
}
inline void TaskData::set_message(const ::std::string& value) {
  
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:TaskData.Message)
}
#if LANG_CXX11
inline void TaskData::set_message(::std::string&& value) {
  
  message_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:TaskData.Message)
}
#endif
inline void TaskData::set_message(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:TaskData.Message)
}
inline void TaskData::set_message(const char* value, size_t size) {
  
  message_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:TaskData.Message)
}
inline ::std::string* TaskData::mutable_message() {
  
  // @@protoc_insertion_point(field_mutable:TaskData.Message)
  return message_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* TaskData::release_message() {
  // @@protoc_insertion_point(field_release:TaskData.Message)
  
  return message_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void TaskData::set_allocated_message(::std::string* message) {
  if (message != NULL) {
    
  } else {
    
  }
  message_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), message);
  // @@protoc_insertion_point(field_set_allocated:TaskData.Message)
}

// int32 TaskID = 2;
inline void TaskData::clear_taskid() {
  taskid_ = 0;
}
inline ::google::protobuf::int32 TaskData::taskid() const {
  // @@protoc_insertion_point(field_get:TaskData.TaskID)
  return taskid_;
}
inline void TaskData::set_taskid(::google::protobuf::int32 value) {
  
  taskid_ = value;
  // @@protoc_insertion_point(field_set:TaskData.TaskID)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_task_5fdata_2eproto
