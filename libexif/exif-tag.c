/* exif-tag.c
 *
 * Copyright � 2001 Lutz M�ller <lutz@users.sourceforge.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details. 
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <config.h>

#include <libexif/exif-tag.h>
#include <libexif/i18n.h>

#include <stdlib.h>
#include <string.h>

#define ESL_NNNN { EXIF_SUPPORT_LEVEL_NOT_RECORDED, EXIF_SUPPORT_LEVEL_NOT_RECORDED, EXIF_SUPPORT_LEVEL_NOT_RECORDED, EXIF_SUPPORT_LEVEL_NOT_RECORDED }
#define ESL_OOOO { EXIF_SUPPORT_LEVEL_OPTIONAL, EXIF_SUPPORT_LEVEL_OPTIONAL, EXIF_SUPPORT_LEVEL_OPTIONAL, EXIF_SUPPORT_LEVEL_OPTIONAL }
#define ESL_MMMN { EXIF_SUPPORT_LEVEL_MANDATORY, EXIF_SUPPORT_LEVEL_MANDATORY, EXIF_SUPPORT_LEVEL_MANDATORY, EXIF_SUPPORT_LEVEL_NOT_RECORDED }
#define ESL_MMMM { EXIF_SUPPORT_LEVEL_MANDATORY, EXIF_SUPPORT_LEVEL_MANDATORY, EXIF_SUPPORT_LEVEL_MANDATORY, EXIF_SUPPORT_LEVEL_MANDATORY }
#define ESL_OMON { EXIF_SUPPORT_LEVEL_OPTIONAL, EXIF_SUPPORT_LEVEL_MANDATORY, EXIF_SUPPORT_LEVEL_OPTIONAL, EXIF_SUPPORT_LEVEL_NOT_RECORDED }
#define ESL_NNOO { EXIF_SUPPORT_LEVEL_NOT_RECORDED, EXIF_SUPPORT_LEVEL_NOT_RECORDED, EXIF_SUPPORT_LEVEL_OPTIONAL, EXIF_SUPPORT_LEVEL_OPTIONAL }
#define ESL_NNMN { EXIF_SUPPORT_LEVEL_NOT_RECORDED, EXIF_SUPPORT_LEVEL_NOT_RECORDED, EXIF_SUPPORT_LEVEL_MANDATORY, EXIF_SUPPORT_LEVEL_NOT_RECORDED }
#define ESL_NNMM { EXIF_SUPPORT_LEVEL_NOT_RECORDED, EXIF_SUPPORT_LEVEL_NOT_RECORDED, EXIF_SUPPORT_LEVEL_MANDATORY, EXIF_SUPPORT_LEVEL_MANDATORY }
#define ESL_NNNM { EXIF_SUPPORT_LEVEL_NOT_RECORDED, EXIF_SUPPORT_LEVEL_NOT_RECORDED, EXIF_SUPPORT_LEVEL_NOT_RECORDED, EXIF_SUPPORT_LEVEL_MANDATORY }
#define ESL_NNNO { EXIF_SUPPORT_LEVEL_NOT_RECORDED, EXIF_SUPPORT_LEVEL_NOT_RECORDED, EXIF_SUPPORT_LEVEL_NOT_RECORDED, EXIF_SUPPORT_LEVEL_OPTIONAL }
#define ESL_GPS { ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN }

static const struct {
	ExifTag tag;
	const char *name;
	const char *title;
	const char *description;
	ExifSupportLevel esl[EXIF_IFD_COUNT][4];
} ExifTagTable[] = {
#ifndef NO_VERBOSE_TAG_STRINGS
	{EXIF_TAG_NEW_SUBFILE_TYPE, "NewSubfileType",
	 "New Subfile Type", N_("A general indication of the kind of data "
	    "contained in this subfile.")},
	{EXIF_TAG_INTEROPERABILITY_INDEX, "InteroperabilityIndex",
	 "InteroperabilityIndex",
	 N_("Indicates the identification of the Interoperability rule. "
	    "Use \"R98\" for stating ExifR98 Rules. Four bytes used "
	    "including the termination code (NULL). see the separate "
	    "volume of Recommended Exif Interoperability Rules (ExifR98) "
	    "for other tags used for ExifR98."),
	 { ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_OOOO } },
	{EXIF_TAG_INTEROPERABILITY_VERSION, "InteroperabilityVersion",
	 "InteroperabilityVersion", "",
	 { ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_OOOO } },
	{EXIF_TAG_IMAGE_WIDTH, "ImageWidth", N_("Image Width"),
	 N_("The number of columns of image data, equal to the number of "
	    "pixels per row. In JPEG compressed data a JPEG marker is "
	    "used instead of this tag."),
	 { ESL_MMMN, ESL_MMMN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_IMAGE_LENGTH, "ImageLength", N_("Image Length"),
	 N_("The number of rows of image data. In JPEG compressed data a "
	    "JPEG marker is used instead of this tag."), 
	 { ESL_MMMN, ESL_MMMN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_BITS_PER_SAMPLE, "BitsPerSample", N_("Bits per Sample"),
	 N_("The number of bits per image component. In this standard each "
	    "component of the image is 8 bits, so the value for this "
	    "tag is 8. See also <SamplesPerPixel>. In JPEG compressed data "
	    "a JPEG marker is used instead of this tag."),
	 { ESL_MMMN, ESL_MMMN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_COMPRESSION, "Compression", N_("Compression"),
	 N_("The compression scheme used for the image data. When a "
	    "primary image is JPEG compressed, this designation is "
	    "not necessary and is omitted. When thumbnails use JPEG "
	    "compression, this tag value is set to 6."),
	 { ESL_MMMN, ESL_MMMM, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_PHOTOMETRIC_INTERPRETATION, "PhotometricInterpretation",
	 N_("Photometric Interpretation"),
	 N_("The pixel composition. In JPEG compressed data a JPEG "
	    "marker is used instead of this tag."),
	 { ESL_MMMN, ESL_MMMN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_FILL_ORDER, "FillOrder", N_("Fill Order"), ""},
	{EXIF_TAG_DOCUMENT_NAME, "DocumentName", N_("Document Name"), ""},
	{EXIF_TAG_IMAGE_DESCRIPTION, "ImageDescription",
	 N_("Image Description"),
	 N_("A character string giving the title of the image. It may be "
	    "a comment such as \"1988 company picnic\" or "
	    "the like. Two-bytes character codes cannot be used. "
	    "When a 2-bytes code is necessary, the Exif Private tag "
	    "<UserComment> is to be used."),
	 { ESL_OOOO, ESL_OOOO, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_MAKE, "Make", N_("Manufacturer"),
	 N_("The manufacturer of the recording "
	    "equipment. This is the manufacturer of the DSC, scanner, "
	    "video digitizer or other equipment that generated the "
	    "image. When the field is left blank, it is treated as "
	    "unknown."),
	 { ESL_OOOO, ESL_OOOO, ESL_NNNN, ESL_NNNN, ESL_NNNN }},
	{EXIF_TAG_MODEL, "Model", N_("Model"),
	 N_("The model name or model number of the equipment. This is the "
	    "model name or number of the DSC, scanner, video digitizer "
	    "or other equipment that generated the image. When the field "
	    "is left blank, it is treated as unknown."),
	 { ESL_OOOO, ESL_OOOO, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_STRIP_OFFSETS, "StripOffsets", N_("Strip Offsets"),
	 N_("For each strip, the byte offset of that strip. It is "
	    "recommended that this be selected so the number of strip "
	    "bytes does not exceed 64 Kbytes. With JPEG compressed "
	    "data this designation is not needed and is omitted. See also "
	    "<RowsPerStrip> and <StripByteCounts>."),
	 { ESL_MMMN, ESL_MMMN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_ORIENTATION, "Orientation", N_("Orientation"),
	 N_("The image orientation viewed in terms of rows and columns."),
	 { ESL_OOOO, ESL_OOOO, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_SAMPLES_PER_PIXEL, "SamplesPerPixel",
	 N_("Samples per Pixel"),
	 N_("The number of components per pixel. Since this standard applies "
	    "to RGB and YCbCr images, the value set for this tag is 3. "
	    "In JPEG compressed data a JPEG marker is used instead of this "
	    "tag."), 
	 { ESL_MMMN, ESL_MMMN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_ROWS_PER_STRIP, "RowsPerStrip", N_("Rows per Strip"),
	 N_("The number of rows per strip. This is the number of rows "
	    "in the image of one strip when an image is divided into "
	    "strips. With JPEG compressed data this designation is not "
	    "needed and is omitted. See also <StripOffsets> and "
	    "<StripByteCounts>."),
	 { ESL_MMMN, ESL_MMMN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_STRIP_BYTE_COUNTS, "StripByteCounts", N_("Strip Byte Count"),
	 N_("The total number of bytes in each strip. With JPEG compressed "
	    "data this designation is not needed and is omitted."),
	 { ESL_MMMN, ESL_MMMN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_X_RESOLUTION, "XResolution", N_("x-Resolution"),
	 N_("The number of pixels per <ResolutionUnit> in the <ImageWidth> "
	    "direction. When the image resolution is unknown, 72 [dpi] "
	    "is designated."),
	 { ESL_MMMM, ESL_MMMM, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_Y_RESOLUTION, "YResolution", N_("y-Resolution"),
	 N_("The number of pixels per <ResolutionUnit> in the <ImageLength> "
	    "direction. The same value as <XResolution> is designated."),
	 { ESL_MMMM, ESL_MMMM, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_PLANAR_CONFIGURATION, "PlanarConfiguration",
	 N_("Planar Configuration"),
	 N_("Indicates whether pixel components are recorded in a chunky "
	    "or planar format. In JPEG compressed files a JPEG marker "
	    "is used instead of this tag. If this field does not exist, "
	    "the TIFF default of 1 (chunky) is assumed."),
	 { ESL_OMON, ESL_OMON, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_RESOLUTION_UNIT, "ResolutionUnit", N_("Resolution Unit"),
	 N_("The unit for measuring <XResolution> and <YResolution>. The same "
	    "unit is used for both <XResolution> and <YResolution>. If "
	    "the image resolution is unknown, 2 (inches) is designated."),
	 { ESL_MMMM, ESL_MMMM, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_TRANSFER_FUNCTION, "TransferFunction",
	 N_("Transfer Function"),
	 N_("A transfer function for the image, described in tabular style. "
	    "Normally this tag is not necessary, since color space is "
	    "specified in the color space information tag (<ColorSpace>)."),
	 { ESL_OOOO, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_SOFTWARE, "Software", N_("Software"),
	 N_("This tag records the name and version of the software or "
	    "firmware of the camera or image input device used to "
	    "generate the image. The detailed format is not specified, but "
	    "it is recommended that the example shown below be "
	    "followed. When the field is left blank, it is treated as "
	    "unknown."),
	 { ESL_OOOO, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_DATE_TIME, "DateTime", N_("Date and Time"),
	 N_("The date and time of image creation. In this standard "
	    "(EXIF-2.1) it is the date and time the file was changed."),
	 { ESL_OOOO, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_ARTIST, "Artist", N_("Artist"),
	 N_("This tag records the name of the camera owner, photographer or "
	    "image creator. The detailed format is not specified, but it is "
	    "recommended that the information be written as in the example "
	    "below for ease of Interoperability. When the field is "
	    "left blank, it is treated as unknown."),
	 { ESL_OOOO, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_WHITE_POINT, "WhitePoint", N_("White Point"),
	 N_("The chromaticity of the white point of the image. Normally "
	    "this tag is not necessary, since color space is specified "
	    "in the colorspace information tag (<ColorSpace>)."),
	 { ESL_OOOO, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_PRIMARY_CHROMATICITIES, "PrimaryChromaticities",
	 N_("Primary Chromaticities"),
	 N_("The chromaticity of the three primary colors of the image. "
	    "Normally this tag is not necessary, since colorspace is "
	    "specified in the colorspace information tag (<ColorSpace>)."),
	 { ESL_OOOO, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_TRANSFER_RANGE, "TransferRange", N_("Transfer Range"), ""},
	{EXIF_TAG_SUB_IFDS, "SubIFDs", "SubIFD Offsets", N_("Defined by Adobe Corporation "
	    "to enable TIFF Trees within a TIFF file.")},
	{EXIF_TAG_JPEG_PROC, "JPEGProc", "JPEGProc", ""},
	{EXIF_TAG_JPEG_INTERCHANGE_FORMAT, "JPEGInterchangeFormat",
	 N_("JPEG Interchange Format"),
	 N_("The offset to the start byte (SOI) of JPEG compressed "
	    "thumbnail data. This is not used for primary image "
	    "JPEG data."),
	 { ESL_NNNN, ESL_NNNM, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_JPEG_INTERCHANGE_FORMAT_LENGTH,
	 "JPEGInterchangeFormatLength", N_("JPEG Interchange Format Length"),
	 N_("The number of bytes of JPEG compressed thumbnail data. This "
	    "is not used for primary image JPEG data. JPEG thumbnails "
	    "are not divided but are recorded as a continuous JPEG "
	    "bitstream from SOI to EOI. Appn and COM markers should "
	    "not be recorded. Compressed thumbnails must be recorded in no "
	    "more than 64 Kbytes, including all other data to be "
	    "recorded in APP1."),
	 { ESL_NNNN, ESL_NNNM, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_YCBCR_COEFFICIENTS, "YCbCrCoefficients",
	 N_("YCbCr Coefficients"),
	 N_("The matrix coefficients for transformation from RGB to YCbCr "
	    "image data. No default is given in TIFF; but here the "
	    "value given in Appendix E, \"Color Space Guidelines\", is used "
	    "as the default. The color space is declared in a "
	    "color space information tag, with the default being the value "
	    "that gives the optimal image characteristics "
	    "Interoperability this condition."),
	 { ESL_NNOO, ESL_NNOO, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_YCBCR_SUB_SAMPLING, "YCbCrSubSampling",
	 N_("YCbCr Sub-Sampling"),
	 N_("The sampling ratio of chrominance components in relation to the "
	    "luminance component. In JPEG compressed data a JPEG marker "
	    "is used instead of this tag."),
	 { ESL_NNMN, ESL_NNMN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_YCBCR_POSITIONING, "YCbCrPositioning",
	 N_("YCbCr Positioning"),
	 N_("The position of chrominance components in relation to the "
	    "luminance component. This field is designated only for "
	    "JPEG compressed data or uncompressed YCbCr data. The TIFF "
	    "default is 1 (centered); but when Y:Cb:Cr = 4:2:2 it is "
	    "recommended in this standard that 2 (co-sited) be used to "
	    "record data, in order to improve the image quality when viewed "
	    "on TV systems. When this field does not exist, the reader shall "
	    "assume the TIFF default. In the case of Y:Cb:Cr = 4:2:0, the "
	    "TIFF default (centered) is recommended. If the reader "
	    "does not have the capability of supporting both kinds of "
	    "<YCbCrPositioning>, it shall follow the TIFF default regardless "
	    "of the value in this field. It is preferable that readers "
	    "be able to support both centered and co-sited positioning."),
	 { ESL_NNMM, ESL_NNOO, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_REFERENCE_BLACK_WHITE, "ReferenceBlackWhite",
	 N_("Reference Black/White"),
	 N_("The reference black point value and reference white point "
	    "value. No defaults are given in TIFF, but the values "
	    "below are given as defaults here. The color space is declared "
	    "in a color space information tag, with the default "
	    "being the value that gives the optimal image characteristics "
	    "Interoperability these conditions."),
	 { ESL_OOOO, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_XML_PACKET, "XMLPacket", N_("XML Packet"), N_("XMP Metadata")},
	{EXIF_TAG_RELATED_IMAGE_FILE_FORMAT, "RelatedImageFileFormat",
	 "RelatedImageFileFormat", ""},
	{EXIF_TAG_RELATED_IMAGE_WIDTH, "RelatedImageWidth",
	 "RelatedImageWidth", ""},
	{EXIF_TAG_RELATED_IMAGE_LENGTH, "RelatedImageLength",
	 "RelatedImageLength", ""},
	{EXIF_TAG_CFA_REPEAT_PATTERN_DIM, "CFARepeatPatternDim",
	 "CFARepeatPatternDim", ""},
	{EXIF_TAG_CFA_PATTERN, "CFAPattern",
	 N_("CFA Pattern"),
	 N_("Indicates the color filter array (CFA) geometric pattern of the "
	    "image sensor when a one-chip color area sensor is used. "
	    "It does not apply to all sensing methods.")},
	{EXIF_TAG_BATTERY_LEVEL, "BatteryLevel", N_("Battery Level"), ""},
	{EXIF_TAG_COPYRIGHT, "Copyright", N_("Copyright"),
	 N_("Copyright information. In this standard the tag is used to "
	    "indicate both the photographer and editor copyrights. It is "
	    "the copyright notice of the person or organization claiming "
	    "rights to the image. The Interoperability copyright "
	    "statement including date and rights should be written in this "
	    "field; e.g., \"Copyright, John Smith, 19xx. All rights "
	    "reserved.\". In this standard the field records both the "
	    "photographer and editor copyrights, with each recorded in a "
	    "separate part of the statement. When there is a clear "
	    "distinction between the photographer and editor copyrights, "
	    "these are to be written in the order of photographer followed "
	    "by editor copyright, separated by NULL (in this case, "
	    "since the statement also ends with a NULL, there are two NULL "
	    "codes) (see example 1). When only the photographer is given, "
	    "it is terminated by one NULL code (see example 2). When only "
	    "the editor copyright is given, "
	    "the photographer copyright part consists of one space followed "
	    "by a terminating NULL code, then the editor copyright is given "
	    "(see example 3). When the field is left blank, it is treated "
	    "as unknown."),
	 { ESL_OOOO, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_EXPOSURE_TIME, "ExposureTime", N_("Exposure Time"),
	 N_("Exposure time, given in seconds (sec)."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_FNUMBER, "FNumber", N_("FNumber"),
	 N_("The F number."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_IPTC_NAA, "IPTC/NAA", "IPTC/NAA", ""},
	{EXIF_TAG_IMAGE_RESOURCES, "ImageResources", N_("Image Resources Block"), ""},
	{EXIF_TAG_EXIF_IFD_POINTER, "ExifIfdPointer", "ExifIFDPointer",
	 N_("A pointer to the Exif IFD. Interoperability, Exif IFD has the "
	    "same structure as that of the IFD specified in TIFF. "
	    "ordinarily, however, it does not contain image data as in "
	    "the case of TIFF."),
	 { ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_INTER_COLOR_PROFILE, "InterColorProfile",
	 "InterColorProfile", ""},
	{EXIF_TAG_EXPOSURE_PROGRAM, "ExposureProgram", "ExposureProgram",
	 N_("The class of the program used by the camera to set exposure "
	    "when the picture is taken."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_SPECTRAL_SENSITIVITY, "SpectralSensitivity",
	 N_("Spectral Sensitivity"),
	 N_("Indicates the spectral sensitivity of each channel of the "
	    "camera used. The tag value is an ASCII string compatible "
	    "with the standard developed by the ASTM Technical Committee."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_GPS_INFO_IFD_POINTER, "GPSInfoIFDPointer", 
	 "GPSInfoIFDPointer",
	 N_("A pointer to the GPS Info IFD. The "
	    "Interoperability structure of the GPS Info IFD, like that of "
	    "Exif IFD, has no image data."),
	 { ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_GPS_VERSION_ID, "GPSVersionID", N_("GPS tag version"),
	 N_("Indicates the version of <GPSInfoIFD>. The version is given "
	    "as 2.0.0.0. This tag is mandatory when <GPSInfo> tag is "
	    "present. (Note: The <GPSVersionID> tag is given in bytes, "
	    "unlike the <ExifVersion> tag. When the version is "
	    "2.0.0.0, the tag value is 02000000.H)."), ESL_GPS},
	{EXIF_TAG_GPS_LATITUDE_REF, "GPSLatitudeRef", N_("North or South Latitude"),
	 N_("Indicates whether the latitude is north or south latitude. The "
	    "ASCII value 'N' indicates north latitude, and 'S' is south "
	    "latitude."), ESL_GPS},
	{EXIF_TAG_GPS_LATITUDE, "GPSLatitude", N_("Latitude"),
	 N_("Indicates the latitude. The latitude is expressed as three "
	    "RATIONAL values giving the degrees, minutes, and seconds, "
	    "respectively. When degrees, minutes and seconds are expressed, "
	    "the format is dd/1,mm/1,ss/1. When degrees and minutes are used "
	    "and, for example, fractions of minutes are given up to two "
	    "decimal places, the format is dd/1,mmmm/100,0/1."),
	 ESL_GPS},
	{EXIF_TAG_GPS_LONGITUDE_REF, "GPSLongitudeRef", N_("East or West Longitude"),
	 N_("Indicates whether the longitude is east or west longitude. "
	    "ASCII 'E' indicates east longitude, and 'W' is west "
	    "longitude."), ESL_GPS},
	{EXIF_TAG_GPS_LONGITUDE, "GPSLongitude", N_("Longitude"),
	 N_("Indicates the longitude. The longitude is expressed as three "
	    "RATIONAL values giving the degrees, minutes, and seconds, "
	    "respectively. When degrees, minutes and seconds are expressed, "
	    "the format is ddd/1,mm/1,ss/1. When degrees and minutes are "
	    "used and, for example, fractions of minutes are given up to "
	    "two decimal places, the format is ddd/1,mmmm/100,0/1."),
	 ESL_GPS},
	{EXIF_TAG_GPS_ALTITUDE_REF, "GPSAltitudeRef", N_("Altitude reference"),
	 N_("Indicates the altitude used as the reference altitude. If the "
	    "reference is sea level and the altitude is above sea level, 0 "
			"is given. If the altitude is below sea level, a value of 1 is given "
			"and the altitude is indicated as an absolute value in the "
			"GSPAltitude tag. The reference unit is meters. Note that this tag "
			"is BYTE type, unlike other reference tags."), ESL_GPS},
	{EXIF_TAG_GPS_ALTITUDE, "GPSAltitude", N_("Altitude"),
	 N_("Indicates the altitude based on the reference in GPSAltitudeRef. "
			"Altitude is expressed as one RATIONAL value. The reference unit "
			"is meters."), ESL_GPS},
	{EXIF_TAG_ISO_SPEED_RATINGS, "ISOSpeedRatings",
	 N_("ISO Speed Ratings"),
	 N_("Indicates the ISO Speed and ISO Latitude of the camera or "
	    "input device as specified in ISO 12232."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_OECF, "OECF", "OECF",
	 N_("Indicates the Opto-Electronic Conversion Function (OECF) "
	    "specified in ISO 14524. <OECF> is the relationship between "
	    "the camera optical input and the image values."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_EXIF_VERSION, "ExifVersion", N_("Exif Version"),
	 N_("The version of this standard supported. Nonexistence of this "
	    "field is taken to mean nonconformance to the standard."),
	 { ESL_NNNN, ESL_NNNN, ESL_MMMM, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_DATE_TIME_ORIGINAL, "DateTimeOriginal",
	 N_("Date and Time (original)"),
	 N_("The date and time when the original image data was generated. "
	    "For a digital still camera "
	    "the date and time the picture was taken are recorded."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_DATE_TIME_DIGITIZED, "DateTimeDigitized",
	 N_("Date and Time (digitized)"),
	 N_("The date and time when the image was stored as digital data. "),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_COMPONENTS_CONFIGURATION, "ComponentsConfiguration",
	 "ComponentsConfiguration",
	 N_("Information specific to compressed data. The channels of "
	    "each component are arranged in order from the 1st "
	    "component to the 4th. For uncompressed data the data "
	    "arrangement is given in the <PhotometricInterpretation> tag. "
	    "However, since <PhotometricInterpretation> can only "
	    "express the order of Y, Cb and Cr, this tag is provided "
	    "for cases when compressed data uses components other than "
	    "Y, Cb, and Cr and to enable support of other sequences."),
	 { ESL_NNNN, ESL_NNNN, ESL_NNNM, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_COMPRESSED_BITS_PER_PIXEL, "CompressedBitsPerPixel",
	 N_("Compressed Bits per Pixel"),
	 N_("Information specific to compressed data. The compression mode "
	    "used for a compressed image is indicated in unit bits "
	    "per pixel."),
	 { ESL_NNNN, ESL_NNNN, ESL_NNNO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_SHUTTER_SPEED_VALUE, "ShutterSpeedValue", N_("Shutter speed"),
	 N_("Shutter speed. The unit is the APEX (Additive System of "
	    "Photographic Exposure) setting."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_APERTURE_VALUE, "ApertureValue", N_("Aperture"),
	 N_("The lens aperture. The unit is the APEX value."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_BRIGHTNESS_VALUE, "BrightnessValue", N_("Brightness"),
	 N_("The value of brightness. The unit is the APEX value. "
	    "Ordinarily it is given in the range of -99.99 to 99.99."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_EXPOSURE_BIAS_VALUE, "ExposureBiasValue",
	 N_("Exposure Bias"),
	 N_("The exposure bias. The units is the APEX value. Ordinarily "
	    "it is given in the range of -99.99 to 99.99."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_MAX_APERTURE_VALUE, "MaxApertureValue", "MaxApertureValue",
	 N_("The smallest F number of the lens. The unit is the APEX value. "
	    "Ordinarily it is given in the range of 00.00 to 99.99, "
	    "but it is not limited to this range."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_SUBJECT_DISTANCE, "SubjectDistance",
	 N_("Subject Distance"),
	 N_("The distance to the subject, given in meters."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_METERING_MODE, "MeteringMode", N_("Metering Mode"),
	 N_("The metering mode."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_LIGHT_SOURCE, "LightSource", N_("Light Source"),
	 N_("The kind of light source."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_FLASH, "Flash", N_("Flash"),
	 N_("This tag is recorded when an image is taken using a strobe "
	    "light (flash).")},
	{EXIF_TAG_FOCAL_LENGTH, "FocalLength", N_("Focal Length"),
	 N_("The actual focal length of the lens, in mm. Conversion is not "
	    "made to the focal length of a 35 mm film camera."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_MAKER_NOTE, "MakerNote", N_("Maker Note"),
	 N_("A tag for manufacturers of Exif writers to record any desired "
	    "information. The contents are up to the manufacturer."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_USER_COMMENT, "UserComment", N_("User Comment"),
	 N_("A tag for Exif users to write keywords or comments on the image "
	    "besides those in <ImageDescription>, and without the "
	    "character code limitations of the <ImageDescription> tag. The "
	    "character code used in the <UserComment> tag is identified "
	    "based on an ID code in a fixed 8-byte area at the start of "
	    "the tag data area. The unused portion of the area is padded "
	    "with NULL (\"00.h\"). ID codes are assigned by means of "
	    "registration. The designation method and references for each "
	    "character code are given in Table 6. The value of CountN "
	    "is determined based on the 8 bytes in the character code "
	    "area and the number of bytes in the user comment part. Since "
	    "the TYPE is not ASCII, NULL termination is not necessary "
	    "(see Fig. 9). "
	    "The ID code for the <UserComment> area may be a Defined code "
	    "such as JIS or ASCII, or may be Undefined. The Undefined name "
	    "is UndefinedText, and the ID code is filled with 8 bytes of all "
	    "\"NULL\" (\"00.H\"). An Exif reader that reads the "
	    "<UserComment> tag must have a function for determining the "
	    "ID code. This function is not required in Exif readers that "
	    "do not use the <UserComment> tag (see Table 7). "
	    "When a <UserComment> area is set aside, it is recommended that "
	    "the ID code be ASCII and that the following user comment "
	    "part be filled with blank characters [20.H]."),
			{ ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_SUB_SEC_TIME, "SubsecTime", "SubsecTime",
	 N_("A tag used to record fractions of seconds for the "
	    "<DateTime> tag."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_SUB_SEC_TIME_ORIGINAL, "SubSecTimeOriginal",
	 "SubSecTimeOriginal",
	 N_("A tag used to record fractions of seconds for the "
	    "<DateTimeOriginal> tag."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_SUB_SEC_TIME_DIGITIZED, "SubSecTimeDigitized",
	 "SubSecTimeDigitized",
	 N_("A tag used to record fractions of seconds for the "
	    "<DateTimeDigitized> tag."),
	 { ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_XP_TITLE, "XPTitle", N_("XP Title"), "",
	 { ESL_OOOO, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_XP_COMMENT, "XPComment", N_("XP Comment"), "",
	 { ESL_OOOO, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_XP_AUTHOR, "XPAuthor", N_("XP Author"), "",
	 { ESL_OOOO, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_XP_KEYWORDS, "XPKeywords", N_("XP Keywords"), "",
	 { ESL_OOOO, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_XP_SUBJECT, "XPSubject", N_("XP Subject"), "",
	 { ESL_OOOO, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_FLASH_PIX_VERSION, "FlashPixVersion", "FlashPixVersion",
	 N_("The FlashPix format version supported by a FPXR file."),
	 { ESL_NNNN, ESL_NNNN, ESL_MMMM, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_COLOR_SPACE, "ColorSpace", N_("Color Space"),
	 N_("The color space information tag is always "
	    "recorded as the color space specifier. Normally sRGB (=1) "
	    "is used to define the color space based on the PC monitor "
	    "conditions and environment. If a color space other than "
	    "sRGB is used, Uncalibrated (=FFFF.H) is set. Image data "
	    "recorded as Uncalibrated can be treated as sRGB when it is "
	    "converted to FlashPix."),
	 { ESL_NNNN, ESL_NNNN, ESL_MMMM, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_PIXEL_X_DIMENSION, "PixelXDimension", "PixelXDimension",
	 N_("Information specific to compressed data. When a "
	    "compressed file is recorded, the valid width of the "
	    "meaningful image must be recorded in this tag, whether or "
	    "not there is padding data or a restart marker. This tag "
	    "should not exist in an uncompressed file."),
	 { ESL_NNNN, ESL_NNNN, ESL_NNNM, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_PIXEL_Y_DIMENSION, "PixelYDimension", "PixelYDimension",
	 N_("Information specific to compressed data. When a compressed "
	    "file is recorded, the valid height of the meaningful image "
	    "must be recorded in this tag, whether or not there is padding "
	    "data or a restart marker. This tag should not exist in an "
	    "uncompressed file. For details see section 2.8.1 and Appendix "
	    "F. Since data padding is unnecessary in the vertical direction, "
	    "the number of lines recorded in this valid image height tag "
	    "will in fact be the same as that recorded in the SOF."),
	 { ESL_NNNN, ESL_NNNN, ESL_NNNM, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_RELATED_SOUND_FILE, "RelatedSoundFile",
	 "RelatedSoundFile",
	 N_("This tag is used to record the name of an audio file related "
	    "to the image data. The only relational information "
	    "recorded here is the Exif audio file name and extension (an "
	    "ASCII string consisting of 8 characters + '.' + 3 "
	    "characters). The path is not recorded. Stipulations on audio "
	    "are given in  section 3.6.3. File naming conventions are "
	    "given in section 3.7.1. "
	    "When using this tag, audio files must be recorded in "
	    "conformance to the Exif audio format. Writers are also allowed "
	    "to store the data such as Audio within APP2 as FlashPix "
	    "extension stream data. "
	    "Audio files must be recorded in conformance to the Exif audio "
	    "format. The mapping of Exif image files and audio files is done "
	    "in any of the three ways shown in Table 8. If multiple files "
	    "are mapped to one file as in [2] or [3] of this table, the above "
	    "format is used to record just one audio file name. If "
	    "there are multiple audio files, the first recorded file is "
	    "given. In the case of [3] in Table 8, for example, for the "
	    "Exif image file \"DSC00001.JPG\" only  \"SND00001.WAV\" is "
	    "given as the related Exif audio file. When there are three "
	    "Exif audio files \"SND00001.WAV\", \"SND00002.WAV\" and "
	    "\"SND00003.WAV\", the Exif image file name for each of them, "
	    "\"DSC00001.JPG\", is indicated. By combining multiple "
	    "relational information, a variety of playback possibilities "
	    "can be supported. The method of using relational information "
	    "is left to the implementation on the playback side. Since this "
	    "information is an ASCII character string, it is terminated by "
	    "NULL. When this tag is used to map audio files, the relation "
	    "of the audio file to image data must also be indicated on the "
	    "audio file end."),
			{ ESL_NNNN, ESL_NNNN, ESL_OOOO, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_INTEROPERABILITY_IFD_POINTER, "InteroperabilityIFDPointer",
	 "InteroperabilityIFDPointer",
	 N_("Interoperability IFD is composed of tags which stores the "
	    "information to ensure the Interoperability and pointed "
	    "by the following tag located in Exif IFD. "
	    "The Interoperability structure of Interoperability IFD is "
	    "the same as TIFF defined IFD structure "
	    "but does not contain the "
	    "image data characteristically compared with normal TIFF "
	    "IFD."),
	 { ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN, ESL_NNNN } },
	{EXIF_TAG_FLASH_ENERGY, "FlashEnergy", N_("Flash Energy"),
	 N_("Indicates the strobe energy at the time the image is "
	    "captured, as measured in Beam Candle Power Seconds (BCPS).")},
	{EXIF_TAG_SPATIAL_FREQUENCY_RESPONSE, "SpatialFrequencyResponse",
	 N_("Spatial Frequency Response"),
	 N_("This tag records the camera or input device spatial frequency "
	    "table and SFR values in the direction of image width, "
	    "image height, and diagonal direction, as specified in ISO "
	    "12233.")},
	{EXIF_TAG_FOCAL_PLANE_X_RESOLUTION, "FocalPlaneXResolution",
	 N_("Focal Plane x-Resolution"),
	 N_("Indicates the number of pixels in the image width (X) direction "
	    "per <FocalPlaneResolutionUnit> on the camera focal plane.")},
	{EXIF_TAG_FOCAL_PLANE_Y_RESOLUTION, "FocalPlaneYResolution",
	 N_("Focal Plane y-Resolution"),
	 N_("Indicates the number of pixels in the image height (V) direction "
	    "per <FocalPlaneResolutionUnit> on the camera focal plane.")},
	{EXIF_TAG_FOCAL_PLANE_RESOLUTION_UNIT, "FocalPlaneResolutionUnit",
	 N_("Focal Plane Resolution Unit"),
	 N_("Indicates the unit for measuring <FocalPlaneXResolution> and "
	    "<FocalPlaneYResolution>. This value is the same as the "
	    "<ResolutionUnit>.")},
	{EXIF_TAG_SUBJECT_LOCATION, "SubjectLocation",
	 N_("Subject Location"),
	 N_("Indicates the location of the main subject in the scene. The "
	    "value of this tag represents the pixel at the center of the "
	    "main subject relative to the left edge, prior to rotation "
	    "processing as per the <Rotation> tag. The first value "
	    "indicates the X column number and the second indicates "
	    "the Y row number.")},
	{EXIF_TAG_EXPOSURE_INDEX, "ExposureIndex", N_("Exposure index"),
	 N_("Indicates the exposure index selected on the camera or "
	    "input device at the time the image is captured.")},
	{EXIF_TAG_SENSING_METHOD, "SensingMethod", N_("Sensing Method"),
	 N_("Indicates the image sensor type on the camera or input "
	    "device.")},
	{EXIF_TAG_FILE_SOURCE, "FileSource", N_("File Source"),
	 N_("Indicates the image source. If a DSC recorded the image, "
	    "the tag value of this tag always be set to 3, indicating "
	    "that the image was recorded on a DSC.")},
	{EXIF_TAG_SCENE_TYPE, "SceneType", N_("Scene Type"),
	 N_("Indicates the type of scene. If a DSC recorded the image, "
	    "this tag value must always be set to 1, indicating that the "
	    "image was directly photographed.")},
	{EXIF_TAG_NEW_CFA_PATTERN, "CFAPattern",
	 N_("CFA Pattern"),
	 N_("Indicates the color filter array (CFA) geometric pattern of the "
	    "image sensor when a one-chip color area sensor is used. "
	    "It does not apply to all sensing methods.")},
	{EXIF_TAG_SUBJECT_AREA, "SubjectArea", N_("Subject Area"),
	 N_("This tag indicates the location and area of the main subject "
	    "in the overall scene.")},
	{EXIF_TAG_TIFF_EP_STANDARD_ID, "TIFF/EPStandardID", N_("TIFF/EP Standard ID"), ""},
	{EXIF_TAG_CUSTOM_RENDERED, "CustomRendered", N_("Custom Rendered"),
	 N_("This tag indicates the use of special processing on image "
	    "data, such as rendering geared to output. When special "
	    "processing is performed, the reader is expected to disable "
	    "or minimize any further processing.")},
	{EXIF_TAG_EXPOSURE_MODE, "ExposureMode", N_("Exposure Mode"),
	 N_("This tag indicates the exposure mode set when the image was "
	    "shot. In auto-bracketing mode, the camera shoots a series of "
	    "frames of the same scene at different exposure settings.")},
	{EXIF_TAG_WHITE_BALANCE, "WhiteBalance", N_("White Balance"),
	 N_("This tag indicates the white balance mode set when the image "
	    "was shot.")},
	{EXIF_TAG_DIGITAL_ZOOM_RATIO, "DigitalZoomRatio",
	 N_("Digital Zoom Ratio"),
	 N_("This tag indicates the digital zoom ratio when the image was "
	    "shot. If the numerator of the recorded value is 0, this "
	    "indicates that digital zoom was not used.")},
	{EXIF_TAG_FOCAL_LENGTH_IN_35MM_FILM, "FocalLengthIn35mmFilm",
	 N_("Focal Length In 35mm Film"),
	 N_("This tag indicates the equivalent focal length assuming a "
	    "35mm film camera, in mm. A value of 0 means the focal "
	    "length is unknown. Note that this tag differs from the "
	    "FocalLength tag.")},
	{EXIF_TAG_SCENE_CAPTURE_TYPE, "SceneCaptureType",
	 N_("Scene Capture Type"),
	 N_("This tag indicates the type of scene that was shot. It can "
	    "also be used to record the mode in which the image was "
	    "shot. Note that this differs from the scene type "
	    "<SceneType> tag.")},
	{EXIF_TAG_GAIN_CONTROL, "GainControl", N_("Gain Control"),
	 N_("This tag indicates the degree of overall image gain "
	    "adjustment.")},
	{EXIF_TAG_CONTRAST, "Contrast", N_("Contrast"),
	 N_("This tag indicates the direction of contrast processing "
	    "applied by the camera when the image was shot.")},
	{EXIF_TAG_SATURATION, "Saturation", N_("Saturation"),
	 N_("This tag indicates the direction of saturation processing "
	    "applied by the camera when the image was shot.")},
	{EXIF_TAG_SHARPNESS, "Sharpness", N_("Sharpness"),
	 N_("This tag indicates the direction of sharpness processing "
	    "applied by the camera when the image was shot.")},
	{EXIF_TAG_DEVICE_SETTING_DESCRIPTION, "DeviceSettingDescription",
	 N_("Device Setting Description"),
	 N_("This tag indicates information on the picture-taking "
	    "conditions of a particular camera model. The tag is used "
	    "only to indicate the picture-taking conditions in the "
	    "reader.")},
	{EXIF_TAG_SUBJECT_DISTANCE_RANGE, "SubjectDistanceRange",
	 N_("Subject Distance Range"),
	 N_("This tag indicates the distance to the subject.")},
	{EXIF_TAG_IMAGE_UNIQUE_ID, "ImageUniqueID", N_("Image Unique ID"),
	 N_("This tag indicates an identifier assigned uniquely to "
	    "each image. It is recorded as an ASCII string equivalent "
	    "to hexadecimal notation and 128-bit fixed length.")},
	{EXIF_TAG_GAMMA, "Gamma", N_("Gamma"),
	 N_("Indicates the value of coefficient gamma.")},
	{EXIF_TAG_PRINT_IMAGE_MATCHING, "PrintImageMatching", N_("PRINT Image Matching"),
	 N_("Related to Epson's PRINT Image Matching technology")},
#endif
	{0, NULL, NULL, NULL}
};

/* For now, do not use these functions. */
ExifTag      exif_tag_table_get_tag  (unsigned int n);
const char  *exif_tag_table_get_name (unsigned int n);
unsigned int exif_tag_table_count    (void);

