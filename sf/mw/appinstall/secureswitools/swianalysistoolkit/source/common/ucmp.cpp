/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
* Ported from us_ucmp.cpp.
* Implementation of the Standard Compression Scheme for Unicode.
* This code is compiled only in the Unicode build. 
*
*/


#include "ucmp.h"
#include <stdexcept>

namespace {
const int KErrNotFound = -1;
}

const TUint32 TUnicodeCompressionState::iStaticWindow[EStaticWindows] =
	{
	0x0000,		// tags
	0x0080,		// Latin-1 supplement
	0x0100,		// Latin Extended-A
	0x0300,		// Combining Diacritics
	0x2000,		// General Punctuation
	0x2080,		// Currency Symbols
	0x2100,		// Letterlike Symbols and Number Forms
	0x3000		// CJK Symbols and Punctuation
	};

const TUint32 TUnicodeCompressionState::iDynamicWindowDefault[EDynamicWindows] =
	{
	0x0080,		// Latin-1 supplement
	0x00C0,		// parts of Latin-1 supplement and Latin Extended-A
	0x0400,		// Cyrillic
	0x0600,		// Arabic
	0x0900,		// Devanagari
	0x3040,		// Hiragana
	0x30A0,		// Katakana
	0xFF00		// Fullwidth ASCII
	};

const TUint16 TUnicodeCompressionState::iSpecialBase[ESpecialBases] =
	{ 
	0x00C0,		// Latin 1 letters (not symbols) and some of Extended-A
	0x0250,		// IPA extensions
	0x0370,		// Greek
	0x0530,		// Armenian
	0x3040,		// Hiragana
	0x30A0,		// Katakana
	0xFF60		// Halfwidth katakana
	};

// Single-byte mode tag values
const TUint8 SQ0 = 0x01;	// <byte>				quote from window 0
const TUint8 SDX = 0x0B;	// <hbyte> <lbyte>		define window in expansion area
const TUint8 SQU = 0x0E;	// <hbyte> <lbyte>		quote Unicode value
const TUint8 SCU = 0x0F;	//						switch to Unicode mode
const TUint8 SC0 = 0x10;	//						select dynamic window 0
const TUint8 SD0 = 0x18;	// <byte>				set dynamic window 0 index to <byte> and select it

// Unicode mode tag values
const TUint8 UC0 = 0xE0;	//						select dynamic window 0 and switch to single-byte mode
const TUint8 UD0 = 0xE8;	// <byte>				set dynamic window 0 index to <byte>, select it and switch to
							//						single-byte mode
const TUint8 UQU = 0xF0;	// <hbyte>, <lbyte>		quote Unicode value
const TUint8 UDX = 0xF1;	// <hbyte>, <lbyte>		define window in expansion area and switch to single-byte mode


TUnicodeCompressionState::TUnicodeCompressionState():
	iUnicodeWords(0),
	iMaxUnicodeWords(0),
	iCompressedBytes(0),
	iMaxCompressedBytes(0)
	{
	Reset();
	}

void TUnicodeCompressionState::Reset()
	{
	iUnicodeMode = false;
	iActiveWindowBase = 0x0080;
	for (int i = 0; i < EDynamicWindows; i++)
		iDynamicWindow[i] = iDynamicWindowDefault[i];
	}


// Return the index of the static window that contains this code, if any, or -1 if there is none.
TInt32 TUnicodeCompressionState::StaticWindowIndex(TUint16 aCode)
	{
	for (TInt32 i = 0; i < EStaticWindows; i++)
		if (aCode >= iStaticWindow[i] && aCode < iStaticWindow[i] + 128)
			return i;
	return -1;
	}

/*
If aCode can be accommodated in one of the legal dynamic windows, return the index of that window
in the offset table. If not return KErrNotFound.
*/
TInt32 TUnicodeCompressionState::DynamicWindowOffsetIndex(TUint16 aCode)
	{
	if (aCode < 0x0080)
		return KErrNotFound;
	if (aCode >= 0x3400 && aCode <= 0xDFFF)
		return KErrNotFound;

	/*
	Prefer sections that cross half-->WriteUint8(block boundaries. These are better adapted to actual text.
	They are represented by offset indices 0xf9..0xff.
	*/
	for (int i = 0; i < ESpecialBases; i++)
		if (aCode >= iSpecialBase[i] && aCode < iSpecialBase[i] + 128)
			return 0xF9 + i;

	/*
	Offset indices 0x01..0x67 represent half blocks from 0x0080 to 0x3380 and
	0x68..0xA7 represent half blocks from 0xE000 to 0xFF80.
	*/
	if (aCode >= 0xE000)
		aCode -= 0xAC00;
	return aCode / 0x80;
	}

