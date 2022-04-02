#include "hm/imagedescription.h"


ImageDescription::ImageDescription(const std::string& pFileName) : mFile(pFileName)
{
}

const std::string&
ImageDescription::getFile() const
{
    return mFile;
}

bool
ImageDescription::operator<(const ImageDescription& pOther) const
{
    return mFile < pOther.mFile;
}