ExifTag
exif_tag_table_get_tag (unsigned int n)
{
	return (n < exif_tag_table_count ()) ? ExifTagTable[n].tag : 0;
}

const char *
exif_tag_table_get_name (unsigned int n)
{
	return (n < exif_tag_table_count ()) ? ExifTagTable[n].name : NULL;
}

unsigned int
exif_tag_table_count (void)
{
	return sizeof (ExifTagTable) / sizeof (ExifTagTable[0]);
}

#define RECORDED \
((ExifTagTable[i].esl[ifd][EXIF_DATA_TYPE_UNCOMPRESSED_CHUNKY] != EXIF_SUPPORT_LEVEL_NOT_RECORDED) || \
 (ExifTagTable[i].esl[ifd][EXIF_DATA_TYPE_UNCOMPRESSED_PLANAR] != EXIF_SUPPORT_LEVEL_NOT_RECORDED) || \
 (ExifTagTable[i].esl[ifd][EXIF_DATA_TYPE_UNCOMPRESSED_YCC] != EXIF_SUPPORT_LEVEL_NOT_RECORDED) || \
 (ExifTagTable[i].esl[ifd][EXIF_DATA_TYPE_COMPRESSED] != EXIF_SUPPORT_LEVEL_NOT_RECORDED))

const char *
exif_tag_get_name_in_ifd (ExifTag tag, ExifIfd ifd)
{
	unsigned int i;

	if (ifd >= EXIF_IFD_COUNT) return NULL;
	for (i = 0; ExifTagTable[i].name; i++)
		if ((ExifTagTable[i].tag == tag) && RECORDED) break;
	return ExifTagTable[i].name;
}