// Return the base of the window represented by offset index <n>. Return 0 if the offset index is illegal.
TUint32 TUnicodeCompressionState::DynamicWindowBase(TInt32 aOffsetIndex)
	{
	if (aOffsetIndex >= 0xF9 && aOffsetIndex <= 0xFF)
		{
		/*->WriteUint8(
		WARNING: don't optimise the following two lines by replacing them with
		'return iSpecialBase[aOffsetIndex - 0xF9];'. To do so would re-introduce a problem
		in ARM builds caused by optimisation and consequent erroneous fixing up
		of the array base: see defect EDNGASR-4AGJQX in ER5U defects.
		*/
		int special_base_index = aOffsetIndex - 0xF9;
		return iSpecialBase[special_base_index];
		}
	if (aOffsetIndex >= 0x01 && aOffsetIndex <= 0x67)
		return aOffsetIndex * 0x80;
	if (aOffsetIndex >= 0x68 && aOffsetIndex <= 0xA7)
		return aOffsetIndex * 0x80 + 0xAC00;
	return 0;
	}

TBool TUnicodeCompressionState::EncodeAsIs(TUint16 aCode)
	{
	return aCode == 0x0000 || aCode == 0x0009 || aCode == 0x000A || aCode == 0x000D ||
		   (aCode >= 0x0020 && aCode <= 0x007F);
	}

void TUnicodeCompressionState::Panic(TPanic /*aPanic*/)
	{
	throw std::runtime_error("ucmp");
	}

 TUnicodeCompressor::TUnicodeCompressor():
	iInputBufferStart(0),
	iInputBufferSize(0),
	iOutputBufferStart(0),
	iOutputBufferSize(0),
	iDynamicWindowIndex(0),
	iOutputStream(NULL),
	iOutputPointer(NULL),
	iInput(NULL)
	{
	}

 void TUnicodeCompressor::Compress(Serialiser& aOutput,MUnicodeSource& aInput,
											TInt32 aMaxOutputBytes,TInt32 aMaxInputWords,
											TInt32* aOutputBytes,TInt32* aInputWords)
	{
	DoCompress(&aOutput,NULL,&aInput,aMaxOutputBytes,aMaxInputWords,aOutputBytes,aInputWords);
	}

 void TUnicodeCompressor::Compress(TUint8* aOutput,MUnicodeSource& aInput,
											TInt32 aMaxOutputBytes,TInt32 aMaxInputWords,
											TInt32* aOutputBytes,TInt32* aInputWords)
	{
	DoCompress(NULL,aOutput,&aInput,aMaxOutputBytes,aMaxInputWords,aOutputBytes,aInputWords);
	}

 TInt32 TUnicodeCompressor::Flush(Serialiser& aOutput,TInt32 aMaxOutputBytes,TInt32& aOutputBytes)
	{
	DoCompress(&aOutput,NULL,NULL,aMaxOutputBytes,0,&aOutputBytes,NULL);
	return iOutputBufferSize;
	}

 TInt32 TUnicodeCompressor::Flush(TUint8* aOutput,TInt32 aMaxOutputBytes,TInt32& aOutputBytes)
	{
	DoCompress(NULL,aOutput,NULL,aMaxOutputBytes,0,&aOutputBytes,NULL);
	return iOutputBufferSize;
	}

 TInt32 TUnicodeCompressor::CompressedSize(MUnicodeSource& aInput,TInt32 aInputWords)
	{
	TInt32 bytes;
	TUnicodeCompressor c;
	c.DoCompress(NULL,NULL,&aInput,KMaxTInt,aInputWords,&bytes,NULL);
	return bytes;
	}

