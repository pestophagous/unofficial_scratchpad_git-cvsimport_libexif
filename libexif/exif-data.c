/* exif-data.c
 *
 * Copyright (C) 2001 Lutz M�ller <lutz@users.sourceforge.net>
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
#include "exif-data.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libjpeg/jpeg-marker.h>

#undef MAX
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

//#define DEBUG

static const unsigned char ExifHeader[] = {0x45, 0x78, 0x69, 0x66, 0x00, 0x00};

struct _ExifDataPrivate
{
	ExifByteOrder order;

	unsigned int ref_count;
};

ExifData *
exif_data_new (void)
{
	ExifData *data;

	data = malloc (sizeof (ExifData));
	if (!data)
		return (NULL);
	memset (data, 0, sizeof (ExifData));
	data->priv = malloc (sizeof (ExifDataPrivate));
	if (!data->priv) {
		free (data);
		return (NULL);
	}
	memset (data->priv, 0, sizeof (ExifDataPrivate));
	data->priv->ref_count = 1;

	data->ifd0                 = exif_content_new ();
	data->ifd1                 = exif_content_new ();
	data->ifd_exif             = exif_content_new ();
	data->ifd_gps              = exif_content_new ();
	data->ifd_interoperability = exif_content_new ();
	if (!data->ifd_exif || !data->ifd_gps || !data->ifd_interoperability ||
	    !data->ifd0 || !data->ifd1) {
		exif_data_free (data);
		return (NULL);
	}

	return (data);
}

ExifData *
exif_data_new_from_data (const unsigned char *data, unsigned int size)
{
	ExifData *edata;

	edata = exif_data_new ();
	exif_data_load_data (edata, data, size);
	return (edata);
}

static void
exif_data_load_data_entry (ExifData *data, ExifEntry *entry,
			   const unsigned char *d,
			   unsigned int size, unsigned int offset)
{
	unsigned int s, doff;

	entry->order = data->priv->order;
	entry->tag        = exif_get_short (d + offset + 0, data->priv->order);
	entry->format     = exif_get_short (d + offset + 2, data->priv->order);
	entry->components = exif_get_long  (d + offset + 4, data->priv->order);

	/*
	 * Size? If bigger than 4 bytes, the actual data is not
	 * in the entry but somewhere else (offset).
	 */
	s = exif_format_get_size (entry->format) * entry->components;
	if (!s)
		return;
	if (s > 4)
		doff = exif_get_long (d + offset + 8, data->priv->order);
	else
		doff = offset + 8;

	/* Sanity check */
	if (size < doff + s)
		return;

	entry->data = malloc (sizeof (char) * s);
	if (!entry->data)
		return;
	entry->size = s;
	memcpy (entry->data, d + doff, s);
}

static void
exif_data_save_data_entry (ExifData *data, ExifEntry *entry,
			   unsigned char **d, unsigned int *ds,
			   unsigned int offset)
{
	unsigned int doff, s;

	/*
	 * Each entry is 12 bytes long. The memory for the entry has
	 * already been allocated.
	 */
	exif_set_short (*d + 6 + offset + 0,
			data->priv->order, entry->tag);
	exif_set_short (*d + 6 + offset + 2,
			data->priv->order, entry->format);
	exif_set_long  (*d + 6 + offset + 4,
			data->priv->order, entry->components);

	/*
	 * Size? If bigger than 4 bytes, the actual data is not in
	 * the entry but somewhere else.
	 */
	s = exif_format_get_size (entry->format) * entry->components;
	if (!s)
		return;
	if (s > 4) {
		*ds += entry->size;
		*d = realloc (*d, sizeof (char) * *ds);
		doff = *ds - 6 - entry->size;
		exif_set_long (*d + 6 + offset + 8,
			       data->priv->order, doff);
	} else
		doff = offset + 8;
	memcpy (*d + 6 + doff, entry->data, entry->size);
}

