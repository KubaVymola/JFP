#include "FDMData.h"

FDMData::FDMData() {
    // TODO Add default native properties
    // TODO Create default bindingsff
}

bool FDMData::DoubleKeyExists(const std::string& key) const {
    return _doubleValues.count(key);
}

bool FDMData::FloatKeyExists(const std::string& key) const {
    return _floatValues.count(key);
}

bool FDMData::IntegerKeyExists(const std::string& key) const {
    return _integerValues.count(key);
}

double FDMData::GetValue(const std::string& key) const {
    if (DoubleKeyExists(key)) return GetDouble(key);
    if (FloatKeyExists(key)) return GetFloat(key);
    if (IntegerKeyExists(key)) return GetInteger(key);

    return 0;
}

double FDMData::GetDouble(const std::string& key) const {
    if (!DoubleKeyExists(key)) return 0;

    return _doubleValues.find(key)->second;
}

float FDMData::GetFloat(const std::string& key) const {
    if (!FloatKeyExists(key)) return 0;

    return _floatValues.find(key)->second;
}

int FDMData::GetInteger(const std::string& key) const {
    if (!IntegerKeyExists(key)) return 0;

    return _integerValues.find(key)->second;
}

void * FDMData::GetBinaryData(std::vector<std::string> keys) const {
    // TODO

    return 0;
}

void FDMData::SetDouble(const std::string& key, double value) {
    if (!DoubleKeyExists(key)) return;

    _floatValues[key] = value;
}

void FDMData::SetFloat(const std::string& key, float value) {
    if (!FloatKeyExists(key)) return;

    _floatValues[key] = value;
}

void FDMData::SetInteger(const std::string& key, int value) {
    if (!IntegerKeyExists(key)) return;

    _integerValues[key] = value;
}

void FDMData::SetValue(const std::string& key, double value) {
    if (DoubleKeyExists(key)) _doubleValues[key] = value;
    else if (FloatKeyExists(key)) _floatValues[key] = value;
    else if (IntegerKeyExists(key)) _integerValues[key] = value;
}

void FDMData::SetValue(const std::string& key, float value) {
    if (FloatKeyExists(key)) _floatValues[key] = value;
    else if (DoubleKeyExists(key)) _doubleValues[key] = value;
    else if (IntegerKeyExists(key)) _integerValues[key] = value;
}

void FDMData::SetValue(const std::string& key, int value) {
    if (IntegerKeyExists(key)) _integerValues[key] = value;
    else if (DoubleKeyExists(key)) _doubleValues[key] = value;
    else if (FloatKeyExists(key)) _floatValues[key] = value;
}

void FDMData::RegisterMappingDouble(const std::string& FDMKey, const std::string& JFPKey) {
    // TODO
}

void FDMData::RegisterMappingFloat(const std::string& FDMKey, const std::string& JFPKey) {
    // TODO
}

void FDMData::RegisterMappingInteger(const std::string& FDMKey, const std::string& JFPKey) {
    // TODO
}

void FDMData::RegisterDouble(const std::string& key) {
    _doubleValues.insert({ key, 0.0 });
}

void FDMData::RegisterFloat(const std::string& key) {
    _floatValues.insert({ key, 0.0f });
}

void FDMData::RegisterInteger(const std::string& key) {
    _integerValues.insert({ key, 0 });
}

std::vector<std::string> FDMData::GetAllJFPKeys() const {
    std::vector<std::string> toReturn;

    for (auto& doubleValue : _doubleValues)   toReturn.push_back(doubleValue.first);
    for (auto& floatValue : _floatValues)     toReturn.push_back(floatValue.first);
    for (auto& integerValue : _integerValues) toReturn.push_back(integerValue.first);

    return toReturn;
}