// Compress until input or output is exhausted or an exception occurs.
void TUnicodeCompressor::DoCompress(Serialiser* aOutputStream,TUint8* aOutputPointer,MUnicodeSource* aInput,
									 TInt32 aMaxOutputBytes,TInt32 aMaxInputWords,
									 TInt32* aOutputBytes,TInt32* aInputWords)
	{
	iOutputStream = aOutputStream;
	iOutputPointer = aOutputPointer;
	iInput = aInput;
	iMaxCompressedBytes = aMaxOutputBytes;
	iMaxUnicodeWords = aMaxInputWords;
	iCompressedBytes = iUnicodeWords = 0;
	FlushOutputBuffer();
	if (iInput)
		{
		while (iUnicodeWords < iMaxUnicodeWords && iCompressedBytes < iMaxCompressedBytes)
			{
			TUint16 x = iInput->ReadUnicodeValue();
			TAction action(x);
			iInputBuffer[(iInputBufferStart + iInputBufferSize) % EMaxInputBufferSize] = action;
			iInputBufferSize++;
			iUnicodeWords++;
			if (iInputBufferSize == EMaxInputBufferSize)
				WriteRun();
			}
		}
	FlushInputBuffer();
	if (aOutputBytes)
		*aOutputBytes = iCompressedBytes;
	if (aInputWords)
		*aInputWords = iUnicodeWords;
	}

TUnicodeCompressor::TAction::TAction(TUint16 aCode):
	iCode(aCode)
	{
	if (TUnicodeCompressionState::EncodeAsIs(aCode))
		iTreatment = EPlainASCII;
	else
		{
		iTreatment = TUnicodeCompressionState::DynamicWindowOffsetIndex(aCode);
		if (iTreatment == -1)
			{
			iTreatment = TUnicodeCompressionState::StaticWindowIndex(aCode);
			if (iTreatment == -1)
				iTreatment = EPlainUnicode;
			else
				iTreatment += EFirstStatic;
			}
		}
	}

void TUnicodeCompressor::WriteCharacterFromBuffer()
	{
	const TAction& action = iInputBuffer[iInputBufferStart];
	iInputBufferSize--;
	iInputBufferStart = (iInputBufferStart + 1) % EMaxInputBufferSize;
	WriteCharacter(action);
	}

void TUnicodeCompressor::FlushInputBuffer()
	{
	while (iInputBufferSize > 0 && iCompressedBytes < iMaxCompressedBytes)
		WriteRun();
	}

void TUnicodeCompressor::WriteRun()
	{
	// Write out any leading characters that can be passed through.
	if (!iUnicodeMode)
		while (iInputBufferSize > 0)
			{
			const TAction& action = iInputBuffer[iInputBufferStart];
			if (action.iTreatment == TAction::EPlainASCII ||
				(action.iCode >= iActiveWindowBase && action.iCode < iActiveWindowBase + 128))
				WriteCharacterFromBuffer();
			else
				break;
			}

	// Write a run of characters that cannot be passed through.
	int i;
	if (iInputBufferSize > 0)
		{
		/*
		Find a run of characters with the same treatment and select that treatment
		if the run has more than one character.
		*/
		int treatment = iInputBuffer[iInputBufferStart].iTreatment;
		int next_treatment = treatment;
		int run_size = 1;
		for (i = 1; i < iInputBufferSize; i++)
			{
			int index = (iInputBufferStart + i) % EMaxInputBufferSize;
			next_treatment = iInputBuffer[index].iTreatment;
			if (next_treatment != treatment)
				break;
			run_size++;
			}
		if (run_size > 1)
			SelectTreatment(treatment);
		for (i = 0; i < run_size; i++)
			WriteCharacterFromBuffer();
		}

	FlushOutputBuffer();
	}

void TUnicodeCompressor::FlushOutputBuffer()
	{
	while (iOutputBufferSize > 0 &&	iCompressedBytes < iMaxCompressedBytes)
		{
		TUint8 byte = iOutputBuffer[iOutputBufferStart];
		if (iOutputPointer)
			*iOutputPointer++ = byte;
		else if (iOutputStream)
			*iOutputStream << byte;
		iCompressedBytes++;
		iOutputBufferSize--;
		iOutputBufferStart = (iOutputBufferStart + 1) % EMaxOutputBufferSize;
		}
	}

void TUnicodeCompressor::SelectTreatment(TInt32 aTreatment)
	{
	if (aTreatment == TAction::EPlainUnicode)
		{
		// Switch to Unicode mode if not there already.
		if (!iUnicodeMode)
			{
			WriteByte(SCU);
			iUnicodeMode = true;
			}
		return;
		}

	if (aTreatment == TAction::EPlainASCII)
		{
		// Switch to single-byte mode, using the current dynamic window, if not there already.
		if (iUnicodeMode)
			{
			WriteByte(UC0 + iDynamicWindowIndex);
			iUnicodeMode = false;
			}
		return;
		}

	if (aTreatment >= TAction::EFirstDynamic && aTreatment <= TAction::ELastDynamic)
		{
		TUint32 base = DynamicWindowBase(aTreatment);

		// Switch to the appropriate dynamic window if it is available; if not, redefine and select dynamic window 4.
		for (int i = 0; i < EDynamicWindows; i++)
			if (base == iDynamicWindow[i])
				{
				if (iUnicodeMode)
					WriteByte(UC0 + i);
				else if (i != iDynamicWindowIndex)
					WriteByte(SC0 + i);
				iUnicodeMode = false;
				iDynamicWindowIndex = i;
				iActiveWindowBase = base;
				return;
				}
		if (iUnicodeMode)
			WriteByte(UD0 + 4);
		else
			WriteByte(SD0 + 4);
		iDynamicWindowIndex = 4;
		iUnicodeMode = false;
		WriteByte(aTreatment);
		iDynamicWindow[4] = base;
		iActiveWindowBase = base;
		return;
		}
	}

