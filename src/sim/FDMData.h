#ifndef __FDMDATA_H__
#define __FDMDATA_H__

#include <vector>
#include <map>
#include <string>
#include "FDMDataMapping.h"

class FDMData {
public:
    FDMData();

    bool DoubleKeyExists(const std::string& key) const;
    bool FloatKeyExists(const std::string& key) const;
    bool IntegerKeyExists(const std::string& key) const;

    double GetValue(const std::string& key) const;
    double GetDouble(const std::string& key) const;
    float GetFloat(const std::string& key) const;
    int GetInteger(const std::string& key) const;

    void * GetBinaryData(std::vector<std::string> keys) const;

    void SetDouble(const std::string& key, double value);
    void SetFloat(const std::string& key, float value);
    void SetInteger(const std::string& key, int value);
    
    // General-purpose value setters
    void SetValue(const std::string& key, double value);
    void SetValue(const std::string& key, float value);
    void SetValue(const std::string& key, int value);
    // END General-purpose value setters

    void RegisterMappingDouble(const std::string& FDMKey, const std::string& JFPKey);
    void RegisterMappingFloat(const std::string& FDMKey, const std::string& JFPKey);
    void RegisterMappingInteger(const std::string& FDMKey, const std::string& JFPKey);
    void RegisterDouble(const std::string& key);
    void RegisterFloat(const std::string& key);
    void RegisterInteger(const std::string& key);

    std::vector<std::string> GetAllJFPKeys() const;

private:
    std::map<std::string, double> _doubleValues;
    std::map<std::string, float> _floatValues;
    std::map<std::string, int> _integerValues;
    std::vector<FDMDataMapping> _dataMappings;

    // float phidot;
    // float thetadot;
    // float psidot;
    // float vcas;
    // float climb_rate;
    // float v_north;
    // float v_east;
    // float v_down;

    // float v_body_u;
    // float v_body_v;
    // float v_body_w;

    // float A_X_pilot;
    // float A_Y_pilot;
    // float A_Z_pilot;
};

#endif