static void
exif_data_load_data_thumbnail (ExifData *data, const unsigned char *d,
			       unsigned int ds, ExifLong offset, ExifLong size)
{
	if (ds < offset + size) {
#ifdef DEBUG
		printf ("Bogus thumbnail offset and size: %i < %i + %i.\n",
			(int) ds, (int) offset, (int) size);
#endif
		return;
	}
	if (data->data)
		free (data->data);
	data->size = size;
	data->data = malloc (sizeof (char) * data->size);
	memcpy (data->data, d + offset, data->size);
}

static void
exif_data_load_data_content (ExifData *data, ExifContent *ifd,
			     const unsigned char *d,
			     unsigned int ds, unsigned int offset)
{
	ExifLong o, thumbnail_offset = 0, thumbnail_length = 0;
	ExifShort n;
	ExifEntry *entry;
	unsigned int i;
	ExifTag tag;

	ifd->order = data->priv->order;

	/* Read the number of entries */
	n = exif_get_short (d + offset, data->priv->order);
#ifdef DEBUG
	printf ("Loading %i entries...\n", n);
#endif
	offset += 2;
	for (i = 0; i < n; i++) {

		tag = exif_get_short (d + offset + 12 * i, data->priv->order);
#ifdef DEBUG
		printf ("Loading entry '%s' (%i of %i)...\n",
			exif_tag_get_name (tag), i + 1, n);
#endif
		switch (tag) {
		case EXIF_TAG_EXIF_IFD_POINTER:
		case EXIF_TAG_GPS_INFO_IFD_POINTER:
		case EXIF_TAG_INTEROPERABILITY_IFD_POINTER:
		case EXIF_TAG_JPEG_INTERCHANGE_FORMAT_LENGTH:
		case EXIF_TAG_JPEG_INTERCHANGE_FORMAT:
			o = exif_get_long (d + offset + 12 * i + 8,
					   data->priv->order);
			switch (tag) {
			case EXIF_TAG_EXIF_IFD_POINTER:
				exif_data_load_data_content (data,
					data->ifd_exif, d, ds, o);
				break;
			case EXIF_TAG_GPS_INFO_IFD_POINTER:
				exif_data_load_data_content (data,
					data->ifd_gps, d, ds, o);
				break;
			case EXIF_TAG_INTEROPERABILITY_IFD_POINTER:
				exif_data_load_data_content (data,
					data->ifd_interoperability, d, ds, o);
				break;
			case EXIF_TAG_JPEG_INTERCHANGE_FORMAT:
#ifdef DEBUG
				printf ("Thumbnail at %i.\n", (int) o);
#endif
				thumbnail_offset = o;
				if (thumbnail_offset && thumbnail_length)
					exif_data_load_data_thumbnail (data, d,
						ds, thumbnail_offset,
						thumbnail_length);
				break;
			case EXIF_TAG_JPEG_INTERCHANGE_FORMAT_LENGTH:
#ifdef DEBUG
				printf ("Thumbnail size: %i.\n", (int) o);
#endif
				thumbnail_length = o;
				if (thumbnail_offset && thumbnail_length)
					exif_data_load_data_thumbnail (data, d,
						ds, thumbnail_offset,
						thumbnail_length);
				break;
			default:
				return;
			}
			break;
		default:
			entry = exif_entry_new ();
			exif_content_add_entry (ifd, entry);
			exif_data_load_data_entry (data, entry, d, ds,
						   offset + 12 * i);
			exif_entry_unref (entry);
			break;
		}
	}
}

