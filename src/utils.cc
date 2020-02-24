#include "utils.h"

namespace ie = InferenceEngine;

namespace ienodejs {

namespace utils {

std::map<ie::Precision, std::string> precision_name_map = {
    {ie::Precision::UNSPECIFIED, "unspecified"},
    {ie::Precision::MIXED, "mixed"},
    {ie::Precision::FP32, "fp32"},
    {ie::Precision::FP16, "fp16"},
    {ie::Precision::Q78, "q87"},
    {ie::Precision::I16, "i16"},
    {ie::Precision::U8, "u8"},
    {ie::Precision::I8, "i8"},
    {ie::Precision::U16, "u16"},
    {ie::Precision::I32, "i32"},
    {ie::Precision::I64, "i64"},
    {ie::Precision::BIN, "bin"},
    {ie::Precision::CUSTOM, "custom"},
};

std::map<std::string, ie::Precision> precision_type_map = {
    {"unspecified", ie::Precision::UNSPECIFIED},
    {"mixed", ie::Precision::MIXED},
    {"fp32", ie::Precision::FP32},
    {"fp16", ie::Precision::FP16},
    {"q78", ie::Precision::Q78},
    {"i16", ie::Precision::I16},
    {"u8", ie::Precision::U8},
    {"i8", ie::Precision::I8},
    {"u16", ie::Precision::U16},
    {"i32", ie::Precision::I32},
    {"i64", ie::Precision::I64},
    {"bin", ie::Precision::BIN},
    {"custom", ie::Precision::CUSTOM}};

std::map<ie::Layout, std::string> layout_name_map = {
    {ie::Layout::ANY, "any"},
    {ie::Layout::NCHW, "nchw"},
    {ie::Layout::NHWC, "nhwc"},
    {ie::Layout::NCDHW, "ncdhw"},
    {ie::Layout::NDHWC, "ndhwc"},
    {ie::Layout::OIHW, "oihw"},
    {ie::Layout::SCALAR, "scalar"},
    {ie::Layout::C, "c"},
    {ie::Layout::CHW, "chw"},
    {ie::Layout::HW, "hw"},
    {ie::Layout::NC, "nc"},
    {ie::Layout::CN, "cn"},
    {ie::Layout::BLOCKED, "blocked"}
};

std::map<std::string, ie::Layout> layout_type_map = {
    {"any", ie::Layout::ANY},
    {"nchw", ie::Layout::NCHW},
    {"nhwc", ie::Layout::NHWC},
    {"oihw", ie::Layout::OIHW},
    {"ncdhw", ie::Layout::NCDHW},
    {"ndhwc", ie::Layout::NDHWC},
    {"oihw", ie::Layout::OIHW},
    {"scalar", ie::Layout::SCALAR},
    {"c", ie::Layout::C},
    {"chw", ie::Layout::CHW},
    {"hw", ie::Layout::HW},
    {"nc", ie::Layout::NC},
    {"cn", ie::Layout::CN},
    {"blocked", ie::Layout::BLOCKED}
};

bool IsValidLayoutName(const std::string& name) {
  return !(layout_type_map.find(name) == layout_type_map.end());
}

ie::Layout GetLayoutByName(const std::string& name) {
  return layout_type_map[name];
}

std::string GetNameOfLayout(const ie::Layout& layout) {
  return layout_name_map[layout];
}

bool IsValidPrecisionName(const std::string& name) {
  return !(precision_type_map.find(name) == precision_type_map.end());
}

ie::Precision GetPrecisionByName(const std::string& name) {
  return precision_type_map[name];
}

std::string GetNameOfPrecision(const InferenceEngine::Precision& precision) {
  return precision_name_map[precision];
}

}  // namespace utils
}  // namespace ienodejs