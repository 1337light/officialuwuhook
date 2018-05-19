#pragma once
#include "../UTILS/CUtlVector.h"
#include "../includes.h"
namespace SDK
{

	class CGlowObjectManager
	{
	public:

		int RegisterGlowObject(SDK::CBaseEntity *pEntity, const Vector &vGlowColor, float flGlowAlpha, bool bRenderWhenOccluded, bool bRenderWhenUnoccluded, int nSplitScreenSlot)
		{
			int nIndex;
			if (m_nFirstFreeSlot == GlowObjectDefinition_t::END_OF_FREE_LIST)
			{
				nIndex = m_GlowObjectDefinitions.AddToTail();
			}
			else
			{
				nIndex = m_nFirstFreeSlot;
				m_nFirstFreeSlot = m_GlowObjectDefinitions[nIndex].m_nNextFreeSlot;
			}

			m_GlowObjectDefinitions[nIndex].m_hEntity = pEntity;
			m_GlowObjectDefinitions[nIndex].m_vGlowColor = vGlowColor;
			m_GlowObjectDefinitions[nIndex].m_flGlowAlpha = flGlowAlpha;
			m_GlowObjectDefinitions[nIndex].flUnk = 0.0f;
			m_GlowObjectDefinitions[nIndex].m_flBloomAmount = 1.0f;
			m_GlowObjectDefinitions[nIndex].localplayeriszeropoint3 = 0.0f;
			m_GlowObjectDefinitions[nIndex].m_bRenderWhenOccluded = bRenderWhenOccluded;
			m_GlowObjectDefinitions[nIndex].m_bRenderWhenUnoccluded = bRenderWhenUnoccluded;
			m_GlowObjectDefinitions[nIndex].m_bFullBloomRender = false;
			m_GlowObjectDefinitions[nIndex].m_nFullBloomStencilTestValue = 0;
			m_GlowObjectDefinitions[nIndex].m_nSplitScreenSlot = nSplitScreenSlot;
			m_GlowObjectDefinitions[nIndex].m_nNextFreeSlot = GlowObjectDefinition_t::ENTRY_IN_USE;

			return nIndex;
		}

		void UnregisterGlowObject(int nGlowObjectHandle)
		{
			Assert(!m_GlowObjectDefinitions[nGlowObjectHandle].IsUnused());

			m_GlowObjectDefinitions[nGlowObjectHandle].m_nNextFreeSlot = m_nFirstFreeSlot;
			m_GlowObjectDefinitions[nGlowObjectHandle].m_hEntity = NULL;
			m_nFirstFreeSlot = nGlowObjectHandle;
		}

		class GlowObjectDefinition_t
		{
		public:
			void set(float r, float g, float b, float a)
			{
				m_vGlowColor = Vector(r, g, b);
				m_flGlowAlpha = a;
				m_bRenderWhenOccluded = true;
				m_bRenderWhenUnoccluded = false;
				m_flBloomAmount = 1.0f;
			}

			SDK::CBaseEntity* getEnt()
			{
				return m_hEntity;
			}

			bool IsUnused() const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

		public:
			SDK::CBaseEntity * m_hEntity;
			Vector            m_vGlowColor;
			float            m_flGlowAlpha;

			char            unknown[4]; //pad 
			float            flUnk; //confirmed to be treated as a float while reversing glow functions 
			float            m_flBloomAmount;
			float            localplayeriszeropoint3;


			bool            m_bRenderWhenOccluded;
			bool            m_bRenderWhenUnoccluded;
			bool            m_bFullBloomRender;
			char            unknown1[1]; //pad 


			int                m_nFullBloomStencilTestValue; // 0x28 
			int                iUnk; //appears like it needs to be zero  
			int                m_nSplitScreenSlot; //Should be -1 

												   // Linked list of free slots 
			int                m_nNextFreeSlot;

			// Special values for GlowObjectDefinition_t::m_nNextFreeSlot 
			static const int END_OF_FREE_LIST = -1;
			static const int ENTRY_IN_USE = -2;
		};

		UTILS::CUtlVector< GlowObjectDefinition_t > m_GlowObjectDefinitions;
		int m_nFirstFreeSlot;
	};
}