static void
exif_data_save_data_content (ExifData *data, ExifContent *ifd,
			     unsigned char **d, unsigned int *ds,
			     unsigned int offset)
{
	unsigned int i, n_ptr = 0, n_thumb = 0;

	/* If we are to save IFD 0, we need some extra entries. */
	if (ifd == data->ifd0)
		n_ptr = 3;

	/*
	 * If we are to save IFD 1, we can point to the thumbnail if it
	 * exists.
	 */
	if ((ifd == data->ifd1) && data->data)
		n_thumb = 2;

	/*
	 * Allocate enough memory for all entries
	 * and the number of entries.
	 */
	*ds += (2 + (ifd->count + n_ptr + n_thumb) * 12 + (n_ptr ? 4 : 0));
	*d = realloc (*d, sizeof (char) * *ds);

	/* Save the number of entries */
	exif_set_short (*d + 6 + offset, data->priv->order,
			ifd->count + n_ptr + n_thumb);
	offset += 2;
#ifdef DEBUG
	printf ("Saving %i entries (offset: %i)...\n", ifd->count, offset);
#endif

	/* Save each entry */
	for (i = 0; i < ifd->count; i++)
		exif_data_save_data_entry (data, ifd->entries[i],
				d, ds, offset + 12 * i);
	offset += 12 * ifd->count;

	/* Save special entries */
	if (n_ptr) {

		/* EXIF_TAG_EXIF_IFD_POINTER */
		exif_set_short (*d + 6 + offset + 0, data->priv->order,
				EXIF_TAG_EXIF_IFD_POINTER);
		exif_set_short (*d + 6 + offset + 2, data->priv->order,
				EXIF_FORMAT_LONG);
		exif_set_long  (*d + 6 + offset + 4, data->priv->order, 1);
		exif_set_long  (*d + 6 + offset + 8, data->priv->order,
				*ds - 6);
		exif_data_save_data_content (data, data->ifd_exif, d, ds,
					     *ds - 6);
		offset += 12;

		/* EXIF_TAG_GPS_INFO_IFD_POINTER */
		exif_set_short (*d + 6 + offset + 0, data->priv->order,
				EXIF_TAG_GPS_INFO_IFD_POINTER);
		exif_set_short (*d + 6 + offset + 2, data->priv->order,
				EXIF_FORMAT_LONG);
		exif_set_long  (*d + 6 + offset + 4, data->priv->order, 1);
		exif_set_long  (*d + 6 + offset + 8, data->priv->order,
				*ds - 6);
		exif_data_save_data_content (data, data->ifd_gps, d, ds,
					     *ds - 6);
		offset += 12;

		/* EXIF_TAG_INTEROPERABILITY_IFD_POINTER */
		exif_set_short (*d + 6 + offset + 0, data->priv->order,
				EXIF_TAG_INTEROPERABILITY_IFD_POINTER);
		exif_set_short (*d + 6 + offset + 2, data->priv->order,
				EXIF_FORMAT_LONG);
		exif_set_long  (*d + 6 + offset + 4, data->priv->order, 1);
		exif_set_long  (*d + 6 + offset + 8, data->priv->order,
				*ds - 6);
		exif_data_save_data_content (data, data->ifd_interoperability,
					     d, ds, *ds - 6);
		offset += 12;

		/*
		 * We are saving IFD 0. Tell where IFD 1 starts and save
		 * IFD 1.
		 */
		exif_set_long (*d + 6 + offset, data->priv->order, *ds - 6);
		exif_data_save_data_content (data, data->ifd1, d, ds,
					     *ds - 6);
	}

	if (n_thumb) {

		/* EXIF_TAG_JPEG_INTERCHANGE_FORMAT_LENGTH */
		exif_set_short (*d + 6 + offset + 0, data->priv->order,
				EXIF_TAG_JPEG_INTERCHANGE_FORMAT_LENGTH);
		exif_set_short (*d + 6 + offset + 2, data->priv->order,
				EXIF_FORMAT_LONG);
		exif_set_long  (*d + 6 + offset + 4, data->priv->order, 1);
		exif_set_long  (*d + 6 + offset + 8, data->priv->order,
				data->size);
		offset += 12;
		
		/* EXIF_TAG_JPEG_INTERCHANGE_FORMAT */
		exif_set_short (*d + 6 + offset + 0, data->priv->order,
				EXIF_TAG_JPEG_INTERCHANGE_FORMAT);
		exif_set_short (*d + 6 + offset + 2, data->priv->order,
				EXIF_FORMAT_LONG);
		exif_set_long  (*d + 6 + offset + 4, data->priv->order, 1);
		exif_set_long  (*d + 6 + offset + 8, data->priv->order,
				*ds - 6);
		*ds += data->size;
		*d = realloc (*d, sizeof (char) * *ds);
		memcpy (*d + *ds - data->size, data->data, data->size);
	}
}

