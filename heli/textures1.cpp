#include "textures.h"

#include <stdlib.h>
#include <windows.h>

#include <stdio.h>
#include <glut.h>

int LoadFileBMP(char *filename, unsigned char **pixels, int *width, int *height, bool flipvert)
{
	FILE*		file;			// file stream
	BITMAPFILEHEADER	*bmfh;			// bitmap file header
	BITMAPINFOHEADER	*bmih;			// bitmap info header (windows)
	BITMAPCOREHEADER	*bmch;			// bitmap core header (os/2)
	RGBTRIPLE			*os2_palette;	// pointer to the color palette os/2
	RGBQUAD				*win_palette;	// pointer to the color palette windows
	char				*buffer;		// buffer storing the entire file
	unsigned char		*ptr;			// pointer to pixels data
	int					bitCount;		// number of bits per pixel
	int					compression;	// compression type (rgb/rle)
	int					row, col, i;	// temporary variables
	int					w, h;			// width, height
	/////////////////////////////////////////////////////
	// read the entire file in the buffer
	file = fopen( filename, "rb" );
	if( file == NULL )
		return 0;
	fseek(file, 0, SEEK_END);
	long flen = ftell(file);
	fseek(file, 0, SEEK_SET);
	buffer = new char[ flen + 1 ];
	fread(buffer, flen, 1, file);
	char *pBuff = buffer;
	fclose(file);
	/////////////////////////////////////////////////////
	// read the header
	bmfh = (BITMAPFILEHEADER *)pBuff;
	pBuff += sizeof( BITMAPFILEHEADER );
	// verify that it's a BITMAP file
	printf("%c", bmfh->bfType);
	if( bmfh->bfType != BITMAP_ID )
	{
		delete [] buffer;
		return 0;
	}
	bmch = (BITMAPCOREHEADER *)pBuff;
	bmih = (BITMAPINFOHEADER *)pBuff;
	if( (bmih->biCompression < 0) || (bmih->biCompression > 3) )
	{
		// OS/2 style
		pBuff += sizeof( BITMAPCOREHEADER );
		bitCount	= bmch->bcBitCount;
		compression	= BI_OS2;
		w = bmch->bcWidth;
		h = bmch->bcHeight;
	}
	else
	{
		// WINDOWS style
		pBuff += sizeof( BITMAPINFOHEADER );
		bitCount	= bmih->biBitCount;
		compression	= bmih->biCompression;
		w = bmih->biWidth;
		h = bmih->biHeight;
	}
	if( width )
		*width	= w;
	if( height )
		*height	= h;
	if( !pixels )
	{
		delete [] buffer;
		return (-1);
	}
	/////////////////////////////////////////////////////
	// read the palette
	if( bitCount <= 8 )
	{
		// 24 and 32 bits images are not paletted
		// ajust the palette pointer to the memory in the buffer
		os2_palette = (RGBTRIPLE *)pBuff;
		win_palette = (RGBQUAD *)pBuff;
		//	[number of colors in the palette] * [size of one pixel]
		pBuff += (1 << bitCount) * (bitCount >> 3) * sizeof( unsigned char );
	}
	/////////////////////////////////////////////////////
	// allocate memory to store pixel data
	*pixels = new unsigned char[ w * h * 4 ];
	ptr		= &(*pixels)[0];
	// move the pixel data pointer to the begening of bitmap data
	pBuff = buffer + (bmfh->bfOffBits * sizeof( char ));
	/////////////////////////////////////////////////////
	// read pixel data following the image compression
	// type and the number of bits per pixels
	/////////////////////////////////////////////////////
	switch( compression )
	{
		case BI_OS2:
		case BI_RGB:
		{
			for( row = h - 1; row >= 0; row-- )
			{
				if( flipvert )
					ptr = &(*pixels)[ row * w * 4 ];
				switch( bitCount )
				{
					case 1:
					{
// RGB 1 BITS
						for( col = 0; col < (int)(w / 8); col++ )
						{
							// read the current pixel
							unsigned char color = *((unsigned char *)(pBuff++));

							for( i = 7; i >= 0; i--, ptr += 4 )
							{
								// convert indexed pixel (1 bit) into rgba (32 bits) pixel
								int clrIdx = ((color & (1<<i)) > 0);

								if( compression == BI_OS2 )
								{
									ptr[0] = os2_palette[ clrIdx ].rgbtRed;
									ptr[1] = os2_palette[ clrIdx ].rgbtGreen;
									ptr[2] = os2_palette[ clrIdx ].rgbtBlue;
									ptr[3] = 255;
								}
								else
								{
									ptr[0] = win_palette[ clrIdx ].rgbRed;
									ptr[1] = win_palette[ clrIdx ].rgbGreen;
									ptr[2] = win_palette[ clrIdx ].rgbBlue;
									ptr[3] = 255;
								}
							}
						}

						break;
					}

					case 4:
					{
// RGB 4 BITS
						for( col = 0; col < (int)(w / 2); col++, ptr += 8 )
						{
							// read the current pixel
							unsigned char color = *((unsigned char *)(pBuff++));

							// convert indexed pixel (4 bits) into rgba (32 bits) pixel
							int clrIdx;

							if( compression == BI_OS2 )
							{
								clrIdx = (color >> 4);
								ptr[0] = os2_palette[ clrIdx ].rgbtRed;
								ptr[1] = os2_palette[ clrIdx ].rgbtGreen;
								ptr[2] = os2_palette[ clrIdx ].rgbtBlue;
								ptr[3] = 255;

								clrIdx = (color & 0x0F);
								ptr[4] = os2_palette[ clrIdx ].rgbtRed;
								ptr[5] = os2_palette[ clrIdx ].rgbtGreen;
								ptr[6] = os2_palette[ clrIdx ].rgbtBlue;
								ptr[7] = 255;
							}
							else
							{
								clrIdx = (color >> 4);
								ptr[0] = win_palette[ clrIdx ].rgbRed;
								ptr[1] = win_palette[ clrIdx ].rgbGreen;
								ptr[2] = win_palette[ clrIdx ].rgbBlue;
								ptr[3] = 255;

								clrIdx = (color & 0x0F);
								ptr[4] = win_palette[ clrIdx ].rgbRed;
								ptr[5] = win_palette[ clrIdx ].rgbGreen;
								ptr[6] = win_palette[ clrIdx ].rgbBlue;
								ptr[7] = 255;
							}
						}

						break;
					}

					case 8:
					{
// RGB 8 BITS
						for( col = 0; col < w; col++, ptr += 4 )
						{
							// read the current pixel
							unsigned char color = *((unsigned char *)(pBuff++));

							// convert indexed pixel (8 bits) into rgba (32 bits) pixel
							if( compression == BI_OS2 )
							{
								ptr[0] = os2_palette[ color ].rgbtRed;
								ptr[1] = os2_palette[ color ].rgbtGreen;
								ptr[2] = os2_palette[ color ].rgbtBlue;
								ptr[3] = 255;
							}
							else
							{
								ptr[0] = win_palette[ color ].rgbRed;
								ptr[1] = win_palette[ color ].rgbGreen;
								ptr[2] = win_palette[ color ].rgbBlue;
								ptr[3] = 255;
							}
						}

						break;
					}

					case 24:
					{
// RGB 24 BITS
						for( col = 0; col < w; col++, ptr += 4 )
						{
							// convert bgr pixel (24 bits) into rgba (32 bits) pixel
							RGBTRIPLE *pix = (RGBTRIPLE *)pBuff;
							pBuff += sizeof( RGBTRIPLE );

							ptr[0] = pix->rgbtRed;
							ptr[1] = pix->rgbtGreen;
							ptr[2] = pix->rgbtBlue;
							ptr[3] = 255;
						}

  						break;
					}

					case 32:
					{
// RGB 32 BITS
						for( col = 0; col < w; col++, ptr += 4 )
						{
							// // convert bgr pixel (32 bits) into rgba (32 bits) pixel
							RGBQUAD *pix = (RGBQUAD *)pBuff;
							pBuff += sizeof( RGBQUAD );

							ptr[0] = pix->rgbRed;
							ptr[1] = pix->rgbGreen;
							ptr[2] = pix->rgbBlue;
							ptr[3] = 255;
						}

						break;
					}
				}
			}

			break;
		}

		case BI_RLE8:
		{
// RLE 8 BITS
			for( row = h - 1; row >= 0; row-- )
			{
				if( flipvert )
					ptr = &(*pixels)[ row * w * 4 ];

				for( col = 0; col < w; /* nothing */ )
				{
					// get one packet (2 bytes)
					unsigned char byte1 = *((unsigned char *)(pBuff++));
					unsigned char byte2 = *((unsigned char *)(pBuff++));


					if( byte1 == RLE_COMMAND )
					{
						// absolute encoding
						for( i = 0; i < byte2; i++, ptr += 4, col++ )
						{
							// read the current pixel
							unsigned char color = *((unsigned char *)(pBuff++));

							// convert indexed pixel (8 bits) into rgba (32 bits) pixel
							ptr[0] = win_palette[ color ].rgbRed;
							ptr[1] = win_palette[ color ].rgbGreen;
							ptr[2] = win_palette[ color ].rgbBlue;
							ptr[3] = 255;
						}

						if( (byte2 % 2) == 1 )
							pBuff++;
					}
					else
					{
						// read next pixels
						for( i = 0; i < byte1; i++, ptr += 4, col++ )
						{
							// convert indexed pixel (8 bits) into rgba (32 bits) pixel
							ptr[0] = win_palette[ byte2 ].rgbRed;
							ptr[1] = win_palette[ byte2 ].rgbGreen;
							ptr[2] = win_palette[ byte2 ].rgbBlue;
							ptr[3] = 255;
						}        
					}
				}
			}

			break;
		}

		case BI_RLE4:
		{
// RLE 4 BITS
			unsigned char color;
			int	bytesRead = 0;		// number of bytes read

			for( row = h - 1; row >= 0; row-- )
			{
				if( flipvert )
					ptr = &(*pixels)[ row * w * 4 ];

				for( col = 0; col < w; /* nothing */ )
				{
					// get one packet (2 bytes)
					unsigned char byte1 = *((unsigned char *)(pBuff++));
					unsigned char byte2 = *((unsigned char *)(pBuff++));

					bytesRead += 2;


					if( byte1 == RLE_COMMAND )
					{
						// absolute encoding
						unsigned char databyte;

						for( i = 0; i < byte2; i++, ptr += 4, col++ )
						{
							if( (i % 2) == 0 )
							{
								// read the current pixel
								databyte = *((unsigned char *)(pBuff++));
								bytesRead++;

								color = (databyte >> 4);	// 4 first bits
							}
							else
							{
								color = (databyte & 0x0F);	// 4 last bits
							}

							// convert indexed pixel (4 bits) into rgba (32 bits) pixel
							ptr[0] = win_palette[ color ].rgbRed;
							ptr[1] = win_palette[ color ].rgbGreen;
							ptr[2] = win_palette[ color ].rgbBlue;
							ptr[3] = 255;
						}
          
						while( (bytesRead % 2) != 0 )
						{
							pBuff++;
							bytesRead++;
						}
					}
					else
					{
						// read next pixels
						for( i = 0; i < byte1; i++, ptr += 4, col++ )
						{
							if( (i % 2) == 0 )
								color = (byte2 >> 4);	// 4 first bits
							else
								color = (byte2 & 0x0F);	// 4 last bits

							// convert indexed pixel (4 bits) into rgba (32 bits) pixel
							ptr[0] = win_palette[ color ].rgbRed;
							ptr[1] = win_palette[ color ].rgbGreen;
							ptr[2] = win_palette[ color ].rgbBlue;
							ptr[3] = 255;
						}        
					}
				}
			}

			break;
		}
	}


	// free buffer memory
	delete [] buffer;

	// return success
	return 1;
}

