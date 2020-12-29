#ifndef IE_NODE_UTILS_H
#define IE_NODE_UTILS_H

#include <napi.h>
#include "inference_engine.hpp"

namespace ienodejs {

namespace utils {

bool checkTensorDesc(const Napi::Object& tensorDesc);

bool IsValidLayoutName(const std::string& name);
InferenceEngine::Layout GetLayoutByName(const std::string& name);
std::string GetNameOfLayout(const InferenceEngine::Layout& layout);

bool IsValidPrecisionName(const std::string& name);
InferenceEngine::Precision GetPrecisionByName(const std::string& name);
std::string GetNameOfPrecision(const InferenceEngine::Precision& precision);

bool IsColorFormatName(const std::string& name);
InferenceEngine::ColorFormat GetColorFormatByName(const std::string& name);
std::string GetNameOfColorFormat(
    const InferenceEngine::ColorFormat& colorformat);

bool IsResizeAlgorithmName(const std::string& name);
InferenceEngine::ResizeAlgorithm GetResizeAlgorithmByName(
    const std::string& name);
std::string GetNameOfResizeAlgorithm(
    const InferenceEngine::ResizeAlgorithm& resizeAlgorithm);

bool IsMeanVariantName(const std::string& name);
InferenceEngine::MeanVariant GetMeanVariantByName(const std::string& name);
std::string GetNameOfMeanVariant(
    const InferenceEngine::MeanVariant& meanvariant);
}  // namespace utils

}  // namespace ienodejs

#endif  // IE_NODE_UTILS_H