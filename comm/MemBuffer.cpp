// MemBuffer.cpp: implementation of the MemBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "MemBuffer.h"
#include "string.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define BUFF_STEP 16

bailin::MemBuffer::MemBuffer(int nInitLen /* = 16 */)
{
	m_pBuffer = NULL;
	m_nCurLen = 0;
	m_nMaxLen = 0;
	ReAllocBuffer(nInitLen);
//	CCLOG("----------MemBuffer Constructor------------ DEFAULT 0x%x", this);
}

bailin::MemBuffer::MemBuffer( char* pData, int nLen /*= 0*/ )
{
	m_pBuffer = NULL;
	m_nCurLen = 0;
	m_nMaxLen = 0;

	if (NULL != pData)
	{
//		CCLOG("bailin::MemBuffer::MemBuffer( char* pData, int nLen /*= 0*/ ) Append Begin");
		Append(pData, nLen);
//		CCLOG("bailin::MemBuffer::MemBuffer( char* pData, int nLen /*= 0*/ ) Append End");
	}
	else
	{
		ReAllocBuffer(32);
	}

//	CCLOG("----------MemBuffer Constructor------------With DATA 0x%x", this);
}

bailin::MemBuffer::MemBuffer( const MemBuffer& buff )
{
	m_pBuffer = NULL;
	m_nCurLen = 0;
	m_nMaxLen = 0;

	Append(buff.m_pBuffer, buff.m_nCurLen);

//	CCLOG("----------MemBuffer Constructor------------ COPY 0x%x", this);
}

bailin::MemBuffer::~MemBuffer()
{
//	CCLOG("----------MemBuffer Disconstructor------------ 0x%x", this);
	if (m_pBuffer)
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}
	m_nCurLen = 0;
	m_nMaxLen = 0;
}

bool bailin::MemBuffer::ReAllocBuffer( int nDeltaLen )
{
	int nStep = nDeltaLen > BUFF_STEP ? nDeltaLen : BUFF_STEP;

	char* pBuff = new char[m_nMaxLen + nStep];
	if (pBuff == NULL)
	{
		return false;
	}

	memset(pBuff, 0, nStep);

	if (NULL != m_pBuffer)
	{
		// Copy the original data to the new buffer
		memcpy(pBuff, m_pBuffer, m_nCurLen + 1);
		// Delete the old buffer
		delete [] m_pBuffer;
	}
	m_pBuffer = pBuff;

	m_nMaxLen += nStep;
	return true;
}

bool bailin::MemBuffer::Append( char* pData, int nLen /*= 0*/ )
{
	if (NULL == pData)
	{
		return false;
	}

	if (nLen <= 0)
	{
		// It means pData is terminate with \0
		if ((nLen = strlen(pData)) <= 0)
		{
			return false;
		}
	}

	// 补上\0的位置
	if (m_nCurLen + nLen + 1 > m_nMaxLen)
	{
//		CCLOG("<<<<<<<<<<<< Append ReAllockBuffer");
		if(!ReAllocBuffer(nLen + 1))
		{
			return false;
		}
	}

	memcpy(m_pBuffer + m_nCurLen, pData, nLen);
	m_nCurLen += nLen;
	*(m_pBuffer + m_nCurLen) = 0;

	return true;
}

inline int bailin::MemBuffer::GetBufferLen()
{
	return m_nCurLen;
}

inline char* bailin::MemBuffer::GetBuffer()
{
	return m_pBuffer;
}

void bailin::MemBuffer::Clear()
{
	m_nCurLen = 0;
	if (m_pBuffer && m_nMaxLen > 0)
	{
		m_pBuffer[0] = '\0';
	}
}

bool bailin::MemBuffer::RemoveLeft( int nLen )
{
	if (nLen >= m_nCurLen)
	{
		Clear();
	}

	if (nLen < m_nCurLen)
	{
		m_nCurLen -= nLen;

		memmove(m_pBuffer, m_pBuffer + nLen, m_nCurLen);

		// 后一位填0
		*(m_pBuffer + m_nCurLen) = 0;
	}

	return true;
}

bool bailin::MemBuffer::PopLeft( MemBuffer* pBuffer, int nLen )
{
	if (pBuffer == NULL || m_nCurLen < nLen)
	{
		return false;
	}

	// 拷贝数据
	pBuffer->Append(m_pBuffer, nLen);
	RemoveLeft(nLen);

	return true;
}
