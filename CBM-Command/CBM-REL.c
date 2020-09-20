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

//#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//#include <conio.h>
//#include <ctype.h>
#include <cbm.h>

#include "CBM-REL.h"
#include "drives.h"
#include "globals.h"
#include "screen.h"

/* Returns the RELative file's record-size (2-254);
** or, returns zero on an error.
*/
unsigned char __fastcall getRecordSize(
	const unsigned char command_lfn,
	const unsigned char unit_number,
	const char* path,
	const char* file_name)
{
	//const static unsigned char s=128;
	char relFileName[16+2+1];
	static struct position_rel command = {'p', 2, 1};
	//static char errorBuffer[256];

	sprintf(relFileName, "%s,l", file_name);

	if (cbmOpen(2, unit_number, 2, path, relFileName, command_lfn) == 0)
	{
		// File found -- find the record length.  Start at the maximum;
		// and, shrink.  The position command fails when the target is
		// beyond the end of the record; it succeeds when the target
		// reaches the last byte in the record -- that spot is the size.
		//
		writeStatusBar("Getting record size");
		command.position = 254;
		do
		{
			cbm_write(command_lfn, &command, sizeof command);

			cbm_read(command_lfn, buffer, sizeof buffer);
			if(buffer[0] == '0')
			{
				break;
			}
		}
		while (--command.position > 0);

		cbm_close(2);
		return command.position;
	}
	cbm_close(2);
	return 0;
}
//
//struct rel_file* __fastcall openRelativeFile(
//	const unsigned char logical_file_number,
//	const unsigned char unit_number,
//	const unsigned char secondary,
//	const unsigned char record_size,
//	const char* file_name,
//	bool count_records)
//{
//	static unsigned char file[22], command[] = "p    ";
//	signed char result;
//	unsigned int count;
//	struct rel_file file_struct;
//
//
//	//printf("\r\n--------------------\r\nOpening relative file %s", file_name);
//	sprintf(file, ":%s,l,%c", file_name, record_size);
//
//	//cbm_open(127u, unit_number, 15u, "");
//	result = cbm_open(logical_file_number, unit_number, secondary, file);
//	//errorBuffer[cbm_read(127u, errorBuffer, sizeof errorBuffer)] = '\0';
//	//printf("errorChannel: %s", errorBuffer);
//	//cbm_close(127u);
//
//	if(result != 5)
//	{
//		strcpy(file_struct.file_name, file_name);
//		file_struct.record_size = record_size;
//		if(count_records)
//		{
//			cbm_open(127u, unit_number, 15u, "");
//			command[1] = secondary;
//			command[4] = 1u;
//			for(count = 0; count < 65535; ++count)
//			{
//				(unsigned*)&command[2] = count;
//				result = cbm_write(127u, command, 5u);
//				if(result == -1) break;
//			}
//			cbm_close(127u);
//		}
//
//		file_struct.record_count = count;
//	}
//
//	return &file_struct;
//}
//
//signed char __fastcall getRecord(
//	const unsigned char logical_file_number,
//	const unsigned char unit_number,
//	const unsigned char secondary,
//	struct rel_file_rec* record,
//	const unsigned char rel_size)
//{
//	signed char result;
//	static unsigned char errorBuffer[41], file[21], command[] = "p    ";
//	//printf("\r\n-----------------------\r\nLFN: %u  UN: %u  Sec: %u", logical_file_number, unit_number, secondary);
//	cbm_open(127u, unit_number, 15u, "");
//	command[1] = 96 + logical_file_number;
//	(unsigned*)&command[2] = record->record_number;
//	command[4] = rel_size;
//			 cbm_write(127u, command, 5u); // needed by a VICE virtual-drive bug
//	result = cbm_write(127u, command, 5u);
//	if(result > -1)
//	{
//		cbm_read(logical_file_number, record->record_data, rel_size);
//		cbm_read(127u, errorBuffer, sizeof errorBuffer);
//		//errorBuffer[2] = '\0';
//		result=atoi(errorBuffer);
//	}
//	cbm_close(127u);
//
//	return result;
//}
//
//signed char __fastcall saveRecord(
//	const unsigned char logical_file_number,
//	const unsigned char unit_number,
//	const unsigned char secondary,
//	struct rel_file_rec* record,
//	const unsigned char rel_size)
//{
//	signed char result;
//	static unsigned char errorBuffer[256], file[21], command[] = "p    ";
//
//	cbm_open(127u, unit_number, 15u, "");
//	command[1] = 96 + logical_file_number;
//	(unsigned*)&command[2] = record->record_number;
//	command[4] = rel_size;
//	result = cbm_write(127u, command, 5u);
//	errorBuffer[cbm_read(127u, errorBuffer, sizeof errorBuffer)] = '\0';
//	printf("\r\n================\r\nWrite Step 1 -- result: %d, errorBuffer: %s\r\n", result, errorBuffer);
//	if(result > -1)
//	{
//		result = cbm_write(logical_file_number, record->record_data, rel_size);
//		errorBuffer[cbm_read(127u, errorBuffer, sizeof errorBuffer)] = '\0';
//		printf("\r\n================\r\nWrite Step 2 -- result: %d, errorBuffer: %s\r\n", result, errorBuffer);
//	}
//	result = cbm_write(127u, command, 5u);
//	errorBuffer[cbm_read(127u, errorBuffer, sizeof errorBuffer)] = '\0';
//	printf("\r\n================\r\nWrite Step 3 -- result: %d, errorBuffer: %s\r\n", result, errorBuffer);
//	cbm_close(127u);
//
//	return result;
//}
//
//signed char __fastcall initializeRelativeFile(
//	const unsigned char unit_number,
//	const unsigned char record_size,
//	const unsigned int record_count,
//	const char* file_name)
//{
//	static signed char result;
//	static unsigned int count;
//	static struct rel_file* file;
//	static struct rel_file_rec rec;
//
//	file = openRelativeFile(
//		126u, unit_number, 2u,
//		record_size, file_name, false);
//	if(file != NULL)
//	{
//		rec.file = file;
//
//		for(count = 0; count < record_size; ++count)
//		{
//			rec.record_data[count] = (unsigned char)count;
//		}
//
//		for(count = 0; count < record_count; ++count)
//		{
//			rec.record_number = count;
//			result = saveRecord(126, unit_number, 2, &rec, record_size);
//			if(result == -1)
//			{
//				break;
//			}
//		}
//
//		cbm_close(126);
//		return result;
//	}
//
//	return -1;
//}
