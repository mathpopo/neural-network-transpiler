#ifndef nnt_CCP_GEN_H
#define nnt_CCP_GEN_H

#include <string>
#include <vector>
#include <tuple>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include "model.h"

namespace nnt {

class TensorsHeader {
 public:
  TensorsHeader(Model& model): model_(model) {}

  std::string Assembler();

 private:
  std::string Generate();

  Model& model_;
};

class ModelGen {
 public:
  ModelGen(Model& model): model_(model), tensor_pos_(0) {}

  std::string Assembler();

 private:
  std::string Generate();
  std::string GenerateTensorType(const Tensor& tensor, int count);
  std::string GenerateTensorsCode();
  std::string TensorTypeStr(TensorType type);
  std::string TensorCppTypeStr(TensorType type);
  std::string TensorDim(const std::vector<int>& dim);
  float TensorQuantizationScale(const QuantizationParameters& q);
  int TensorQuantizationZeroPoint(const QuantizationParameters& q);
  std::string CheckStatus(const boost::format& msg);

  std::string GenerateOpCode();
  std::string GenerateOpInputs(const std::vector<int>& inputs,
      size_t num_params);
  std::string GenerateOpOutputs(const std::vector<int>& outputs);
  std::string OpTypeStr(BuiltinOperator op_type);
  std::tuple<size_t, std::string> OpParams(const Operator& op);
  std::string GenerateInputsAndOutputs();
  std::string GenerateInputFunctions();
  std::string GenerateOutputFunctions();
  std::string GenerateHeader();
  std::string AddScalarInt32(int value);
  std::string AddScalarFloat32(float value);
  int TensorSize(const Tensor& tensor);

  Model& model_;
  size_t tensor_pos_;
  int count_operands_;
};

class ModelGenHeader {
 public:
  ModelGenHeader(Model& model): model_(model) {}

  std::string Assembler();
 private:
  std::string GenerateHeader();
  Model& model_;
};

class ModelGenJni {
 public:
  ModelGenJni(Model& model, const std::string& java_package)
      : model_(model)
      , java_package_(java_package) {}

  std::string Assembler();
 private:
  std::string GenerateJni();

  template<class Fn>
  int TotalSize(Fn&& fn);

  Model& model_;
  std::string java_package_;
};

class CppGen {
 public:
  CppGen(Model& model): model_(model) {}

  void GenFiles(const boost::filesystem::path& path,
      const std::string& java_path);

 private:
  void GenTensorsDataFile(const boost::filesystem::path& path);
  void GenCppFile(const boost::filesystem::path& path);
  void GenHFile(const boost::filesystem::path& path);
  void GenJniFile(const boost::filesystem::path& path,
      const std::string& java_package);

  Model& model_;
};

}

#endif  // nnt_CCP_GEN_H
