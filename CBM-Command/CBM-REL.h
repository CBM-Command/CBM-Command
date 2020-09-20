/***************************************************************
Copyright (c) 2012, Payton Byrd
All rights reserved.

Redistribution and use in source and binary forms, with or
without modification, are permitted provided that the following
conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the following
  disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the following
  disclaimer in the documentation and/or other materials
  provided with the distribution.

* Neither the name of Payton Byrd nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior written
  permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***************************************************************/

#ifndef _CBM_REL_H
#define _CBM_REL_H

//#include <stdbool.h>

#ifndef __fastcall
#define __fastcall __fastcall__
#endif

struct position_rel
{
	char cmd;
	unsigned char channel;
	unsigned rec_number;
	unsigned char position;
};

struct rel_file
{
	char file_name[17];
	unsigned char record_size;
	unsigned int record_count;
};

struct rel_file_rec
{
	struct rel_file* file;
	unsigned int record_number;
	unsigned char record_data[255];
};

unsigned char __fastcall getRecordSize(
	const unsigned char command_lfn,
	const unsigned char unit_number,
	const char* path,
	const char* file_name);
//
//struct rel_file* __fastcall openRelativeFile(
//	const unsigned char logical_file_number,
//	const unsigned char unit_number,
//	const unsigned char secondary,
//	const unsigned char record_size,
//	const char* file_name,
//	bool count_records);
//
//signed char __fastcall getRecord(
//	const unsigned char logical_file_number,
//	const unsigned char unit_number,
//	const unsigned char secondary,
//	struct rel_file_rec* record,
//	const unsigned char rel_size);
//
//signed char __fastcall saveRecord(
//	const unsigned char logical_file_number,
//	const unsigned char unit_number,
//	const unsigned char secondary,
//	struct rel_file_rec* record,
//	const unsigned char rel_size);
//
//signed char __fastcall initializeRelativeFile(
//	const unsigned char unit_number,
//	const unsigned char record_size,
//	const unsigned int record_count,
//	const char* file_name);

#endif