const char *
exif_tag_get_title_in_ifd (ExifTag tag, ExifIfd ifd)
{
	unsigned int i;

	/* FIXME: This belongs to somewhere else. */
	/* libexif should use the default system locale.
	 * If an application specifically requires UTF-8, then we
	 * must give the application a way to tell libexif that.
	 * 
	 * bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	 */
	bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);

	if (ifd >= EXIF_IFD_COUNT) return NULL;
	for (i = 0; ExifTagTable[i].title; i++)
		if ((ExifTagTable[i].tag == tag) && RECORDED) break;
	return _(ExifTagTable[i].title);
}

const char *
exif_tag_get_description_in_ifd (ExifTag tag, ExifIfd ifd)
{
	unsigned int i;

	/* libexif should use the default system locale.
	 * If an application specifically requires UTF-8, then we
	 * must give the application a way to tell libexif that.
	 * 
	 * bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	 */
	bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);

	if (ifd >= EXIF_IFD_COUNT) return NULL;
	for (i = 0; ExifTagTable[i].description; i++)
		if ((ExifTagTable[i].tag == tag) && RECORDED) break;
	return _(ExifTagTable[i].description);
}


/**********************************************************************
 * convenience functions
 **********************************************************************/

/* generic part: iterate through IFD list and return first result */
typedef const char * (*get_stuff_func) (ExifTag tag, ExifIfd ifd);

