// This file is part of the AliceVision project.
// Copyright (c) 2016 AliceVision contributors.
// Copyright (c) 2012 openMVG contributors.
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#pragma once

#include <aliceVision/image/Image.hpp>
#include <aliceVision/image/pixelTypes.hpp>
#include <aliceVision/types.hpp>

#include <OpenImageIO/paramlist.h>
#include <OpenImageIO/imagebuf.h>

#include <string>

namespace oiio = OIIO;

namespace aliceVision {

class rgb;
template<typename Color> class LegacyImage;
using ImageRGBf = LegacyImage<image::RGBfColor>;
using ImageRGBAf = LegacyImage<image::RGBAfColor>;

namespace image {

/**
 * @brief Available image color space for pipeline input
 */
enum class EImageColorSpace
{
  AUTO,
  LINEAR,
  SRGB,
  ACES,
  ACEScg,
  LAB,
  XYZ,
  NO_CONVERSION
};

std::string EImageColorSpace_informations();
EImageColorSpace EImageColorSpace_stringToEnum(const std::string& dataType);
std::string EImageColorSpace_enumToString(const EImageColorSpace dataType);
std::string EImageColorSpace_enumToOIIOString(const EImageColorSpace colorSpace);
EImageColorSpace EImageColorSpace_OIIOstringToEnum(const std::string& colorspace);
std::ostream& operator<<(std::ostream& os, EImageColorSpace dataType);
std::istream& operator>>(std::istream& in, EImageColorSpace& dataType);

struct OutputFileColorSpace
{
    EImageColorSpace from{EImageColorSpace::LINEAR};
    EImageColorSpace to{EImageColorSpace::AUTO};

    OutputFileColorSpace(EImageColorSpace from_, EImageColorSpace to_)
        : from(from_)
        , to(to_)
    {
    }
    /// @brief Assumes that @p from is LINEAR
    explicit OutputFileColorSpace(EImageColorSpace to_)
    {
        if(to_ == EImageColorSpace::NO_CONVERSION)
            to = from;
        else
            to = to_;
    }
    OutputFileColorSpace() = default;
};

/**
 * @brief Available image file type for pipeline output
 */
enum class EImageFileType
{
  JPEG,
  PNG,
  TIFF,
  EXR,
  NONE
};

/**
 * @brief aggregate for multiple image reading options
 */
struct ImageReadOptions
{  
  ImageReadOptions(EImageColorSpace colorSpace = EImageColorSpace::AUTO, bool useWhiteBalance = true, const oiio::ROI & roi = oiio::ROI()) :
  outputColorSpace(colorSpace), applyWhiteBalance(useWhiteBalance), subROI(roi)
  {
  }

  EImageColorSpace outputColorSpace;
  bool applyWhiteBalance;