int LoadFilePCX( const char *filename, unsigned char **pixels, int *width, int *height, bool flipvert )
{
	FILE*		file;			// file stream
	PCXHEADER			*header;		// header PCX
	unsigned char		*data;			// uncompressed paletted image data
	unsigned char		*ptr;			// pointer to pixels data
	unsigned char		c;				// temporary variable
	char				*buffer;		// buffer storing the entire file
	int					idx = 0;		// temporary variable
	int					numRepeat;		// temporary variable
	int					i, j;			// temporary variables



	/////////////////////////////////////////////////////
	// read the entire file in the buffer
	file = fopen( filename, "rb" );
	if( file == NULL )
		return 0;
	fseek(file, 0, SEEK_END);
	long flen = ftell(file);
	fseek(file, 0, SEEK_SET);
	buffer = new char[ flen + 1 ];
	fread(buffer, flen, 1, file);
	char *pBuff = buffer;
	fclose(file);
	/////////////////////////////////////////////////////
	header = (PCXHEADER *)pBuff;
	if( (header->manufacturer	!= 10)	||
		(header->version		!= 5)	||
		(header->encoding		!= 1)	||
		(header->bitsPerPixel	!= 8) )
	{
		return 0;
	}
	header->width	= header->width	 - header->x + 1;
	header->height	= header->height - header->y + 1;
	if( width )
		*width = header->width;
	if( height )
		*height = header->height;
	if( !pixels )
	{
		delete [] buffer;
		return (-1);
	}
	// allocate memory for image data
	data = new unsigned char[ header->width * header->height ];
	pBuff = (char *)&buffer[ 128 ];
	// uncode compressed image (RLE)
	while( idx < (header->width * header->height) )
	{
		if( (c = *(pBuff++)) > 0xbf )
		{
			numRepeat = 0x3f & c;
			c = *(pBuff++);

			for( i = 0; i < numRepeat; i++ )
				data[ idx++ ] = c;
		}
		else
			data[ idx++ ] = c;
	}
	// the palette is located at the 769th last byte of the file
	pBuff = &buffer[ flen - 769 ];
	// verify the palette; first char must be equal to 12
	if( *(pBuff++) != 12 )
	{
		delete [] buffer;
		delete [] data;
		return 0;
	}
	// read the palette
	header->palette = (unsigned char *)pBuff;
	// allocate memory for 32 bits pixel data
	*pixels = new unsigned char[ header->width * header->height * 4 ];
	ptr = &(*pixels)[0];
	// convert from paletted to 32 bits rgba pixels
	for( j = header->height - 1; j > 0; j-- )
	{
		if( flipvert )
			ptr = &(*pixels)[ j * header->width * 4 ];

		for( i = 0; i < header->width; i++, ptr += 4 )
		{
			int color = 3 * data[ j * header->width + i ];

			ptr[0] = (unsigned char)header->palette[ color + 0 ];
			ptr[1] = (unsigned char)header->palette[ color + 1 ];
			ptr[2] = (unsigned char)header->palette[ color + 2 ];
			ptr[3] = (unsigned char)255;
		}
	}
	delete [] data;
	return 1;
}
int LoadFileTGA( const char *filename, unsigned char **pixels, int *width, int *height, bool flipvert )
{
	FILE*	file;			// file stream
	TGAHEADER		*tgah;			// targa header
	RGBTRIPLE		*palette;		// pointer on the color palette
	char			*buffer;		// buffer storing the entire file
	unsigned char	*ptr;			// pointer to pixels data
	int				row, col, i;	// temporary variables



	/////////////////////////////////////////////////////
	// read the entire file in the buffer
file = fopen( filename, "rb" );
	if( file == NULL )
		return 0;
	fseek(file, 0, SEEK_END);
	long flen = ftell(file);
	fseek(file, 0, SEEK_SET);
	buffer = new char[ flen + 1 ];
	fread(buffer, flen, 1, file);
	char *pBuff = buffer;
	fclose(file);
	// read the header
	tgah = (TGAHEADER *)pBuff;
	pBuff += sizeof( TGAHEADER );
	if( width )
		*width = tgah->is_width;
	if( height )
		*height = tgah->is_height;
	if( !pixels )
	{
		delete [] buffer;
		return (-1);
	}
	// allocate memory to store pixel data
	*pixels	= new unsigned char[ tgah->is_width * tgah->is_height * 4 ];
	ptr		= &(*pixels)[0];
	// move the pixel data pointer to the begening of bitmap data
	if( tgah->id_lenght )
		pBuff = buffer + (tgah->id_lenght * sizeof( unsigned char ));
	/////////////////////////////////////////////////////
	// read the palette
	if( tgah->color_map_type )
	{
		// 24 and 32 bits images are not paletted
		// ajust the palette pointer to the memory in the buffer
		palette = (RGBTRIPLE *)pBuff;
		pBuff += tgah->cm_length * (tgah->cm_size >> 3) * sizeof( unsigned char );
	}
	/////////////////////////////////////////////////////
	// read pixel data following the image compression
	// type and the number of bits per pixels
	/////////////////////////////////////////////////////
	switch( tgah->image_type )
	{
		case 0:
			// pas de données image
			break;
		case 1:
// COLOR-MAPPED BGR 8 BITS GREYSCALE
		case 3:
		{
// COLOR-MAPPED BGR 8 BITS
			for( row = tgah->is_height - 1; row >= 0; row-- )
			{
				if( flipvert )
					ptr = &(*pixels)[ row * tgah->is_width * 4 ];
				for( col = 0; col < tgah->is_width; col++, ptr += 4 )
				{
					// read the current pixel
					unsigned char color = *((unsigned char *)(pBuff++));
					// convert indexed pixel (8 bits) into rgba (32 bits) pixel
					ptr[0] = palette[ color ].rgbtRed;		// b->r
					ptr[1] = palette[ color ].rgbtGreen;	// g->g
					ptr[2] = palette[ color ].rgbtBlue;		// r->b
					ptr[3] = 255;							// alpha
				}
			}
			break;
		}
		case 2:
		{
			for( row = tgah->is_height - 1; row >= 0; row-- )
			{
				if( flipvert )
					ptr = &(*pixels)[ row * tgah->is_width * 4 ];
				for( col = 0; col < tgah->is_width; col++, ptr += 4 )
				{
					switch( tgah->is_pixel_depth )
					{
						case 16:
						{
// TRUE-COLOR BGR 16 BITS
							// read the current pixel
							unsigned short color = *((unsigned short *)pBuff);
							pBuff += sizeof( short );
							// convert bgr (16 bits) pixel into rgba (32 bits) pixel
							ptr[0] = ((color & 0x7C00) >> 10) << 3;	// b->r
							ptr[1] = ((color & 0x03E0) >>  5) << 3;	// g->g
							ptr[2] = ((color & 0x001F) >>  0) << 3;	// r->b
							ptr[3] = 255;							// alpha
							break;
						}
						case 24:
						{
// TRUE-COLOR BGR 24 BITS
							// convert bgr (24 bits) pixel into rgba (32 bits) pixel
							RGBTRIPLE *pix = (RGBTRIPLE *)pBuff;
							pBuff += sizeof( RGBTRIPLE );
							ptr[0] = pix->rgbtRed;
							ptr[1] = pix->rgbtGreen;
							ptr[2] = pix->rgbtBlue;
							ptr[3] = 255;
							break;
						}
						case 32:
						{
// TRUE-COLOR BGR 32 BITS
							// convert bgr (32 bits) pixel into rgba (32 bits) pixel
							BGRAQUAD *pix = (BGRAQUAD *)pBuff;
							pBuff += sizeof( BGRAQUAD );
							ptr[0] = pix->bgraRed;
							ptr[1] = pix->bgraGreen;
							ptr[2] = pix->bgraBlue;
							ptr[3] = pix->bgraAlpha;
							break;
						}
					}
				}
			}
			break;
		}
		case 9:
// RLE COLOR-MAPPED BGR 8 BITS
		case 11:
		{
// RLE COLOR-MAPPED BGR 8 BITS GREYSCALE
			unsigned char	packetHeader, packetSize, i;
			for( row = tgah->is_height - 1; row >= 0; row-- )
			{
				if( flipvert )
					ptr = &(*pixels)[ row * tgah->is_width * 4 ];
				for( col = 0; col < tgah->is_width; /* rien */ )
				{
					packetHeader	= *((unsigned char *)(pBuff++));
					packetSize		= 1 + (packetHeader & 0x7f);
					if( packetHeader & 0x80 )
					{
						// run-length packet
						// read the current pixel
						unsigned char color = *((unsigned char *)(pBuff++));
						// convert indexed pixel (8 bits) pixel into rgba (32 bits) pixel
						for( i = 0; i < packetSize; i++, ptr += 4, col++ )
						{
							ptr[0] = palette[ color ].rgbtRed;		// b->r
							ptr[1] = palette[ color ].rgbtGreen;	// g->g
							ptr[2] = palette[ color ].rgbtBlue;		// r->b
							ptr[3] = 255;							// alpha
						}
					}
					else
					{
						// non run-length packet
						for( i = 0; i < packetSize; i++, ptr += 4, col++ )
						{
							// read the current pixel
							unsigned char color = *((unsigned char *)(pBuff++));
							// convert indexed pixel (8 bits) pixel into rgba (32 bits) pixel
							ptr[0] = palette[ color ].rgbtRed;		// b->r
							ptr[1] = palette[ color ].rgbtGreen;	// g->g
							ptr[2] = palette[ color ].rgbtBlue;		// r->b
							ptr[3] = 255;							// alpha
						}
					}
				}
			}

			break;
		}
		case 10:
		{
			unsigned char	packetHeader, packetSize;
			for( row = tgah->is_height - 1; row >= 0; row-- )
			{
				if( flipvert )
					ptr = &(*pixels)[ row * tgah->is_width * 4 ];
				for( col = 0; col < tgah->is_width; /* rien */ )
				{
					packetHeader	= *((unsigned char *)(pBuff++));
					packetSize		= 1 + (packetHeader & 0x7f);
					if( packetHeader & 0x80 )
					{
						// run-length packet
						switch( tgah->is_pixel_depth )
						{
							case 16:
							{
// RLE TRUE-COLOR BGR 16 BITS
								// read the current pixel
								unsigned short color = *((unsigned short *)pBuff);
								pBuff += sizeof( short );
								// convert bgr (16 bits) pixel into rgba (32 bits) pixel
								for( i = 0; i < packetSize; i++, ptr += 4, col++ )
								{
									ptr[0] = ((color & 0x7C00) >> 10) << 3;	// b->r
									ptr[1] = ((color & 0x03E0) >>  5) << 3;	// g->g
									ptr[2] = ((color & 0x001F) >>  0) << 3;	// r->b
									ptr[3] = 255;
								}

								break;
							}
							case 24:
							{
// RLE TRUE-COLOR BGR 24 BITS
								// convert bgr (24 bits) pixel into rgba (32 bits) pixel
								RGBTRIPLE *pix = (RGBTRIPLE *)pBuff;
								pBuff += sizeof( RGBTRIPLE );
								for( i = 0; i < packetSize; i++, ptr += 4, col++ )
								{
									ptr[0] = pix->rgbtRed;
									ptr[1] = pix->rgbtGreen;
									ptr[2] = pix->rgbtBlue;
									ptr[3] = 255;
								}
								break;
							}
							case 32:
							{
// RLE TRUE-COLOR BGR 32 BITS
								// convert bgr (32 bits) pixel into rgba (32 bits) pixel
								BGRAQUAD *pix = (BGRAQUAD *)pBuff;
								pBuff += sizeof( BGRAQUAD );
								for( i = 0; i < packetSize; i++, ptr += 4, col++ )
								{
									ptr[0] = pix->bgraRed;
									ptr[1] = pix->bgraGreen;
									ptr[2] = pix->bgraBlue;
									ptr[3] = pix->bgraAlpha;
								}
								break;
							}
						}
					}
					else
					{
						// non run-length packet
						for( i = 0; i < packetSize; i++, ptr += 4, col++ )
						{
							switch( tgah->is_pixel_depth )
							{
								case 16:
								{
// RLE TRUE-COLOR BGR 16 BITS
									// read the current pixel
									unsigned short color = *((unsigned short *)pBuff);
									pBuff += sizeof( short );
									// convert bgr (16 bits) pixel into rgba (32 bits) pixel
									ptr[0] = ((color & 0x7C00) >> 10) << 3;	// b->r
									ptr[1] = ((color & 0x03E0) >>  5) << 3;	// g->g
									ptr[2] = ((color & 0x001F) >>  0) << 3;	// r->b
									ptr[3] = 255;							// alpha
									break;
								}
								case 24:
								{
// RLE TRUE-COLOR BGR 24 BITS
									// convert bgr (24 bits) pixel into rgba (32 bits) pixel
									RGBTRIPLE *pix = (RGBTRIPLE *)pBuff;
									pBuff += sizeof( RGBTRIPLE );
									ptr[0] = pix->rgbtRed;
									ptr[1] = pix->rgbtGreen;
									ptr[2] = pix->rgbtBlue;
									ptr[3] = 255;
									break;
								}
								case 32:
								{
// RLE TRUE-COLOR BGR 32 BITS
									// convert bgr (32 bits) pixel into rgba (32 bits) pixel
									BGRAQUAD *pix = (BGRAQUAD *)pBuff;
									pBuff += sizeof( BGRAQUAD );
									ptr[0] = pix->bgraRed;
									ptr[1] = pix->bgraGreen;
									ptr[2] = pix->bgraBlue;
									ptr[3] = pix->bgraAlpha;

									break;
								}
							}
						}
					}
				}
			}
			break;
		}
		default:
		{
			// unknown format
			delete [] pixels;
			delete [] buffer;
			return 0;
		}
	}
	delete [] buffer;
	return 1;
}
/////////////////////////////////////////////////////////////////////
unsigned int LoadTexture(char *filename)
{
	unsigned int	id = 0;
	unsigned char	*texels = 0;
	int				width, height;
	int				success;

	if( strstr( filename, ".bmp" ) || strstr( filename, ".BMP" ) )
		success = LoadFileBMP( filename, &texels, &width, &height, true );
	if( strstr( filename, ".tga" ) || strstr( filename, ".TGA" ) )
		success = LoadFileTGA( filename, &texels, &width, &height, true );
	if( strstr( filename, ".pcx" ) || strstr( filename, ".PCX" ) )
		success = LoadFilePCX( filename, &texels, &width, &height, true );
	if( success > 0 )
	{
		// create and initialize new texture
		glGenTextures( 1, &id );
		glBindTexture( GL_TEXTURE_2D, id );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		
		gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, texels );
	}
	else
	{
		id = NULL;
	}
	if( texels )
		delete [] texels;
	return id;
}