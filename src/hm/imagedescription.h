#pragma once

#include <string>


class ImageDescription
{
public:
    explicit ImageDescription(const std::string& pFileName);

    const std::string&
    getFile() const;


    bool
    operator<(const ImageDescription& pOther) const;

private:
    const std::string mFile;
};

