#ifndef __FLOPPY__H
#define __FLOPPY__H

/* io ports */
#define FDC_DOR  (0x3f2)   /* Digital Output Register */
#define FDC_MSR  (0x3f4)   /* Main Status Register (input) */
#define FDC_DRS  (0x3f4)   /* Data Rate Select Register (output) */
#define FDC_DATA (0x3f5)   /* Data Register */
#define FDC_DIR  (0x3f7)   /* Digital Input Register (input) */
#define FDC_CCR  (0x3f7)   /* Configuration Control Register (output) */

/* command bytes (these are 765 commands + options such as MFM, etc) */
#define FDC_CMD_RESET   (0x00)
#define FDC_CMD_SPECIFY (0x03)  /* specify drive timings */
#define FDC_CMD_WRITE   (0xc5)  /* write data (+ MT,MFM) */
#define FDC_CMD_READ    (0xe6)  /* read data (+ MT,MFM,SK) */
#define FDC_CMD_RECAL   (0x07)  /* recalibrate */
#define FDC_CMD_SENSEI  (0x08)  /* sense interrupt status */
#define FDC_CMD_FORMAT  (0x4d)  /* format track (+ MFM) */
#define FDC_CMD_SEEK    (0x0f)  /* seek track */
#define FDC_CMD_VERSION (0x10)  /* FDC version */

/* drive geometries */
#define DG144_HEADS       2     /* heads per drive (1.44M) */
#define DG144_TRACKS     80     /* number of tracks (1.44M) */
#define DG144_SPT        18     /* sectors per track (1.44M) */
#define DG144_GAP3FMT  0x54     /* gap3 while formatting (1.44M) */
#define DG144_GAP3RW   0x1b     /* gap3 while reading/writing (1.44M) */

#define DG168_HEADS       2     /* heads per drive (1.68M) */
#define DG168_TRACKS     80     /* number of tracks (1.68M) */
#define DG168_SPT        21     /* sectors per track (1.68M) */
#define DG168_GAP3FMT  0x0c     /* gap3 while formatting (1.68M) */
#define DG168_GAP3RW   0x1c     /* gap3 while reading/writing (1.68M) */

#define FLOPPYD_READ	1
#define FLOPPYD_WRITE	2
#define FLOPPYD_DONE	0xFFFF

void fdc_block2hts(uint16 block, uint16 *head, uint16 *track, uint16 *sector);
uint8 fdc_wait(uint8 send_sensei);
void fdc_sendbyte(uint8 byte);
uint8 fdc_getbyte();
void fdc_start_motor();
void fdc_stop_motor();
void fdc_seek(uint16 track);
void fdc_recalibrate();
void fdc_reset();
void fdc_readwrite(uint16 block, int read);
void fdc_init();

#endif
