/*
 *   Copyright (C) 2011-2017 by Jonathan Naylor G4KLX
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef	MODEM_H
#define	MODEM_H

#include "RingBuffer.h"
#include "Defines.h"
#include "Timer.h"

#include <string>
#include <cstdint>

class CModem {
public:
	CModem(const std::string& port, bool duplex, bool rxInvert, bool txInvert, bool pttInvert, unsigned int txDelay, unsigned int dmrDelay, int oscOffset, const std::string& samplesDir, bool debug = false);
	~CModem();

	void setRFParams(unsigned int rxFrequency, unsigned int txFrequency);
	void setModeParams(bool dstarEnabled, bool dmrEnabled, bool ysfEnabled, bool p25Enabled);
	void setLevels(unsigned int rxLevel, unsigned int cwIdTXLevel, unsigned int dstarTXLevel, unsigned int dmrTXLevel, unsigned int ysfTXLevel, unsigned int p25Enabled);
	void setDMRParams(unsigned int colorCode);

	bool open();

	unsigned int readDStarData(unsigned char* data);
	unsigned int readDMRData1(unsigned char* data);
	unsigned int readDMRData2(unsigned char* data);
	unsigned int readYSFData(unsigned char* data);
	unsigned int readP25Data(unsigned char* data);

	unsigned int readSerial(unsigned char* data, unsigned int length);

	bool hasDStarSpace() const;
	bool hasDMRSpace1() const;
	bool hasDMRSpace2() const;
	bool hasYSFSpace() const;
	bool hasP25Space() const;

	bool hasTX() const;
	bool hasCD() const;

	bool hasLockout() const;
	bool hasError() const;

	bool writeDStarData(const unsigned char* data, unsigned int length);
	bool writeDMRData1(const unsigned char* data, unsigned int length);
	bool writeDMRData2(const unsigned char* data, unsigned int length);
	bool writeYSFData(const unsigned char* data, unsigned int length);
	bool writeP25Data(const unsigned char* data, unsigned int length);

	bool writeDMRStart(bool tx);
	bool writeDMRShortLC(const unsigned char* lc);
	bool writeDMRAbort(unsigned int slotNo);

	bool writeSerial(const unsigned char* data, unsigned int length);

	bool setMode(unsigned char mode);

	bool sendCWId(const std::string& callsign);

	HW_TYPE getHWType() const;

	void clock(unsigned int ms);

	void close();

private:
	std::string                m_samplesDir;
	bool                       m_debug;
	CRingBuffer<unsigned char> m_rxDStarData;
	CRingBuffer<unsigned char> m_rxDMRData2;
	CRingBuffer<unsigned char> m_rxYSFData;
	CRingBuffer<unsigned char> m_rxP25Data;
  CTimer                     m_dmrTimer;
	CTimer                     m_ysfTimer;
  CTimer                     m_p25Timer;
  FILE*                      m_dmrFP;
	FILE*                      m_ysfFP;
	FILE*                      m_p25FP;
  int16_t                    m_dmrThresholdVal;
  int16_t                    m_dmrCentreVal;
  int16_t                    m_dmrThreshold[16U];
  int16_t                    m_dmrCentre[16U];
  uint16_t                   m_dmrAveragePtr;
  int16_t                    m_ysfThresholdVal;
	int16_t                    m_ysfCentreVal;
	int16_t                    m_ysfThreshold[16U];
	int16_t                    m_ysfCentre[16U];
	uint16_t                   m_ysfAveragePtr;
	int16_t                    m_p25ThresholdVal;
	int16_t                    m_p25CentreVal;
	int16_t                    m_p25Threshold[16U];
	int16_t                    m_p25Centre[16U];
	uint16_t                   m_p25AveragePtr;

	void processYSF();
	void processP25();
  void processDMR();

  void dmrCalculateLevels(const int16_t* symbols);
  void dmrSamplesToBits(const int16_t* symbols, unsigned char* buffer);

  void ysfCalculateLevels(const int16_t* symbols);
	void ysfSamplesToBits(const int16_t* symbols, unsigned char* buffer);

	void p25CalculateLevels(const int16_t* symbols);
	void p25SamplesToBits(const int16_t* symbols, unsigned char* buffer);
};

#endif