// Write a character without changing mode or window.
void TUnicodeCompressor::WriteCharacter(const TAction& aAction)
	{
	if (iUnicodeMode)
		WriteUCharacter(aAction.iCode);
	else
		WriteSCharacter(aAction);
	}

void TUnicodeCompressor::WriteUCharacter(TUint16 aCode)
	{
	// Emit the 'quote Unicode' tag if the character would conflict with a tag.
	if (aCode >= 0xE000 && aCode <= 0xF2FF)
		WriteByte(UQU);

	// Write the Unicode value big-end first.
	WriteByte((aCode >> 8) & 0xFF);
	WriteByte(aCode & 0xFF);
	}

void TUnicodeCompressor::WriteByte(TUint32 aByte)
	{
	if (iOutputBufferSize >= EMaxOutputBufferSize)
		Panic(EOutputBufferOverflow);
	iOutputBuffer[(iOutputBufferStart + iOutputBufferSize) % EMaxOutputBufferSize] = (TUint8)aByte;
	iOutputBufferSize++;
	}

void TUnicodeCompressor::WriteSCharacter(const TAction& aAction)
	{
	// Characters in the range 0x0020..0x007F, plus nul, tab, cr, and lf, can be emitted as their low bytes.
	if (aAction.iTreatment == TAction::EPlainASCII)
		{
		WriteByte(aAction.iCode);
		return;
		}

	// Characters in a static window can be written using SQ<n> plus a byte in the range 0x00-0x7F
	if (aAction.iTreatment >= TAction::EFirstStatic && aAction.iTreatment <= TAction::ELastStatic)
		{
		int window = aAction.iTreatment - TAction::EFirstStatic;
		WriteByte(SQ0 + window);
		WriteByte(aAction.iCode);
		return;
		}

	// Characters in the current dynamic window can be written as a byte in the range 0x80-0xFF.
	if (aAction.iCode >= iActiveWindowBase && aAction.iCode < iActiveWindowBase + 128)
		{
		WriteByte(aAction.iCode - iActiveWindowBase + 0x80);
		return;
		}

	// Characters in another dynamic window can be written using SQ<n> plus a byte in the range 0x80-0xFF
	int i;
	for (i = 0; i < EDynamicWindows; i++)
		if (aAction.iCode >= iDynamicWindow[i] && aAction.iCode < iDynamicWindow[i] + 128)
			{
			WriteByte(SQ0 + i);
			WriteByte(aAction.iCode - iDynamicWindow[i] + 0x80);
			return;
			}

	// Other characters can be quoted.
	WriteByte(SQU);
	WriteByte((aAction.iCode >> 8) & 0xFF);
	WriteByte(aAction.iCode & 0xFF);
	return;
	}

 TUnicodeExpander::TUnicodeExpander():
	iInputBufferStart(0),
	iInputBufferSize(0),
	iOutputBufferStart(0),
	iOutputBufferSize(0),
	iOutput(NULL),
	iInputStream(NULL),
	iInputPointer(NULL)
	{
	}

 void TUnicodeExpander::Expand(MUnicodeSink& aOutput,Deserialiser& aInput,
										TInt32 aMaxOutputWords,TInt32 aMaxInputBytes,
										TInt32* aOutputWords,TInt32* aInputBytes)
	{
	DoExpand(&aOutput,&aInput,NULL,aMaxOutputWords,aMaxInputBytes,aOutputWords,aInputBytes);
	}

 void TUnicodeExpander::Expand(MUnicodeSink& aOutput,const TUint8* aInput,
										TInt32 aMaxOutputWords,TInt32 aMaxInputBytes,
										TInt32* aOutputWords,TInt32* aInputBytes)
	{
	DoExpand(&aOutput,NULL,aInput,aMaxOutputWords,aMaxInputBytes,aOutputWords,aInputBytes);
	}

 TInt32 TUnicodeExpander::Flush(MUnicodeSink& aOutput,TInt32 aMaxOutputWords,TInt32& aOutputWords)
	{
	DoExpand(&aOutput,NULL,NULL,aMaxOutputWords,0,&aOutputWords,NULL);
	return iOutputBufferSize;
	}

 TInt32 TUnicodeExpander::ExpandedSize(Deserialiser& aInput,TInt32 aInputBytes)
	{
	TInt32 words;
	TUnicodeExpander e;
	e.DoExpand(NULL,&aInput,NULL,KMaxTInt,aInputBytes,&words,NULL);
	return words;
	}

 TInt32 TUnicodeExpander::ExpandedSize(const TUint8* aInput,TInt32 aInputBytes)
	{
	TInt32 words;
	TUnicodeExpander e;
	e.DoExpand(NULL,NULL,aInput,KMaxTInt,aInputBytes,&words,NULL);
	return words;
	}

