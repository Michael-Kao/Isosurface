#include "VolumeData.h"

std::unordered_set<std::string> const kAlias::Raw({"raw-file", "Rawfile"});
std::unordered_set<std::string> const kAlias::Resolution({"resolution", "Resolution"});
std::unordered_set<std::string> const kAlias::VolumeSize({"voxel-size", "VoxelSize", "ratio", "Ratio"});
std::unordered_set<std::string> const kAlias::vDataType({"sample-type", "SampleType"});
std::unordered_set<std::string> const kAlias::Endian({"endian", "Endian"});
std::string const InfoKey::Inf = "inf-file";
std::string const InfoKey::Raw = "raw-file";
std::string const InfoKey::Resolution = "resolution";
std::string const InfoKey::VolumeSize = "voxel-size";
std::string const InfoKey::vDataType = "sample-type";
std::string const InfoKey::Endian = "endian";

struct kAlias static const key_alias;
struct InfoKey static const key;

VolumeData::VolumeData(std::string const &filePath, std::string const &infFile) {
    info[key.Inf] = infFile;
    info["path"] = filePath;

    //read info file
    std::ifstream  infIn(info["path"] + info[key.Inf]);
    if(!infIn) {
        std::cerr << "fail in opening " << info[key.Inf] << " for reading.\n";
        exit(1);
    }

    std::string line;
    while(std::getline(infIn, line)) {
        std::vector<std::string> words = strUtil::split(line, "=");
        // // std::vector<std::string> delimiters({":", "x"});
        
        if (key_alias.Raw.count(words[0])) {
            info[key.Raw] = words[1];
        } else if (key_alias.Resolution.count(words[0])) {
            info[key.Resolution] = words[1];
        } else if (key_alias.vDataType.count(words[0])) {
            info[key.vDataType] = strUtil::remove(words[1], " ", LOWER);
        } else if (key_alias.VolumeSize.count(words[0])) {
            info[key.VolumeSize] = words[1];
        } else if (key_alias.Endian.count(words[0])) {
            info[key.Endian] = strUtil::remove(words[1], "", LOWER);
        }
    }

    if (!info.count(key.Resolution)) {
        std::cerr << "[ERROR] .inf file: Missing attribute \"sample-type\"\n";
        exit(1);
    }
    if (!info.count(key.vDataType)) {
        std::cerr << "[ERROR] .inf file: Missing attribute \"sample-type\"\n";
        exit(1);
    }

    // assign default
    if (!info.count(key.VolumeSize)) {
        info[key.VolumeSize] = "1:1:1";
    }
    if (!info.count(key.Endian)) {
        info[key.Endian] = "little";
    }
    if (!info.count(key.Raw)) {
        auto fileInfo = strUtil::split(infFile, ".");
        info[key.Raw] = fileInfo[0] + ".raw";
    }
}