void
exif_data_load_data (ExifData *data, const unsigned char *d, unsigned int size)
{
	unsigned int l, len = size;
	ExifLong offset;
	ExifShort n;

	if (!data)
		return;
	if (!d || !size)
		return;

#ifdef DEBUG
	printf ("Parsing %i byte(s) EXIF data...\n", size);
#endif

	/*
	 * It can be that the data starts with the EXIF header. If it does
	 * not, search the EXIF marker.
	 */
	if (size < 6) {
#ifdef DEBUG
		printf ("Size too small.\n");
		return;
#endif
	}
	if (!memcmp (d, ExifHeader, 6)) {
#ifdef DEBUG
		printf ("Found EXIF header.\n");
#endif
	} else {
#ifdef DEBUG
		printf ("Data begins with 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x "
			"0x%x...\n", d[0], d[1], d[2], d[3], d[4], d[5], d[6]);
#endif
		while (1) {
			while ((d[0] == 0xff) && size) {
				d++;
				size--;
			}

			/* JPEG_MARKER_SOI */
			if (d[0] == JPEG_MARKER_SOI) {
				d++;
				size--;
				continue;
			}

			/* JPEG_MARKER_APP0 */
			if (d[0] == JPEG_MARKER_APP0) {
				d++;
				size--;
				l = (d[0] << 8) | d[1];
				if (l > size)
					return;
				d += l;
				size -= l;
				continue;
			}

			/* JPEG_MARKER_APP1 */
			if (d[0] == JPEG_MARKER_APP1)
				break;

			/* Unknown marker or data. Give up. */
			printf ("EXIF marker not found.\n");
			return;
		}
		d++;
		size--;
		if (size < 2) {
#ifdef DEBUG
			printf ("Size too small.\n");
#endif
			return;
		}
		len = (d[0] << 8) | d[1];
#ifdef DEBUG
		printf ("We have to deal with %i byte(s) of EXIF data.\n", len);
#endif
		d += 2;
		size -= 2;
	}

	/*
	 * Verify the exif header
	 * (offset 2, length 6).
	 */
	if (size < 6) {
#ifdef DEBUG
		printf ("Size too small.\n");
		return;
#endif
	}
	if (memcmp (d, ExifHeader, 6)) {
#ifdef DEBUG
		printf ("EXIF header not found.\n");
		return;
#endif
	}

#ifdef DEBUG
	printf ("Found EXIF header.\n");
#endif

	/* Byte order (offset 6, length 2) */
	if (size < 12)
		return;
	if (!memcmp (d + 6, "II", 2))
		data->priv->order = EXIF_BYTE_ORDER_INTEL;
	else if (!memcmp (d + 6, "MM", 2))
		data->priv->order = EXIF_BYTE_ORDER_MOTOROLA;
	else
		return;

	/* Fixed value */
	if (exif_get_short (d + 8, data->priv->order) != 0x002a)
		return;

	/* IFD 0 offset */
	offset = exif_get_long (d + 10, data->priv->order);
#ifdef DEBUG
	printf ("IFD 0 at %i.\n", (int) offset);
#endif

	/* Parse the actual exif data (offset 14) */
	exif_data_load_data_content (data, data->ifd0, d + 6,
				     size - 6, offset);

	/* IFD 1 offset */
	n = exif_get_short (d + 6 + offset, data->priv->order);
	offset = exif_get_long (d + 6 + offset + 2 + 12 * n, data->priv->order);
	if (offset) {
#ifdef DEBUG
		printf ("IFD 1 at %i.\n", (int) offset);
#endif
		exif_data_load_data_content (data, data->ifd1, d + 6,
					     size - 6, offset);
	}
}