  //ROI for this image.
  //If the image contains an roi, this is the roi INSIDE the roi.
  oiio::ROI subROI;
};


/**
 * @brief get informations about each image file type
 * @return String
 */
std::string EImageFileType_informations();

/**
 * @brief It returns the EImageFileType enum from a string.
 * @param[in] imageFileType the input string.
 * @return the associated EImageFileType enum.
 */
EImageFileType EImageFileType_stringToEnum(const std::string& imageFileType);

/**
 * @brief It converts a EImageFileType enum to a string.
 * @param[in] imageFileType the EImageFileType enum to convert.
 * @return the string associated to the EImageFileType enum.
 */
std::string EImageFileType_enumToString(const EImageFileType imageFileType);

/**
 * @brief It write a EImageFileType enum into a stream by converting it to a string.
 * @param[in] os the stream where to write the imageType.
 * @param[in] imageFileType the EImageFileType enum to write.
 * @return the modified stream.
 */
std::ostream& operator<<(std::ostream& os, EImageFileType imageFileType);

/**
 * @brief It read a EImageFileType enum from a stream.
 * @param[in] in the stream from which the enum is read.
 * @param[out] imageFileType the EImageFileType enum read from the stream.
 * @return the modified stream without the read enum.
 */
std::istream& operator>>(std::istream& in, EImageFileType& imageFileType);

/**
 * @brief Return a list of extensions supported by openImageIO ie exists in extension_list from imageio.h
 * @return A vector containing all supported extensions
 */
std::vector<std::string> getSupportedExtensions();

/**
 * @brief Check if input image extension is supported by openImageIO ie exists in extension_list from imageio.h
 * @param[in] ext - image extension
 * @return true if valid extension
 */
bool isSupported(const std::string& ext);


/**
* @brief Data type use to write the image
*/
enum class EStorageDataType
{
    Float, //< Use full floating point precision to store
    Half, //< Use half (values our of range could become inf or nan)
    HalfFinite, //< Use half, but ensures out-of-range pixels are clamps to keep finite pixel values
    Auto //< Use half if all pixels can be stored in half without clamp, else use full float
};

std::string EStorageDataType_informations();
EStorageDataType EStorageDataType_stringToEnum(const std::string& dataType);
std::string EStorageDataType_enumToString(const EStorageDataType dataType);
std::ostream& operator<<(std::ostream& os, EStorageDataType dataType);
std::istream& operator>>(std::istream& in, EStorageDataType& dataType);

/**
 * @brief Available image qualities for pipeline output
 */
enum class EImageQuality
{
  OPTIMIZED,
  LOSSLESS
};

/**
 * @brief get informations about each image quality
 * @return String
 */
std::string EImageQuality_informations();

/**
 * @brief returns the EImageQuality enum from a string.
 * @param[in] imageQuality the input string.
 * @return the associated EImageQuality enum.
 */
EImageQuality EImageQuality_stringToEnum(const std::string& imageQuality);

/**
 * @brief converts an EImageQuality enum to a string.
 * @param[in] imageQuality the EImageQuality enum to convert.
 * @return the string associated to the EImageQuality enum.
 */
std::string EImageQuality_enumToString(const EImageQuality imageQuality);

/**
 * @brief write an EImageQuality enum into a stream by converting it to a string.
 * @param[in] os the stream where to write the imageType.
 * @param[in] imageQuality the EImageQuality enum to write.
 * @return the modified stream.
 */
std::ostream& operator<<(std::ostream& os, EImageQuality imageQuality);

/**
 * @brief read a EImageQuality enum from a stream.
 * @param[in] in the stream from which the enum is read.
 * @param[out] imageQuality the EImageQuality enum read from the stream.
 * @return the modified stream without the read enum.
 */
std::istream& operator>>(std::istream& in, EImageQuality& imageQuality);

/**
 * @brief Test if the extension is supported for undistorted images.
 * @param[in] ext The extension with the dot (eg ".png")
 * @return \p true if the extension is supported.
 */
bool isSupportedUndistortFormat(const std::string &ext);

/**
 * @brief convert a metadata string map into an oiio::ParamValueList
 * @param[in] metadataMap string map
 * @return oiio::ParamValueList
 */
oiio::ParamValueList getMetadataFromMap(const std::map<std::string, std::string>& metadataMap);

/**
 * @brief convert an oiio::ParamValueList into metadata string map
 * @param[in] metadata An instance of oiio::ParamValueList
 * @return std::map Metadata string map
 */
// Warning: type conversion problems from string to param value, we may lose some metadata with string maps
std::map<std::string, std::string> getMapFromMetadata(const oiio::ParamValueList& metadata);

/**
 * @brief extract metadata from an image for a given path
 * @param[in] path The given path to the image
 * @param[out] width The image header width
 * @param[out] height The image header height
 * @return metadata All metadata find in the image
 */
oiio::ParamValueList readImageMetadata(const std::string& path, int& width, int& height);

/**
 * @brief extract metadata from an image for a given path
 * @param[in] path The given path to the image
 * @return metadata All metadata find in the image
 */
oiio::ParamValueList readImageMetadata(const std::string& path);

/**
 * @brief extract entire image specification from an image for a given path
 * @param[in] path The given path to the image
 * @return imageSpec Specification describing the image
 */
oiio::ImageSpec readImageSpec(const std::string& path);

/**
 * @brief return the size of the image for a given path
 * @param path The given path to the image
 * @param[out] width The image header width
 * @param[out] height The image header height
 */
void readImageSize(const std::string& path, int& width, int& height);

/**
 * @brief get OIIO buffer from an AliceVision image
 * @param[in] image Image class
 * @param[out] buffer OIIO buffer
 */
void getBufferFromImage(Image<float>& image, oiio::ImageBuf& buffer);
void getBufferFromImage(Image<unsigned char>& image, oiio::ImageBuf& buffer);
void getBufferFromImage(Image<RGBAfColor>& image, oiio::ImageBuf& buffer);
void getBufferFromImage(Image<RGBAColor>& image, oiio::ImageBuf& buffer);
void getBufferFromImage(Image<RGBfColor>& image, oiio::ImageBuf& buffer);
void getBufferFromImage(Image<RGBColor>& image, oiio::ImageBuf& buffer);

/**
 * @brief read an image with a given path and buffer
 * @param[in] path The given path to the image
 * @param[out] image The output image buffer
 * @param[in] image color space
 */
void readImage(const std::string& path, Image<float>& image, const ImageReadOptions & imageReadOptions);
void readImage(const std::string& path, Image<unsigned char>& image, const ImageReadOptions & imageReadOptions);
void readImage(const std::string& path, Image<IndexT>& image, const ImageReadOptions & imageReadOptions);
void readImage(const std::string& path, Image<RGBAfColor>& image, const ImageReadOptions & imageReadOptions);
void readImage(const std::string& path, Image<RGBAColor>& image, const ImageReadOptions & imageReadOptions);
void readImage(const std::string& path, Image<RGBfColor>& image, const ImageReadOptions & imageReadOptions);
void readImage(const std::string& path, Image<RGBColor>& image, const ImageReadOptions & imageReadOptions);

/**
 * @brief read an image with a given path and buffer without any processing such as color conversion
 * @param[in] path The given path to the image
 * @param[out] image The output image buffer
 */
void readImageDirect(const std::string& path, Image<IndexT>& image);
void readImageDirect(const std::string& path, Image<unsigned char>& image);

/**
 * @brief write an image with a given path and buffer
 * @param[in] path The given path to the image
 * @param[in] image The output image buffer
 */
void writeImage(const std::string& path, const Image<float>& image, EImageColorSpace imageColorSpace,const oiio::ParamValueList& metadata = oiio::ParamValueList(),const oiio::ROI& roi = oiio::ROI());
void writeImage(const std::string& path, const Image<unsigned char>& image, EImageColorSpace imageColorSpace, const oiio::ParamValueList& metadata = oiio::ParamValueList());
void writeImage(const std::string& path, const Image<int>& image, EImageColorSpace imageColorSpace, const oiio::ParamValueList& metadata = oiio::ParamValueList());
void writeImage(const std::string& path, const Image<IndexT>& image, EImageColorSpace imageColorSpace, const oiio::ParamValueList& metadata = oiio::ParamValueList());
void writeImage(const std::string& path, const Image<RGBAfColor>& image, EImageColorSpace imageColorSpace,const oiio::ParamValueList& metadata = oiio::ParamValueList(),const oiio::ROI& roi = oiio::ROI());
void writeImage(const std::string& path, const Image<RGBAColor>& image, EImageColorSpace imageColorSpace, const oiio::ParamValueList& metadata = oiio::ParamValueList());
void writeImage(const std::string& path, const Image<RGBfColor>& image, EImageColorSpace imageColorSpace,const oiio::ParamValueList& metadata = oiio::ParamValueList(),const oiio::ROI& roi = oiio::ROI());
void writeImage(const std::string& path, const Image<RGBColor>& image, EImageColorSpace imageColorSpace, const oiio::ParamValueList& metadata = oiio::ParamValueList());

/**
 * @brief read an image with a given path and buffer
 * @param[in] path The given path to the image
 * @param[out] width The output image width
 * @param[out] height The output image height
 * @param[out] buffer The output image buffer
 * @param[in] image color space
 */
void readImage(const std::string& path, int& width, int& height, std::vector<unsigned char>& buffer,
               EImageColorSpace toColorSpace);
void readImage(const std::string& path, int& width, int& height, std::vector<unsigned short>& buffer,
               EImageColorSpace toColorSpace);
void readImage(const std::string& path, int& width, int& height, std::vector<rgb>& buffer,
               EImageColorSpace toColorSpace);
void readImage(const std::string& path, int& width, int& height, std::vector<float>& buffer,
               EImageColorSpace toColorSpace);
void readImage(const std::string& path, int& width, int& height, std::vector<RGBfColor>& buffer,
               EImageColorSpace toColorSpace);
void readImage(const std::string& path, int& width, int& height, std::vector<RGBAfColor>& buffer,
               EImageColorSpace toColorSpace);
void readImage(const std::string& path, ImageRGBf& image, EImageColorSpace toColorSpace);
void readImage(const std::string& path, ImageRGBAf& image, EImageColorSpace toColorSpace);

/**
 * @brief write an image with a given path and buffer
 * @param[in] path The given path to the image
 * @param[in] width The input image width
 * @param[in] height The input image height
 * @param[in] buffer The input image buffer
 */
void writeImage(const std::string& path, int width, int height, const std::vector<unsigned char>& buffer,
                EImageQuality imageQuality, const OutputFileColorSpace& colorspace,
                const oiio::ParamValueList& metadata = oiio::ParamValueList());
void writeImage(const std::string& path, int width, int height, const std::vector<unsigned short>& buffer,
                EImageQuality imageQuality, const OutputFileColorSpace& colorspace,
                const oiio::ParamValueList& metadata = oiio::ParamValueList());
void writeImage(const std::string& path, int width, int height, const std::vector<rgb>& buffer,
                EImageQuality imageQuality, const OutputFileColorSpace& colorspace,
                const oiio::ParamValueList& metadata = oiio::ParamValueList());
void writeImage(const std::string& path, int width, int height, const std::vector<float>& buffer,
                EImageQuality imageQuality, const OutputFileColorSpace& colorspace,
                const oiio::ParamValueList& metadata = oiio::ParamValueList());
void writeImage(const std::string& path, int width, int height, const std::vector<RGBfColor>& buffer,
                EImageQuality imageQuality, const OutputFileColorSpace& colorspace,
                const oiio::ParamValueList& metadata = oiio::ParamValueList());
void writeImage(const std::string& path, ImageRGBf& image, EImageQuality imageQuality,
                const OutputFileColorSpace& colorspace,
                const oiio::ParamValueList& metadata = oiio::ParamValueList());

template <typename T>
struct ColorTypeInfo
{
    // no size parameter, so no default value.
    // An error will be raise at compile time if this type traits is not defined.
};

template <>
struct ColorTypeInfo<unsigned char>
{
    static const int size = 1;
    static const oiio::TypeDesc::BASETYPE typeDesc = oiio::TypeDesc::UINT8;
};
template <>
struct ColorTypeInfo<float>
{
    static const int size = 1;
    static const oiio::TypeDesc::BASETYPE typeDesc = oiio::TypeDesc::FLOAT;
};
template <>
struct ColorTypeInfo<RGBColor>
{
    static const int size = 3;
    static const oiio::TypeDesc::BASETYPE typeDesc = oiio::TypeDesc::UINT8;
};
template <>
struct ColorTypeInfo<RGBfColor>
{
    static const int size = 3;
    static const oiio::TypeDesc::BASETYPE typeDesc = oiio::TypeDesc::FLOAT;
};
template <>
struct ColorTypeInfo<RGBAColor>
{
    static const int size = 4;
    static const oiio::TypeDesc::BASETYPE typeDesc = oiio::TypeDesc::UINT8;
};
template <>
struct ColorTypeInfo<RGBAfColor>
{
    static const int size = 4;
    static const oiio::TypeDesc::BASETYPE typeDesc = oiio::TypeDesc::FLOAT;
};

bool tryLoadMask(Image<unsigned char>* mask, const std::vector<std::string>& masksFolders,
                 const IndexT viewId, const std::string & srcImage);

/**
 * Returns the value of ALICEVISION_ROOT environmental variable, or empty string if it is not
 * defined. The returned value can be overridden by `setAliceVisionRootOverride` if needed, for
 * example in tests.
 */
// TODO: use std::optional when the C++ standard version is upgraded to C++17
std::string getAliceVisionRoot();

/// Returns path to OpenColorIO config that is shipped with aliceVision
std::string getAliceVisionOCIOConfig();

void setAliceVisionRootOverride(const std::string& value);

}  // namespace image
}  // namespace aliceVision
