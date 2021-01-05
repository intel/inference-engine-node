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
    {ie::Layout::ANY, "any"},        {ie::Layout::NCHW, "nchw"},
    {ie::Layout::NHWC, "nhwc"},      {ie::Layout::NCDHW, "ncdhw"},
    {ie::Layout::NDHWC, "ndhwc"},    {ie::Layout::OIHW, "oihw"},
    {ie::Layout::SCALAR, "scalar"},  {ie::Layout::C, "c"},
    {ie::Layout::CHW, "chw"},        {ie::Layout::HW, "hw"},
    {ie::Layout::NC, "nc"},          {ie::Layout::CN, "cn"},
    {ie::Layout::BLOCKED, "blocked"}};

std::map<std::string, ie::Layout> layout_type_map = {
    {"any", ie::Layout::ANY},     {"nchw", ie::Layout::NCHW},
    {"nhwc", ie::Layout::NHWC},   {"oihw", ie::Layout::OIHW},
    {"ncdhw", ie::Layout::NCDHW}, {"ndhwc", ie::Layout::NDHWC},
    {"oihw", ie::Layout::OIHW},   {"scalar", ie::Layout::SCALAR},
    {"c", ie::Layout::C},         {"chw", ie::Layout::CHW},
    {"hw", ie::Layout::HW},       {"nc", ie::Layout::NC},
    {"cn", ie::Layout::CN},       {"blocked", ie::Layout::BLOCKED}};

std::map<ie::ColorFormat, std::string> colorformat_name_map = {
    {ie::ColorFormat::RAW, "raw"},   {ie::ColorFormat::RGB, "rgb"},
    {ie::ColorFormat::BGR, "bgr"},   {ie::ColorFormat::RGBX, "rgbx"},
    {ie::ColorFormat::BGRX, "bgrx"}, {ie::ColorFormat::NV12, "nv12"},
    {ie::ColorFormat::I420, "i420"}};

std::map<std::string, ie::ColorFormat> colorformat_type_map = {
    {"raw", ie::ColorFormat::RAW},   {"rgb", ie::ColorFormat::RGB},
    {"bgr", ie::ColorFormat::BGR},   {"rgbx", ie::ColorFormat::RGBX},
    {"bgrx", ie::ColorFormat::BGRX}, {"nv12", ie::ColorFormat::NV12},
    {"i420", ie::ColorFormat::I420}};

std::map<ie::ResizeAlgorithm, std::string> resizeAlgorithm_name_map = {
    {ie::ResizeAlgorithm::NO_RESIZE, "no_resize"},
    {ie::ResizeAlgorithm::RESIZE_BILINEAR, "resize_bilinear"},
    {ie::ResizeAlgorithm::RESIZE_AREA, "resize_area"}};

std::map<std::string, ie::ResizeAlgorithm> resizeAlgorithm_type_map = {
    {"no_resize", ie::ResizeAlgorithm::NO_RESIZE},
    {"resize_bilinear", ie::ResizeAlgorithm::RESIZE_BILINEAR},
    {"resize_area", ie::ResizeAlgorithm::RESIZE_AREA}};

std::map<ie::MeanVariant, std::string> meanvariant_name_map = {
    {ie::MeanVariant::MEAN_IMAGE, "mean_image"},
    {ie::MeanVariant::MEAN_VALUE, "mean_value"},
    {ie::MeanVariant::NONE, "none"}};

std::map<std::string, ie::MeanVariant> meanvariant_type_map = {
    {"mean_image", ie::MeanVariant::MEAN_IMAGE},
    {"mean_value", ie::MeanVariant::MEAN_VALUE},
    {"none", ie::MeanVariant::NONE}};

bool checkTensorDesc(const Napi::Object& tensorDesc) {
  if (tensorDesc.Has("precision") && tensorDesc.Has("dims") &&
      tensorDesc.Has("layout")) {
    if (tensorDesc.Get("precision").IsString() &&
        tensorDesc.Get("layout").IsString() &&
        tensorDesc.Get("dims").IsArray()) {
      std::string precision = tensorDesc.Get("precision").ToString();
      std::string layout = tensorDesc.Get("layout").ToString();
      if (utils::IsValidPrecisionName(precision) &&
          utils::IsValidLayoutName(layout)) {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  } else {
    return false;
  }
}

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

bool IsColorFormatName(const std::string& name) {
  return !(colorformat_type_map.find(name) == colorformat_type_map.end());
}

InferenceEngine::ColorFormat GetColorFormatByName(const std::string& name) {
  return colorformat_type_map[name];
}

std::string GetNameOfColorFormat(
    const InferenceEngine::ColorFormat& colorformat) {
  return colorformat_name_map[colorformat];
}

bool IsResizeAlgorithmName(const std::string& name) {
  return !(resizeAlgorithm_type_map.find(name) ==
           resizeAlgorithm_type_map.end());
}

InferenceEngine::ResizeAlgorithm GetResizeAlgorithmByName(
    const std::string& name) {
  return resizeAlgorithm_type_map[name];
}

std::string GetNameOfResizeAlgorithm(
    const InferenceEngine::ResizeAlgorithm& resizeAlgorithm) {
  return resizeAlgorithm_name_map[resizeAlgorithm];
}

bool IsMeanVariantName(const std::string& name) {
  return !(meanvariant_type_map.find(name) == meanvariant_type_map.end());
}

InferenceEngine::MeanVariant GetMeanVariantByName(const std::string& name) {
  return meanvariant_type_map[name];
}

std::string GetNameOfMeanVariant(
    const InferenceEngine::MeanVariant& meanvariant) {
  return meanvariant_name_map[meanvariant];
}

}  // namespace utils
}  // namespace ienodejs