void
exif_data_save_data (ExifData *data, unsigned char **d, unsigned int *ds)
{
	if (!data)
		return;
	if (!d || !ds)
		return;

	/* Header */
	*ds = 6;
	*d = malloc (sizeof (char) * *ds);
	memcpy (*d, ExifHeader, 6);

	/* Order (offset 6) */
	*ds += 2;
	*d = realloc (*d, sizeof (char) * *ds);
	if (data->priv->order == EXIF_BYTE_ORDER_INTEL) {
		memcpy (*d + 6, "II", 2);
	} else {
		memcpy (*d + 6, "MM", 2);
	}

	/* Fixed value (2 bytes, offset 8) */
	*ds += 2;
	*d = realloc (*d, sizeof (char) * *ds);
	exif_set_short (*d + 8, data->priv->order, 0x002a);

	/*
	 * IFD 0 offset (4 bytes, offset 10).
	 * We will start 8 bytes after the
	 * EXIF header (2 bytes for order, another 2 for the test, and 
	 * 4 bytes for the IFD 0 offset make 8 bytes together).
	 */
	*ds += 4;
	*d = realloc (*d, sizeof (char) * *ds);
	exif_set_long (*d + 10, data->priv->order, 8);

	/* Now save IFD 0. IFD 1 will be saved automatically. */
#ifdef DEBUG
	printf ("Saving IFDs...\n");
#endif
	exif_data_save_data_content (data, data->ifd0, d, ds, *ds - 6);

#ifdef DEBUG
	printf ("Saved %i byte(s) EXIF data.\n", *ds);
#endif
}

ExifData *
exif_data_new_from_file (const char *path)
{
	FILE *f;
	unsigned int size;
	unsigned char *data;
	ExifData *edata;

	f = fopen (path, "r");
	if (!f)
		return (NULL);

	/* For now, we read the data into memory. Patches welcome... */
	fseek (f, 0, SEEK_END);
	size = ftell (f);
	fseek (f, 0, SEEK_SET);
	data = malloc (sizeof (char) * size);
	if (!data)
		return (NULL);
	if (fread (data, 1, size, f) != size) {
		free (data);
		return (NULL);
	}

	edata = exif_data_new_from_data (data, size);
	free (data);

	fclose (f);

	return (edata);
}

void
exif_data_ref (ExifData *data)
{
	if (!data)
		return;

	data->priv->ref_count++;
}

void
exif_data_unref (ExifData *data)
{
	if (!data)
		return;

	data->priv->ref_count--;
	if (!data->priv->ref_count)
		exif_data_free (data);
}

void
exif_data_free (ExifData *data)
{
	if (data->ifd0)
		exif_content_unref (data->ifd0);
	if (data->ifd1)
		exif_content_unref (data->ifd1);
	if (data->data)
		free (data->data);
	free (data->priv);
	free (data);
}

void
exif_data_dump (ExifData *data)
{
	if (!data)
		return;

	if (data->ifd0->count) {
		printf ("Dumping IFD 0...\n");
		exif_content_dump (data->ifd0, 0);
	}

	if (data->ifd1->count) {
		printf ("Dumping IFD 1...\n");
		exif_content_dump (data->ifd1, 0);
	}

	if (data->ifd_exif->count) {
		printf ("Dumping IFD EXIF...\n");
		exif_content_dump (data->ifd_exif, 0);
	}

	if (data->ifd_gps->count) {
		printf ("Dumping IFD GPS...\n");
		exif_content_dump (data->ifd_gps, 0);
	}

	if (data->ifd_interoperability->count) {
		printf ("Dumping IFD Interoperability...\n");
		exif_content_dump (data->ifd_interoperability, 0);
	}

	if (data->data) {
		printf ("%i byte(s) thumbnail data available.", data->size);
		if (data->size >= 4) {
			printf ("0x%02x 0x%02x ... 0x%02x 0x%02x\n",
				data->data[0], data->data[1],
				data->data[data->size - 2],
				data->data[data->size - 1]);
		}
	}
}