// Expand until input or output is exhausted or an exception occurs.
void TUnicodeExpander::DoExpand(MUnicodeSink* aOutput,Deserialiser* aInputStream,const TUint8* aInputPointer,
								 TInt32 aMaxOutputWords,TInt32 aMaxInputBytes,
								 TInt32* aOutputWords,TInt32* aInputBytes)
	{
	iOutput = aOutput;
	iInputStream = aInputStream;
	iInputPointer = aInputPointer;
	iMaxUnicodeWords = aMaxOutputWords;
	iMaxCompressedBytes = aMaxInputBytes;
	iUnicodeWords = iCompressedBytes = 0;
	iInputBufferStart = 0;
	FlushOutputBuffer();
	if (iInputPointer || iInputStream)
		{
		while (iUnicodeWords + iOutputBufferSize < iMaxUnicodeWords && iCompressedBytes < iMaxCompressedBytes)
			HandleByte();
		}
	if (aOutputWords)
		*aOutputWords = iUnicodeWords;
	if (aInputBytes)
		*aInputBytes = iCompressedBytes;
	}

void TUnicodeExpander::HandleByte()
	{
	TUint8 byte;
	TBool handled = false;
	if (ReadByte(byte))
		{
		if (iUnicodeMode)
			handled = HandleUByte(byte);
		else
			handled = HandleSByte(byte);
		}
	iInputBufferStart = 0;
	if (handled)
		iInputBufferSize = 0;
	FlushOutputBuffer();
	}

void TUnicodeExpander::FlushOutputBuffer()
	{
	while (iOutputBufferSize > 0 &&	iUnicodeWords < iMaxUnicodeWords)
		{
		if (iOutput)
			iOutput->WriteUnicodeValue(iOutputBuffer[iOutputBufferStart]);
		iUnicodeWords++;
		iOutputBufferSize--;
		iOutputBufferStart = (iOutputBufferStart + 1) % EMaxOutputBufferSize;
		}
	}

TBool TUnicodeExpander::HandleSByte(TUint8 aByte)
	{
	// 'Pass-through' codes.
	if (TUnicodeCompressionState::EncodeAsIs(aByte))
		{
		WriteChar(aByte);
		return true;
		}

	// Codes 0x80-0xFF select a character from the active window.
	if (aByte >= 0x80)
		{
		WriteChar32(iActiveWindowBase + aByte - 0x80);
		return true;
		}

	// SQU: quote a Unicode character.
	if (aByte == SQU)
		return QuoteUnicode();

	// SCU: switch to Unicode mode.
	if (aByte == SCU)
		{
		iUnicodeMode = true;
		return true;
		}

	// SQn: quote from window n.
	if (aByte >= SQ0 && aByte <= SQ0 + 7)
		{
		int window = aByte - SQ0;
		TUint8 byte;
		if (ReadByte(byte))
			{
			TUint32 c = byte;
			if (c <= 0x7F)
				c += iStaticWindow[window];
			else
				c += iDynamicWindow[window] - 0x80;
			WriteChar32(c);
			return true;
			}
		else
			return false;
		}

	// SCn: switch to dynamic window n.
	if (aByte >= SC0 && aByte <= SC0 + 7)
		{
		iActiveWindowBase = iDynamicWindow[aByte - SC0];
		return true;
		}

	// SDn: define dynamic window n and switch to it.
	if (aByte >= SD0 && aByte <= SD0 + 7)
		return DefineWindow(aByte - SD0);

	// SDX: define window in the expansion space.
	if (aByte == SDX)
		return DefineExpansionWindow();

	Panic(EUnhandledByte);
	return false;
	}