static const char *
exif_tag_get_stuff (ExifTag tag, get_stuff_func func)
{
	static const ExifIfd ifds[5] = {
		EXIF_IFD_0,
		EXIF_IFD_1,
		EXIF_IFD_EXIF,
		EXIF_IFD_INTEROPERABILITY,
		EXIF_IFD_GPS
	};
	int i;
	for (i=0; i<5; i++) {
		const char *result = func(tag, ifds[i]);
		if (result != NULL) {
			return result;
		}
	}
	return (const char *) NULL;
}

/* explicit functions */
const char *
exif_tag_get_name (ExifTag tag)
{
	return exif_tag_get_stuff(tag, exif_tag_get_name_in_ifd);
}

const char *
exif_tag_get_title (ExifTag tag)
{
	return exif_tag_get_stuff(tag, exif_tag_get_title_in_ifd);
}

const char *
exif_tag_get_description (ExifTag tag)
{
	return exif_tag_get_stuff (tag, exif_tag_get_description_in_ifd);
}



ExifTag 
exif_tag_from_name (const char *name)
{
	unsigned int i;

	if (!name) return 0;

	for (i = 0; ExifTagTable[i].name; i++)
		if (!strcmp (ExifTagTable[i].name, name)) break;
	return ExifTagTable[i].tag;
}

ExifSupportLevel
exif_tag_get_support_level_in_ifd (ExifTag tag, ExifIfd ifd, ExifDataType t)
{
	unsigned int i;

	if (ifd >= EXIF_IFD_COUNT) return EXIF_SUPPORT_LEVEL_UNKNOWN;
	if (t >= EXIF_DATA_TYPE_COUNT) return EXIF_SUPPORT_LEVEL_UNKNOWN;

	for (i = 0; ExifTagTable[i].description; i++)
		if ((ExifTagTable[i].tag == tag) &&
				(ExifTagTable[i].esl[ifd][t] != EXIF_SUPPORT_LEVEL_NOT_RECORDED))
			return ExifTagTable[i].esl[ifd][t];
	return EXIF_SUPPORT_LEVEL_NOT_RECORDED;
}