TBool TUnicodeExpander::HandleUByte(TUint8 aByte)
	{
	// Plain Unicode; get the low byte and emit the Unicode value.
	if (aByte <= 0xDF || aByte >= 0xF3)
		{
		TUint8 lo;
		if (ReadByte(lo))
			{
			TUint16 c = (TUint16)((aByte << 8) | lo);
			WriteChar(c);
			return true;
			}
		else
			return false;
		}

	// Quote a Unicode character that would otherwise conflict with a tag.
	if (aByte == UQU)
		return QuoteUnicode();

	// UCn: change to single byte mode and select window n.
	if (aByte >= UC0 && aByte <= UC0 + 7)
		{
		iUnicodeMode = false;
		iActiveWindowBase = iDynamicWindow[aByte - UC0];
		return true;
		}

	// UDn: define dynamic window n and switch to it.
	if (aByte >= UD0 && aByte <= UD0 + 7)
		return DefineWindow(aByte - UD0);

	// UDX: define window in the expansion space.
	if (aByte == UDX)
		return DefineExpansionWindow();

	Panic(EUnhandledByte);
	return false;
	}

TBool TUnicodeExpander::QuoteUnicode()
	{
	TUint8 hi, lo;
	if (ReadByte(hi) && ReadByte(lo))
		{
		TUint16 c = (TUint16)((hi << 8) | lo);
		WriteChar(c);
		return true;
		}
	else
		return false;
	}

TBool TUnicodeExpander::DefineWindow(TInt32 aIndex)
	{
	TUint8 window;
	if (ReadByte(window))
		{
		iUnicodeMode = false;
		iActiveWindowBase = DynamicWindowBase(window);
		iDynamicWindow[aIndex] = iActiveWindowBase;
		return true;
		}
	else
		return false;
	}

TBool TUnicodeExpander::DefineExpansionWindow()
	{
	TUint8 hi, lo;
	if (ReadByte(hi) && ReadByte(lo))
		{
		iUnicodeMode = false;
		iActiveWindowBase = 0x10000 + (0x80 * ((hi & 0x1F) * 0x100 + lo));
		iDynamicWindow[hi >> 5] = iActiveWindowBase;
		return true;
		}
	else
		return false;
	}

// Read either from the buffer (in the case of restarting after source finished in mid-operation) or from the source.
TBool TUnicodeExpander::ReadByte(TUint8& aByte)
	{
	if (iInputBufferStart < iInputBufferSize)
		{
		aByte = iInputBuffer[iInputBufferStart++];
		return true;
		}
	else if (iCompressedBytes < iMaxCompressedBytes)
		{
		if (iInputPointer)
			aByte = *iInputPointer++;
		else
			*iInputStream >> aByte;
		iInputBuffer[iInputBufferStart++] = aByte;
		iInputBufferSize = iInputBufferStart;
		iCompressedBytes++;
		return true;
		}
	else
		return false;
	}

void TUnicodeExpander::WriteChar(TUint16 aChar)
	{
	if (iOutputBufferSize >= EMaxOutputBufferSize)
		Panic(EOutputBufferOverflow);
	iOutputBuffer[(iOutputBufferStart + iOutputBufferSize) % EMaxOutputBufferSize] = aChar;
	iOutputBufferSize++;
	}

// Write a Unicode character; write using surrogates if in the range 0x10000..0x10FFFF.
void TUnicodeExpander::WriteChar32(TUint32 aChar)
	{
	if (aChar <= 0xFFFF)
		WriteChar((TUint16)aChar);
	else if (aChar <= 0x10FFFF)
		{
		aChar -= 0x10000;									// reduce to 20-bit value in the range 0x0..0xFFFFF
		WriteChar((TUint16)(0xD800 + (aChar >> 10)));		// first high surrogate + high 10 bits
		WriteChar((TUint16)(0xDC00 + (aChar & 0x03FF)));	// first low surrogate + low 10 bits
		}
	else
		Panic(ENotUnicode);
	}


void TMemoryStreamUnicodeSink::WriteUnicodeValue(TUint16 aValue)
	{
	iStream.write((const char*)&aValue,sizeof(TUint16));
	}

TUint16 TMemoryStreamUnicodeSource::ReadUnicodeValue()
	{
	TUint16 x;
	iStream.read((char *)&x,sizeof(TUint16));
	return x;
	}

